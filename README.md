<img src="https://capsule-render.vercel.app/api?type=waving&color=1d9cd8&height=200&section=header&text=Aurum&fontSize=60" />

## Contents
1. [Introduce Aurum](#introduce-aurum)
2. [Reference and Tip](#reference-and-tip)

## Introduce Aurum
- Aurum is a UI automation framework without UI Toolkit dependency.

  Provides Commands to interact with the device UI by simulation user actions and introspection of the screen content.

  It relies on the platform accessibility APIs to introspect the screen.

- User can use the IDL defined in Proto to create an automation app or script in a variety of languages without a language dependency.

Please refer [grpc](https://grpc.io/) and [proto buffers](https://developers.google.com/protocol-buffers) for more information

**If you're new to Aurum or want a more comprehensive description of what this is all about, please read our
[Github Pages](https://pages.github.sec.samsung.net/tizen/aurum/)**

### Reference and Tip
- [Aurum python library](https://github.sec.samsung.net/tizen/aurum-python)
- [How to run aurum on TM1](https://code.sec.samsung.net/confluence/display/GFX/04.+NUITizenGallery+Test+Script+Guide)
- [NUI automation test](https://github.com/nui-dali/NUIAutomationTest)
- [Aurum test sciprts(samples)](https://github.sec.samsung.net/NUI/AurumTestScript)
- [Automation application(C++) in target](https://github.sec.samsung.net/TizenUIFW/aurum-test-sample)
- [UI Analyzer](https://github.sec.samsung.net/TizenNativeUI/ui-analyzer)
   * Aurum API is not public. we dont recommand direct calling aurum API in target.

### Local Build (Ubuntu)
```bash
meson setup builddir -Dlibaurum_only=true
ninja -C builddir/
```
