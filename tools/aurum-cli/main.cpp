#include <chrono>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>

#include "Aurum.h"

namespace {

// Function declarations
std::string lowerCopy(std::string s);
std::string escapeForQuote(const std::string &src);

enum ExitCode {
    EXIT_OK = 0,
    EXIT_ERROR = 1,
    EXIT_INVALID_ARGS = 2,
    EXIT_NOT_FOUND = 3,
    EXIT_ACTION_FAILED = 4,
};

using Options = std::unordered_map<std::string, std::string>;
std::unordered_map<std::string, std::shared_ptr<Aurum::UiObject>> gRefObjectMap;
constexpr const char *DAEMON_SOCKET_PATH = "/tmp/aurum-cli-daemon.sock";
constexpr int DAEMON_IDLE_TIMEOUT_SEC = 600;

int parsePositiveInt(const std::string &text, const std::string &optName);
bool shouldIncludeInSnapshot(const std::shared_ptr<Aurum::AccessibleNode> &node, const std::string &role);
std::string roleOrFallback(const std::shared_ptr<Aurum::AccessibleNode> &node);
std::shared_ptr<Aurum::UiObject> findTargetObject(const std::shared_ptr<Aurum::UiDevice> &device,
                                                  const Options &opts,
                                                  int &exitCode,
                                                  std::string &detail);

void printUsage()
{
    std::cout
        << "Usage: aurum-cli <command> [options]\n\n"
        << "Commands:\n"
        << "  find       --text <value> | --id <value> | --automationid <value>\n"
        << "  click      selector options\n"
        << "  dblclick   selector options\n"
        << "  longclick  selector options [--duration-ms <ms>]\n"
        << "  focus      selector options\n"
        << "  fill       selector options + --value <new text>\n"
        << "  wait       selector options [--timeout-ms <ms>]\n"
        << "  press      --key <KEYCODE> [--type stroke|long-stroke|press|release|repeat]\n"
        << "  drag       --sx <x> --sy <y> --ex <x> --ey <y> [--steps 10] [--duration-ms 500]\n"
        << "  get        --field <text|value|role|description|count> + selector options\n"
        << "  is         --field <visible|enabled|checked|focusable|clickable> + selector options\n"
        << "  screenshot --path <file>\n"
        << "  snapshot   [--path <file>]\n"
        << "  screen-size\n"
        << "  help\n\n"
        << "Selector options (one or more): --text, --id, --automationid, --role, --type, --style\n"
        << "Reference target: pass @eN from latest snapshot in the same process (example: aurum-cli click @e1)\n";
}

bool parseOptions(const std::vector<std::string> &args, Options &options, std::string &err)
{
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string &token = args[i];
        if (!token.empty() && token[0] == '@') {
            if (token.size() < 3 || token[1] != 'e') {
                err = "invalid ref token: " + token;
                return false;
            }
            options["--ref"] = token.substr(1); // e.g. e1
            continue;
        }

        if (token.rfind("--", 0) != 0) {
            err = "expected option starting with '--': " + token;
            return false;
        }
        if (i + 1 >= args.size()) {
            err = "missing value for option: " + token;
            return false;
        }
        options[token] = args[i + 1];
        ++i;
    }
    return true;
}

std::shared_ptr<Aurum::UiSelector> buildSelector(const Options &options)
{
    auto selector = std::make_shared<Aurum::UiSelector>();
    bool hasAnySelector = false;

    auto add = [&](const char *name, auto setter) {
        const auto iter = options.find(name);
        if (iter == options.end())
            return;
        setter(iter->second);
        hasAnySelector = true;
    };

    add("--text", [&](const std::string &v) { selector->text(v); });
    add("--id", [&](const std::string &v) { selector->id(v); });
    add("--automationid", [&](const std::string &v) { selector->automationid(v); });
    add("--role", [&](const std::string &v) { selector->role(v); });
    add("--type", [&](const std::string &v) { selector->type(v); });
    add("--style", [&](const std::string &v) { selector->style(v); });

    if (!hasAnySelector)
        return nullptr;
    return selector;
}

