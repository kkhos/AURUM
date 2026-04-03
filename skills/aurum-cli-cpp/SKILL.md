---
name: aurum-cli-cpp
description: Build or modify a C++ command-line interface (CLI) that automates Tizen UI interactions using libaurum. Use this when the user asks to create an Aurum CLI tool, add CLI commands (find/click/set-text/screenshot/key input/wait), wire argument parsing to libaurum APIs, or generate a reusable C++ automation executable based on libaurum.
---

# Build an Aurum CLI in C++ (libaurum)

Use this skill when you need a native C++ executable that exposes Aurum automation operations as shell commands.

## Core Workflow

1. **Define command surface**: decide subcommands and arguments (`find`, `click`, `set-text`, `press-key`, `screenshot`, `snapshot`, `wait`).
2. **Map command → libaurum API**: use `UiDevice`, `UiObject`, `UiSelector`, `Sel`, and `Until`.
3. **Implement parser + dispatcher** in one binary (single `main.cpp` is preferred first).
4. **Return stable exit codes**: `0` success, non-zero on invalid args/not found/action failure.
5. **Print machine-friendly output**: compact key-value or JSON-like lines for automation.
6. **Verify with local build** (`meson`/`ninja`) and sanity-run at least `--help`.

## Recommended Command Design

Use subcommands instead of many flags:

- `aurum-cli find --text <value> [--timeout-ms 3000]`
- `aurum-cli click --text <value>`
- `aurum-cli set-text --text <target> --value <new-value>`
- `aurum-cli press-key --key <KEYCODE>`
- `aurum-cli screenshot --path <file>`
- `aurum-cli snapshot [--path <file>]` (agent-browser style refs)
- `aurum-cli wait --text <value> [--timeout-ms 5000]`

Guidelines:

- Always support at least one selector option (`--text` minimum).
- Prefer explicit names (`--timeout-ms`, `--duration-ms`) over short ambiguous flags.
- Never silently ignore missing required options.

## API Mapping (quick)

- Device singleton: `auto device = Aurum::UiDevice::getInstance();`
- Find one: `device->findObject(Aurum::Sel::text("..."))`
- Find many: `device->findObjects(selector)`
- Wait for existence: `device->waitFor(Aurum::Until::hasObject(selector))`
- Click object: `obj->click()`
- Set text: `obj->setText("...")`
- Press key: `device->pressKeyCode(key, Aurum::KeyRequestType::STROKE)`
- Screenshot: `device->takeScreenshot(path, false, nullptr)`
- Snapshot: traverse `device->getWindowRoot()` and emit filtered lines like `[ref=e1] button "OK"` for interactive/important roles.
- Screen size: `device->getScreenSize()`

For broader selector/action coverage, read [references/api-map.md](references/api-map.md).

## Minimal Implementation Pattern

```cpp
#include <iostream>
#include <string>
#include "Aurum.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: aurum-cli <command> [options]\n";
        return 2;
    }

    auto device = Aurum::UiDevice::getInstance();
    std::string cmd = argv[1];

    if (cmd == "press-key") {
        if (argc < 4 || std::string(argv[2]) != "--key") return 2;
        bool ok = device->pressKeyCode(argv[3], Aurum::KeyRequestType::STROKE);
        return ok ? 0 : 1;
    }

    if (cmd == "find") {
        if (argc < 4 || std::string(argv[2]) != "--text") return 2;
        auto obj = device->findObject(Aurum::Sel::text(argv[3]));
        if (!obj) return 1;
        std::cout << "found text=" << obj->getText() << "\n";
        return 0;
    }

    std::cerr << "unknown command\n";
    return 2;
}
```

## Build Notes

- This repository builds with Meson (`cpp_std=gnu++17`).
- `libaurum` exposes a pkg-config entry named `libaurum`.
- Prefer integrating the new CLI into existing Meson targets instead of ad-hoc compiler flags when editing this repo.

## Reliability Rules

- Re-query objects right before stateful actions if UI may have changed.
- Treat `nullptr` from `findObject` as normal not-found (not a crash case).
- Return deterministic stderr messages for failures (`NOT_FOUND`, `INVALID_ARGS`, `ACTION_FAILED`).
- Keep side effects explicit (no implicit click during `find`).

## When to Load More Context

Load only when needed:

- `references/api-map.md` for selector/action/wait combinations.
- `libaurum/inc/UiDevice.h` and `libaurum/inc/UiObject.h` when adding less-common APIs.
