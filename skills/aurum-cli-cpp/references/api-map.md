# libaurum API map for C++ CLI

Use this file when you need to extend command coverage beyond the minimal set in `SKILL.md`.

## Selector creation

### Fast helpers (`Sel`)

- `Sel::text("Play")`
- `Sel::type("Button")`
- `Sel::style("style-name")`
- `Sel::depth(3)`

### Rich selectors (`UiSelector`)

```cpp
auto sel = std::make_shared<Aurum::UiSelector>();
sel->text("Settings")->isClickable(true)->isEnabled(true);
```

Common filters:

- identity/text: `id`, `automationid`, `text`, `ocrText`, `textPartialMatch`, `pkg`
- structure: `depth`, `minDepth`, `maxDepth`, `childSelector`, `fromParent`
- role/type/style: `role`, `type`, `style`
- state: `isChecked`, `isCheckable`, `isClickable`, `isEnabled`, `isFocused`, `isFocusable`, `isScrollable`, `isSelected`, `isShowing`, `isActive`, `isVisible`

## Search and wait

- boolean check: `device->hasObject(selector)`
- one object: `device->findObject(selector)`
- many objects: `device->findObjects(selector)`
- wait until present: `device->waitFor(Aurum::Until::hasObject(selector))`
- wait and return object: `device->waitFor(Aurum::Until::findObject(selector))`

## UiObject getters (useful for CLI output)

- identity: `getId`, `getText`, `getAutomationId`, `getDescription`, `getRole`, `getType`
- geometry: `getWindowBoundingBox`, `getScreenBoundingBox`, `getTextMinBoundingRect`
- state: `isEnabled`, `isFocused`, `isClickable`, `isVisible`, `isChecked`, `isSelected`
- hierarchy: `getParent`, `getChildren`, `getChildAt`, `getChildCount`

## UiObject actions

- `click()`
- `longClick(durationMs)`
- `setText(value)`
- `setValue(double)`
- `setFocus()`
- `moveTo()`
- `doAction("action-name")`

## UiDevice actions

- coordinate/touch/mouse: `click`, `drag`, `touchDown`, `touchMove`, `touchUp`, `mouseDown`, `mouseMove`, `mouseUp`
- keys: `pressBack`, `pressHome`, `pressMenu`, `pressPower`, `pressKeyCode`, `repeatKeyCode`
- device info: `getScreenSize`, `getSystemTime`, `getWindowAngle`, `getTargetAngle`
- screenshot: `takeScreenshot(path, false, nullptr)`
- tree snapshot (agent-browser style refs): `getWindowRoot()` + DFS over `getChildren()` and emit `[ref=eN] role "label"` for clickable/focusable/important-role nodes.

## CLI architecture tips

- Keep parsing separate from execution (`parse_*` + `run_*` functions).
- Make selectors reusable via a helper:

```cpp
std::shared_ptr<Aurum::UiSelector> selectorFromArgs(const Args& args);
```

- Emit parseable output:

```text
status=ok command=find text="Settings"
status=error code=NOT_FOUND command=click selector=text:Settings
```

- Normalize failures:
  - `2` = invalid usage
  - `3` = selector not found
  - `4` = action failed
