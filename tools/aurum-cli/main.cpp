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

#include "Aurum.h"

namespace {

enum ExitCode {
    EXIT_OK = 0,
    EXIT_ERROR = 1,
    EXIT_INVALID_ARGS = 2,
    EXIT_NOT_FOUND = 3,
    EXIT_ACTION_FAILED = 4,
};

using Options = std::unordered_map<std::string, std::string>;

void printUsage()
{
    std::cout
        << "Usage: aurum-cli <command> [options]\n\n"
        << "Commands:\n"
        << "  find       --text <value> | --id <value> | --automationid <value>\n"
        << "  click      selector options\n"
        << "  set-text   selector options + --value <new text>\n"
        << "  wait       selector options [--timeout-ms <ms>]\n"
        << "  press-key  --key <KEYCODE> [--type stroke|long-stroke|press|release|repeat]\n"
        << "  screenshot --path <file>\n"
        << "  snapshot   [--path <file>]\n"
        << "  screen-size\n"
        << "  help\n\n"
        << "Selector options (one or more): --text, --id, --automationid, --role, --type, --style\n";
}

bool parseOptions(const std::vector<std::string> &args, Options &options, std::string &err)
{
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string &token = args[i];
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
    throw std::invalid_argument("invalid --type for press-key: " + raw);
}

int runFind(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    auto selector = buildSelector(opts);
    if (!selector) {
        std::cerr << "status=error code=INVALID_ARGS detail='selector option required'\n";
        return EXIT_INVALID_ARGS;
    }

    auto obj = device->findObject(selector);
    if (!obj) {
        std::cerr << "status=error code=NOT_FOUND\n";
        return EXIT_NOT_FOUND;
    }

    std::cout << "status=ok text=\"" << obj->getText() << "\" role=\"" << obj->getRole() << "\"\n";
    return EXIT_OK;
}

int runClick(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    auto selector = buildSelector(opts);
    if (!selector) {
        std::cerr << "status=error code=INVALID_ARGS detail='selector option required'\n";
        return EXIT_INVALID_ARGS;
    }

    auto obj = device->findObject(selector);
    if (!obj) {
        std::cerr << "status=error code=NOT_FOUND\n";
        return EXIT_NOT_FOUND;
    }

    obj->click();
    std::cout << "status=ok action=click\n";
    return EXIT_OK;
}

int runSetText(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    const auto valIter = opts.find("--value");
    if (valIter == opts.end()) {
        std::cerr << "status=error code=INVALID_ARGS detail='--value is required'\n";
        return EXIT_INVALID_ARGS;
    }

    auto selector = buildSelector(opts);
    if (!selector) {
        std::cerr << "status=error code=INVALID_ARGS detail='selector option required'\n";
        return EXIT_INVALID_ARGS;
    }

    auto obj = device->findObject(selector);
    if (!obj) {
        std::cerr << "status=error code=NOT_FOUND\n";
        return EXIT_NOT_FOUND;
    }

    bool ok = obj->setText(valIter->second);
    if (!ok) {
        std::cerr << "status=error code=ACTION_FAILED detail='setText returned false'\n";
        return EXIT_ACTION_FAILED;
    }

    std::cout << "status=ok action=set-text\n";
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

    std::cout << "status=ok action=press-key\n";
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

void appendSnapshotLines(const std::shared_ptr<Aurum::AccessibleNode> &node, int &refIndex, std::ostringstream &out)
{
    const std::string role = roleOrFallback(node);
    const std::string text = collapseWhitespace(node->getText());
    const std::string description = collapseWhitespace(node->getDescription());
    const std::string label = !text.empty() ? text : description;

    if (shouldIncludeInSnapshot(node, role)) {
        const SnapshotRoleGroup group = classifySnapshotRole(role);
        out << "[ref=e" << refIndex++ << "] " << role << " \"" << escapeForQuote(label) << "\"";
        out << " [" << snapshotGroupText(group) << "]";
        if (node->isClickable()) {
            out << " [clickable]";
        }
        if (node->isFocusable()) {
            out << " [focusable]";
        }
        out << "\n";
    }

    for (const auto &child : node->getChildren()) {
        appendSnapshotLines(child, refIndex, out);
    }
}

int runSnapshot(const std::shared_ptr<Aurum::UiDevice> &device, const Options &opts)
{
    auto roots = device->getWindowRoot();
    std::ostringstream out;
    int refIndex = 1;
    for (const auto &root : roots) {
        appendSnapshotLines(root, refIndex, out);
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

int main(int argc, char **argv)
{
    if (argc < 2) {
        printUsage();
        return EXIT_INVALID_ARGS;
    }

    const std::string command = argv[1];
    if (command == "help" || command == "--help" || command == "-h") {
        printUsage();
        return EXIT_OK;
    }

    std::vector<std::string> rawArgs;
    rawArgs.reserve(static_cast<size_t>(argc > 2 ? argc - 2 : 0));
    for (int i = 2; i < argc; ++i)
        rawArgs.emplace_back(argv[i]);

    Options opts;
    std::string parseError;
    if (!parseOptions(rawArgs, opts, parseError)) {
        std::cerr << "status=error code=INVALID_ARGS detail='" << parseError << "'\n";
        return EXIT_INVALID_ARGS;
    }

    auto device = Aurum::UiDevice::getInstance();

    try {
        if (command == "find")
            return runFind(device, opts);
        if (command == "click")
            return runClick(device, opts);
        if (command == "set-text")
            return runSetText(device, opts);
        if (command == "wait")
            return runWait(device, opts);
        if (command == "press-key")
            return runPressKey(device, opts);
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