Aurum::KeyRequestType parseKeyType(const std::string &raw)
{
    if (raw == "stroke")
        return Aurum::KeyRequestType::STROKE;
    if (raw == "long-stroke")
        return Aurum::KeyRequestType::LONG_STROKE;
    if (raw == "press")
        return Aurum::KeyRequestType::PRESS;
    if (raw == "release")
        return Aurum::KeyRequestType::RELEASE;
    if (raw == "repeat")
        return Aurum::KeyRequestType::REPEAT;
    throw std::invalid_argument("invalid --type for press: " + raw);
}

int runFind(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    std::cout << "status=ok text=\"" << obj->getText() << "\" role=\"" << obj->getRole() << "\"\n";
    return EXIT_OK;
}

bool parseRefIndex(const std::string &rawRef, int &index)
{
    std::string ref = rawRef;
    if (!ref.empty() && ref[0] == '@')
        ref = ref.substr(1);
    if (ref.size() < 2 || ref[0] != 'e')
        return false;
    try {
        int parsed = std::stoi(ref.substr(1));
        if (parsed <= 0)
            return false;
        index = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

std::shared_ptr<Aurum::UiObject> resolveObjectByRef(const std::shared_ptr<Aurum::UiDevice> &device,
                                                    const std::string &rawRef)
{
    (void)device;
    int targetIndex = 0;
    if (!parseRefIndex(rawRef, targetIndex))
        return nullptr;

    const auto refKey = "e" + std::to_string(targetIndex);
    const auto iter = gRefObjectMap.find(refKey);
    if (iter == gRefObjectMap.end())
        return nullptr;
    return iter->second;
}

std::shared_ptr<Aurum::UiObject> findTargetObject(const std::shared_ptr<Aurum::UiDevice> &device,
                                                  const Options &opts,
                                                  int &exitCode,
                                                  std::string &detail)
{
    const auto refIter = opts.find("--ref");
    if (refIter != opts.end()) {
        auto refObj = resolveObjectByRef(device, refIter->second);
        if (!refObj) {
            exitCode = EXIT_NOT_FOUND;
            detail = "ref not found (run snapshot in same process first)";
            return nullptr;
        }
        return refObj;
    }

    auto selector = buildSelector(opts);
    if (!selector) {
        exitCode = EXIT_INVALID_ARGS;
        detail = "selector option required";
        return nullptr;
    }

    auto obj = device->findObject(selector);
    if (!obj) {
        exitCode = EXIT_NOT_FOUND;
        detail = "not found";
        return nullptr;
    }

    return obj;
}

int runClick(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    obj->click();
    std::cout << "status=ok action=click\n";
    return EXIT_OK;
}

int runDoubleClick(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    obj->click();
    obj->click();
    std::cout << "status=ok action=dblclick\n";
    return EXIT_OK;
}

int runLongClick(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    unsigned int durationMs = 500;
    const auto durIter = opts.find("--duration-ms");
    if (durIter != opts.end()) {
        try {
            durationMs = static_cast<unsigned int>(parsePositiveInt(durIter->second, "--duration-ms"));
        } catch (const std::invalid_argument &e) {
            std::cerr << "status=error code=INVALID_ARGS detail='" << e.what() << "'\n";
            return EXIT_INVALID_ARGS;
        }
    }

    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    obj->longClick(durationMs);
    std::cout << "status=ok action=longclick duration-ms=" << durationMs << "\n";
    return EXIT_OK;
}

int runFocus(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    bool ok = obj->setFocus();
    if (!ok) {
        std::cerr << "status=error code=ACTION_FAILED detail='setFocus returned false'\n";
        return EXIT_ACTION_FAILED;
    }

    std::cout << "status=ok action=focus\n";
    return EXIT_OK;
}

int runSetText(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    const auto valIter = opts.find("--value");
    if (valIter == opts.end()) {
        std::cerr << "status=error code=INVALID_ARGS detail='--value is required'\n";
        return EXIT_INVALID_ARGS;
    }

    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    bool ok = obj->setText(valIter->second);
    if (!ok) {
        std::cerr << "status=error code=ACTION_FAILED detail='setText returned false'\n";
        return EXIT_ACTION_FAILED;
    }

    std::cout << "status=ok action=fill\n";
    return EXIT_OK;
}

int parsePositiveInt(const std::string &text, const std::string &optName)
{
    try {
        int parsed = std::stoi(text);
        if (parsed < 0)
            throw std::invalid_argument("negative");
        return parsed;
    } catch (...) {
        throw std::invalid_argument("invalid integer for " + optName + ": " + text);
    }
}

int runWait(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    auto selector = buildSelector(opts);
    if (!selector) {
        std::cerr << "status=error code=INVALID_ARGS detail='selector option required'\n";
        return EXIT_INVALID_ARGS;
    }

    int timeoutMs = 5000;
    const auto timeoutIter = opts.find("--timeout-ms");
    if (timeoutIter != opts.end()) {
        try {
            timeoutMs = parsePositiveInt(timeoutIter->second, "--timeout-ms");
        } catch (const std::invalid_argument &e) {
            std::cerr << "status=error code=INVALID_ARGS detail='" << e.what() << "'\n";
            return EXIT_INVALID_ARGS;
        }
    }

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    while (std::chrono::steady_clock::now() < deadline) {
        auto obj = device->findObject(selector);
        if (obj) {
            std::cout << "status=ok waited-ms<=" << timeoutMs << "\n";
            return EXIT_OK;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cerr << "status=error code=NOT_FOUND detail='wait timeout'\n";
    return EXIT_NOT_FOUND;
}

int runPressKey(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    const auto keyIter = opts.find("--key");
    if (keyIter == opts.end()) {
        std::cerr << "status=error code=INVALID_ARGS detail='--key is required'\n";
        return EXIT_INVALID_ARGS;
    }

    Aurum::KeyRequestType keyType = Aurum::KeyRequestType::STROKE;
    const auto typeIter = opts.find("--type");
    if (typeIter != opts.end()) {
        try {
            keyType = parseKeyType(typeIter->second);
        } catch (const std::invalid_argument &e) {
            std::cerr << "status=error code=INVALID_ARGS detail='" << e.what() << "'\n";
            return EXIT_INVALID_ARGS;
        }
    }

    bool ok = device->pressKeyCode(keyIter->second, keyType);
    if (!ok) {
        std::cerr << "status=error code=ACTION_FAILED detail='pressKeyCode returned false'\n";
        return EXIT_ACTION_FAILED;
    }

    std::cout << "status=ok action=press\n";
    return EXIT_OK;
}

int runDrag(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    const std::vector<std::string> required = {"--sx", "--sy", "--ex", "--ey"};
    for (const auto &opt : required) {
        if (opts.find(opt) == opts.end()) {
            std::cerr << "status=error code=INVALID_ARGS detail='missing " << opt << "'\n";
            return EXIT_INVALID_ARGS;
        }
    }

    int sx = 0, sy = 0, ex = 0, ey = 0, steps = 10, durationMs = 500;
    try {
        sx = parsePositiveInt(opts.at("--sx"), "--sx");
        sy = parsePositiveInt(opts.at("--sy"), "--sy");
        ex = parsePositiveInt(opts.at("--ex"), "--ex");
        ey = parsePositiveInt(opts.at("--ey"), "--ey");
        if (opts.find("--steps") != opts.end())
            steps = parsePositiveInt(opts.at("--steps"), "--steps");
        if (opts.find("--duration-ms") != opts.end())
            durationMs = parsePositiveInt(opts.at("--duration-ms"), "--duration-ms");
    } catch (const std::invalid_argument &e) {
        std::cerr << "status=error code=INVALID_ARGS detail='" << e.what() << "'\n";
        return EXIT_INVALID_ARGS;
    }

    bool ok = device->drag(sx, sy, ex, ey, steps, durationMs);
    if (!ok) {
        std::cerr << "status=error code=ACTION_FAILED detail='drag returned false'\n";
        return EXIT_ACTION_FAILED;
    }

    std::cout << "status=ok action=drag\n";
    return EXIT_OK;
}

int runGet(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    const auto fieldIter = opts.find("--field");
    if (fieldIter == opts.end()) {
        std::cerr << "status=error code=INVALID_ARGS detail='--field is required'\n";
        return EXIT_INVALID_ARGS;
    }
    const std::string field = lowerCopy(fieldIter->second);

    if (field == "count") {
        const auto refIter = opts.find("--ref");
        if (refIter != opts.end()) {
            int refIndex = 0;
            if (!parseRefIndex(refIter->second, refIndex)) {
                std::cerr << "status=error code=INVALID_ARGS detail='invalid --ref'\n";
                return EXIT_INVALID_ARGS;
            }
            std::cout << "status=ok field=count value=1\n";
            return EXIT_OK;
        }
        auto selector = buildSelector(opts);
        if (!selector) {
            std::cerr << "status=error code=INVALID_ARGS detail='selector option required'\n";
            return EXIT_INVALID_ARGS;
        }
        auto objs = device->findObjects(selector);
        std::cout << "status=ok field=count value=" << objs.size() << "\n";
        return EXIT_OK;
    }

    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    if (field == "text") {
        std::cout << "status=ok field=text value=\"" << escapeForQuote(obj->getText()) << "\"\n";
        return EXIT_OK;
    }
    if (field == "value") {
        std::cout << "status=ok field=value value=\"" << escapeForQuote(obj->getValueText()) << "\"\n";
        return EXIT_OK;
    }
    if (field == "role") {
        std::cout << "status=ok field=role value=\"" << escapeForQuote(obj->getRole()) << "\"\n";
        return EXIT_OK;
    }
    if (field == "description") {
        std::cout << "status=ok field=description value=\"" << escapeForQuote(obj->getDescription()) << "\"\n";
        return EXIT_OK;
    }

    std::cerr << "status=error code=INVALID_ARGS detail='unsupported get field'\n";
    return EXIT_INVALID_ARGS;
}

int runIs(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    const auto fieldIter = opts.find("--field");
    if (fieldIter == opts.end()) {
        std::cerr << "status=error code=INVALID_ARGS detail='--field is required'\n";
        return EXIT_INVALID_ARGS;
    }
    const std::string field = lowerCopy(fieldIter->second);
    int err = EXIT_OK;
    std::string detail;
    auto obj = findTargetObject(device, opts, err, detail);
    if (!obj) {
        std::cerr << "status=error code=" << (err == EXIT_INVALID_ARGS ? "INVALID_ARGS" : "NOT_FOUND")
                  << " detail='" << detail << "'\n";
        return err;
    }

    bool result = false;
    if (field == "visible") {
        result = obj->isVisible();
    } else if (field == "enabled") {
        result = obj->isEnabled();
    } else if (field == "checked") {
        result = obj->isChecked();
    } else if (field == "focusable") {
        result = obj->isFocusable();
    } else if (field == "clickable") {
        result = obj->isClickable();
    } else {
        std::cerr << "status=error code=INVALID_ARGS detail='unsupported is field'\n";
        return EXIT_INVALID_ARGS;
    }

    std::cout << "status=ok field=" << field << " value=" << (result ? "true" : "false") << "\n";
    return EXIT_OK;
}

int runScreenshot(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    const auto pathIter = opts.find("--path");
    if (pathIter == opts.end()) {
        std::cerr << "status=error code=INVALID_ARGS detail='--path is required'\n";
        return EXIT_INVALID_ARGS;
    }

    bool ok = device->takeScreenshot(pathIter->second, false, nullptr);
    if (!ok) {
        std::cerr << "status=error code=ACTION_FAILED detail='takeScreenshot returned false'\n";
        return EXIT_ACTION_FAILED;
    }

    std::cout << "status=ok action=screenshot path=\"" << pathIter->second << "\"\n";
    return EXIT_OK;
}

int runScreenSize(const std::shared_ptr<Aurum::UiDevice> &device)
{
    auto size = device->getScreenSize();
    std::cout << "status=ok width=" << size.width << " height=" << size.height << "\n";
    return EXIT_OK;
}

std::string trim(const std::string &src)
{
    size_t first = 0;
    while (first < src.size() && std::isspace(static_cast<unsigned char>(src[first]))) {
        ++first;
    }
    if (first == src.size())
        return "";
    size_t last = src.size() - 1;
    while (last > first && std::isspace(static_cast<unsigned char>(src[last]))) {
        --last;
    }
    return src.substr(first, last - first + 1);
}

std::string collapseWhitespace(const std::string &src)
{
    std::string out;
    out.reserve(src.size());
    bool inSpace = false;
    for (char c : src) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!inSpace) {
                out.push_back(' ');
                inSpace = true;
            }
            continue;
        }
        inSpace = false;
        out.push_back(c);
    }
    return trim(out);
}

std::string lowerCopy(std::string s)
{
    for (auto &c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

std::string escapeForQuote(const std::string &src)
{
    std::string out;
    out.reserve(src.size());
    for (char c : src) {
        if (c == '"' || c == '\\') {
            out.push_back('\\');
        }
        out.push_back(c);
    }
    return out;
}

std::string roleOrFallback(const std::shared_ptr<Aurum::AccessibleNode> &node)
{
    auto role = lowerCopy(collapseWhitespace(node->getRole()));
    if (!role.empty())
        return role;

    auto type = lowerCopy(collapseWhitespace(node->getType()));
    if (!type.empty())
        return type;
    return "node";
}

enum class SnapshotRoleGroup {
    INTERACTIVE,
    CONTENT,
    STRUCTURAL,
    UNKNOWN,
};

SnapshotRoleGroup classifySnapshotRole(const std::string &role)
{
    static const std::unordered_set<std::string> interactiveRoles = {
        "check box", "menu item", "password text", "push button", "radio button",
        "radio menu item", "scroll bar", "slider", "spin button", "text",
        "toggle button", "edit bar", "entry", "link",
    };

    static const std::unordered_set<std::string> contentRoles = {
        "icon", "image", "label", "list item", "progress bar", "status bar",
        "text", "tool tip", "notification", "info bar", "level bar",
        "title bar", "video",
    };

    static const std::unordered_set<std::string> structuralRoles = {
        "dialog", "list", "menu", "menu bar", "panel", "popup menu",
        "scroll pane", "separator", "split pane", "table", "window",
        "embedded", "input method window", "tool b", "tool bar",
    };

    if (interactiveRoles.find(role) != interactiveRoles.end()) {
        return SnapshotRoleGroup::INTERACTIVE;
    }
    if (contentRoles.find(role) != contentRoles.end()) {
        return SnapshotRoleGroup::CONTENT;
    }
    if (structuralRoles.find(role) != structuralRoles.end()) {
        return SnapshotRoleGroup::STRUCTURAL;
    }
    return SnapshotRoleGroup::UNKNOWN;
}

const char *snapshotGroupText(SnapshotRoleGroup group)
{
    switch (group) {
    case SnapshotRoleGroup::INTERACTIVE:
        return "interactive";
    case SnapshotRoleGroup::CONTENT:
        return "content";
    case SnapshotRoleGroup::STRUCTURAL:
        return "structural";
    default:
        return "unknown";
    }
}

bool shouldIncludeInSnapshot(const std::shared_ptr<Aurum::AccessibleNode> &node, const std::string &role)
{
    const SnapshotRoleGroup group = classifySnapshotRole(role);
    if (node->isClickable() || node->isFocusable()) {
        return true;
    }

    const auto text = collapseWhitespace(node->getText());
    const auto description = collapseWhitespace(node->getDescription());
    const bool hasLabel = !text.empty() || !description.empty();

    if (group == SnapshotRoleGroup::INTERACTIVE) {
        return true;
    }
    if (group == SnapshotRoleGroup::CONTENT) {
        return hasLabel;
    }
    if (group == SnapshotRoleGroup::STRUCTURAL) {
        return hasLabel && role != "window";
    }
    return hasLabel && role != "node";
}

void appendSnapshotLines(const std::shared_ptr<Aurum::UiDevice> &device,
                         const std::shared_ptr<Aurum::AccessibleNode> &node,
                         int &refIndex,
                         std::ostringstream &out)
{
    const std::string role = roleOrFallback(node);
    const std::string text = collapseWhitespace(node->getText());
    const std::string description = collapseWhitespace(node->getDescription());
    const std::string label = !text.empty() ? text : description;

    if (shouldIncludeInSnapshot(node, role)) {
        const SnapshotRoleGroup group = classifySnapshotRole(role);
        const std::string refKey = "e" + std::to_string(refIndex++);
        out << "[ref=" << refKey << "] " << role << " \"" << escapeForQuote(label) << "\"";
        out << " [" << snapshotGroupText(group) << "]";
        if (node->isClickable()) {
            out << " [clickable]";
        }
        if (node->isFocusable()) {
            out << " [focusable]";
        }
        out << "\n";

        gRefObjectMap[refKey] = std::make_shared<Aurum::UiObject>(device, nullptr, node);
    }

    for (const auto &child : node->getChildren()) {
        appendSnapshotLines(device, child, refIndex, out);
    }
}

int runSnapshot(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    auto roots = device->getWindowRoot();
    std::ostringstream out;
    gRefObjectMap.clear();
    int refIndex = 1;
    for (const auto &root : roots) {
        appendSnapshotLines(device, root, refIndex, out);
    }

    const auto pathIter = opts.find("--path");
    if (pathIter != opts.end()) {
        std::ofstream file(pathIter->second);
        if (!file.is_open()) {
            std::cerr << "status=error code=ACTION_FAILED detail='failed to open snapshot path'\n";
            return EXIT_ACTION_FAILED;
        }
        file << out.str();
        std::cout << "status=ok action=snapshot path=\"" << pathIter->second
                  << "\" refs=" << (refIndex - 1) << "\n";
        return EXIT_OK;
    }

    std::cout << out.str();
    return EXIT_OK;
}

} // namespace

namespace {

int runCliCommand(const std::vector<std::string> &argvList)
{
    if (argvList.empty()) {
        printUsage();
        return EXIT_INVALID_ARGS;
    }

    const std::string command = argvList[0];
    if (command == "help" || command == "--help" || command == "-h") {
        printUsage();
        return EXIT_OK;
    }

    std::vector<std::string> rawArgs;
    rawArgs.reserve(argvList.size() > 1 ? argvList.size() - 1 : 0);
    for (size_t i = 1; i < argvList.size(); ++i) {
        rawArgs.emplace_back(argvList[i]);
    }

    Options opts;
    std::string parseError;
    if (!parseOptions(rawArgs, opts, parseError)) {
        std::cerr << "status=error code=INVALID_ARGS detail='" << parseError << "'\n";
        return EXIT_INVALID_ARGS;
    }

    Aurum::AccessibleWatcher::getInstance();
    auto device = Aurum::UiDevice::getInstance();

    try {
        if (command == "find")
            return runFind(device, opts);
        if (command == "click")
            return runClick(device, opts);
        if (command == "dblclick")
            return runDoubleClick(device, opts);
        if (command == "longclick")
            return runLongClick(device, opts);
        if (command == "focus")
            return runFocus(device, opts);
        if (command == "fill")
            return runSetText(device, opts);
        if (command == "wait")
            return runWait(device, opts);
        if (command == "press")
            return runPressKey(device, opts);
        if (command == "drag")
            return runDrag(device, opts);
        if (command == "get")
            return runGet(device, opts);
        if (command == "is")
            return runIs(device, opts);
        if (command == "screenshot")
            return runScreenshot(device, opts);
        if (command == "snapshot")
            return runSnapshot(device, opts);
        if (command == "screen-size")
            return runScreenSize(device);

        std::cerr << "status=error code=INVALID_ARGS detail='unknown command: " << command << "'\n";
        printUsage();
        return EXIT_INVALID_ARGS;
    } catch (const std::exception &e) {
        std::cerr << "status=error code=ERROR detail='" << e.what() << "'\n";
        return EXIT_ERROR;
    }
}

bool connectToDaemon(int &sockFd)
{
    sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockFd < 0)
        return false;

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", DAEMON_SOCKET_PATH);
    if (connect(sockFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        close(sockFd);
        sockFd = -1;
        return false;
    }
    return true;
}

bool startDaemonProcess(const char *selfPath)
{
    pid_t pid = fork();
    if (pid < 0)
        return false;
    if (pid == 0) {
        execl(selfPath, selfPath, "--daemon-run", nullptr);
        _exit(127);
    }
    int status = 0;
    (void)waitpid(pid, &status, WNOHANG);
    return true;
}

bool sendAll(int fd, const std::string &data)
{
    size_t sent = 0;
    while (sent < data.size()) {
        ssize_t n = send(fd, data.data() + sent, data.size() - sent, 0);
        if (n <= 0)
            return false;
        sent += static_cast<size_t>(n);
    }
    return true;
}

std::string recvAll(int fd)
{
    std::string out;
    char buf[4096];
    while (true) {
        ssize_t n = recv(fd, buf, sizeof(buf), 0);
        if (n <= 0)
            break;
        out.append(buf, static_cast<size_t>(n));
    }
    return out;
}

int runClient(int argc, char **argv)
{
    if (argc < 2) {
        printUsage();
        return EXIT_INVALID_ARGS;
    }

    int sockFd = -1;
    if (!connectToDaemon(sockFd)) {
        if (!startDaemonProcess(argv[0])) {
            std::cerr << "status=error code=ERROR detail='failed to start daemon'\n";
            return EXIT_ERROR;
        }

        bool connected = false;
        for (int i = 0; i < 50; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (connectToDaemon(sockFd)) {
                connected = true;
                break;
            }
        }
        if (!connected) {
            std::cerr << "status=error code=ERROR detail='daemon did not start'\n";
            return EXIT_ERROR;
        }
    }

    std::string payload;
    for (int i = 1; i < argc; ++i) {
        payload += argv[i];
        payload.push_back('\n');
    }

    if (!sendAll(sockFd, payload)) {
        close(sockFd);
        std::cerr << "status=error code=ERROR detail='failed to send request'\n";
        return EXIT_ERROR;
    }
    shutdown(sockFd, SHUT_WR);

    std::string response = recvAll(sockFd);
    close(sockFd);
    if (response.empty()) {
        std::cerr << "status=error code=ERROR detail='empty daemon response'\n";
        return EXIT_ERROR;
    }

    auto nl = response.find('\n');
    if (nl == std::string::npos) {
        std::cerr << "status=error code=ERROR detail='invalid daemon response'\n";
        return EXIT_ERROR;
    }
    int code = EXIT_ERROR;
    try {
        code = std::stoi(response.substr(0, nl));
    } catch (...) {
        std::cerr << "status=error code=ERROR detail='invalid response code'\n";
        return EXIT_ERROR;
    }

    const std::string output = response.substr(nl + 1);
    std::cout << output;
    return code;
}

int runDaemon()
{
    unlink(DAEMON_SOCKET_PATH);
    int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverFd < 0)
        return EXIT_ERROR;

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", DAEMON_SOCKET_PATH);

    if (bind(serverFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        close(serverFd);
        return EXIT_ERROR;
    }

    if (listen(serverFd, 16) < 0) {
        close(serverFd);
        return EXIT_ERROR;
    }

    while (true) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(serverFd, &rfds);
        timeval tv{};
        tv.tv_sec = DAEMON_IDLE_TIMEOUT_SEC;
        tv.tv_usec = 0;

        int sel = select(serverFd + 1, &rfds, nullptr, nullptr, &tv);
        if (sel == 0) {
            break; // idle timeout
        }
        if (sel < 0) {
            continue;
        }

        int clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0) {
            continue;
        }

        std::string request = recvAll(clientFd);
        std::vector<std::string> args;
        std::stringstream ss(request);
        std::string line;
        while (std::getline(ss, line)) {
            if (!line.empty())
                args.push_back(line);
        }

        std::ostringstream captured;
        auto *coutBuf = std::cout.rdbuf(captured.rdbuf());
        auto *cerrBuf = std::cerr.rdbuf(captured.rdbuf());
        int code = runCliCommand(args);
        std::cout.rdbuf(coutBuf);
        std::cerr.rdbuf(cerrBuf);

        std::string response = std::to_string(code) + "\n" + captured.str();
        (void)sendAll(clientFd, response);
        close(clientFd);
    }

    close(serverFd);
    unlink(DAEMON_SOCKET_PATH);
    return EXIT_OK;
}

} // namespace

int main(int argc, char **argv)
{
    if (argc >= 2 && std::string(argv[1]) == "--daemon-run") {
        return runDaemon();
    }
    return runClient(argc, argv);
}
