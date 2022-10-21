## Contents
1. [Introduce Aurum](#introduce-aurum)
2. [Command List](#command-list)
3. [Running on TV](#running-on-tv)
4. [Reference and Tip](#reference-and-tip)

## Introduce Aurum
- Aurum is a UI automation framework without UI Toolkit dependency.

  Provides Commands to interact with the device UI by simulation user actions and introspection of the screen content.

  It relies on the platform accessibility APIs to introspect the screen.

- User can use the IDL defined in Proto to create an automation app or script in a variety of languages without a language dependency.

![image](https://media.github.sec.samsung.net/user/39889/files/ff684061-8bee-4a3f-855e-48ac38a5089a)

Please refer [grpc](https://grpc.io/) and [proto buffers](https://developers.google.com/protocol-buffers) for more information


## Command List
<table>

  <tr>
    <td> Name </td> <td> Description </td> <td> Proto definition </td> <td> Python usage </td>
  </tr>


  <tr>
    <td valign="top"> killServer </td>
    <td valign="top"> Terminate the org.tizen.aurum-bootstrap process </td>
    <td valign="top"> rpc killServer(ReqEmpty) returns (RspEmpty) {} </td>
    <td valign="top">

  ```python
stub.killServer(ReqEmpty())
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> findElements </td>
    <td valign="top"> If an object that satisfies a specific condition is searched on the screen and found, the object list is returned </td>
    <td valign="top"> rpc findElements(ReqFindElements) returns (RspFindElements) {}<br><br>ReqFindElements :<br>elementId<br>automationid<br>textfield<br>widgetType<br>widgetstyle<br> ischecked<br>ischeckable<br>iscliackable<br>isenabled<br>isfocused<br>isfocusable<br>isscrollable<br>isselected<br>isshowing<br>isactive<br>mindepth<br>maxdepth<br>packagename<br>textpartialmatch<br>xpath<br>geometry</td>
    <td valign="top">

  ```python
#Find Clickable object.
def findClickableObject(stub):
    response = stub.findElements(ReqFindElements(isClickable=True))

    for el in response.elements:
        return True

    return False

#Find Object which has Button as text
def findButtonTextObject(stub):
    response = stub.findElements(ReqFindElements(textField='Button'))

    for el in response.elements:
        response = stub.getValue(ReqGetValue(elementId=el.elementId))
        return response.stringValue == 'Button'

    return False

#Find entry type object
def findEntryObject(stub):
    response = stub.findElements(ReqFindElements(widgetType='Elm_Entry'))

    if len(response.elements) <= 0: return False

    return True

#Find object with variouse condition
def findTextWithShowingObject(stub):
    response = stub.findElements(ReqFindElements(textField='Bg', isShowing=True))

    if len(response.elements) <= 0: return False

    return True
  ```
</td>
  </tr>

  <tr>
    <td valign="top"> findElement </td>
    <td valign="top"> If an object that satisfies a specific condition is searched on the screen and found, the object is returned </td>
    <td valign="top"> rpc findElement(ReqFindElement) returns (RspFindElement) {}<br><br>ReqFindElement :<br>elementId<br>automationid<br>textfield<br>widgetType<br>widgetstyle<br> ischecked<br>ischeckable<br>iscliackable<br>isenabled<br>isfocused<br>isfocusable<br>isscrollable<br>isselected<br>isshowing<br>isactive<br>mindepth<br>maxdepth<br>packagename<br>textpartialmatch<br>xpath<br>geometry</td>
    <td valign="top">

  ```python
def findTextWithShowingObject(stub):
    response = stub.findElements(ReqFindElements(textField='Bg', isShowing=True))

    if response.element is None:
        return False

    return True
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> getValue </td>
    <td valign="top"> Get the value of the object </td>
    <td valign="top"> rpc getValue(ReqGetValue) returns (RspGetValue) {}<br><br>RspGetValue :<br>stringValue<br>intValue<br>doubleValue<br>boolValue </td>
    <td valign="top">

  ```python
 def findButtonTextObject(stub):
    response = stub.findElements(ReqFindElements(textField='Button'))

    for el in response.elements:
        response = stub.getValue(ReqGetValue(elementId=el.elementId))
        return response.stringValue == 'Button'

    return False
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> setValue </td>
    <td valign="top"> Set the value of the object </td>
    <td valign="top"> rpc setValue(ReqSetValue) returns (RspSetValue) {}<br><br>RspGetValue :<br>stringValue<br>intValue<br>doubleValue<br>boolValue </td>
    <td valign="top">

  ```python
 def setValueTest(stub):
    response = stub.findElements(ReqFindElements(widgetType='Elm_Entry'))

    if len(response.elements) <= 0: return False

    targetObj = response.elements[0].elementId
    testString = 'set test string by calling SetValue Method'

    stub.setValue(ReqSetValue(elementId=targetObj, stringValue=testString))
    response = stub.getValue(ReqGetValue(elementId=targetObj))

    if response.stringValue != testString:
        return False

    return True
  ```
</td>
  </tr>

  <tr>
    <td valign="top"> getSize </td>
    <td valign="top"> Get geometry of the object </td>
    <td valign="top"> rpc getSize(ReqGetSize) returns (RspGetSize) {} </td>
    <td valign="top">

  ```python
 def getSizeTest(stub):
    response = stub.findElements(ReqFindElements(textField='Button'))
    print("els", response)

    for el in response.elements:
        response = stub.getSize(ReqGetSize(elementId=el.elementId))
        print(response)

        return response.size.width + response.size.height > 0

    return False
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> clear </td>
    <td valign="top"> Clear value of the object </td>
    <td valign="top"> rpc clear(ReqClear) returns (RspClear) {} </td>
    <td valign="top">

  ```python
 def setValueAndClearTest(stub):
    response = stub.findElements(ReqFindElements(widgetType='Elm_Entry'))

    if len(response.elements) <= 0: return False

    targetObj = response.elements[0].elementId
    testString = 'set test string by calling SetValue Method'

    stub.setValue(ReqSetValue(elementId=targetObj, stringValue=testString))
    response = stub.getValue(ReqGetValue(elementId=targetObj))

    if response.stringValue != testString:
        return False

    stub.clear(ReqClear(elementId=targetObj))
    response = stub.getValue(ReqGetValue(elementId=targetObj))
    if response.stringValue != '':
        return False

    return True
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> getAttribute </td>
    <td valign="top"> Get attribute of the object </td>
    <td valign="top"> rpc getAttribute(ReqGetAttribute) returns (RspGetAttribute) {}<br><br>ReqGetAttribute :<br>VISIBLE<br>FOCUSABLE<br>FOCUSED<br>ENABLED<br>CLICKABLE<br>SCROLLABLE<br> CHECKABLE<br>CHECKED<br>SELECTED<br>SELECTABLE<br>SHOWING<br>ACTIVE<br><br>RspGetAttribute :<br>stringValue<br>intValue<br>doubleValue<br>boolValue </td>
    <td valign="top">

  ```python
 def getAttributeTest(stub):
    response = stub.findElements(ReqFindElements(textField='Button'))
    if len(response.elements) <= 0: return False

    checkList = [
            ['VISIBLE', True],
            ['FOCUSABLE', True],
            ['FOCUSED', False],
            ['ENABLED', True],
            ['CLICKABLE', True],
            ['SCROLLABLE', False],
            ['CHECKABLE', False],
            ['CHECKED', False],
            ['SELECTED', False],
            ['SELECTABLE',True],
            ['SHOWING', True],
    ]
    isFailed = False

    for el in response.elements:
        for attr in checkList:
            if stub.getAttribute(ReqGetAttribute(elementId=el.elementId, attribute=attr[0])).boolValue != attr[1]:
               isFailed = True

    if isFailed == True: return False

    retrurn True
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> click </td>
    <td valign="top"> Generate click event </td>
    <td valign="top"> rpc click(ReqClick) returns (RspClick) {}<br><br>ReqClick :<br>elementId<br>coordination<br>Type :<br>ELEMENTID<br>COORD<br>ATSPI </td>
    <td valign="top">

  ```python
 def clickTest(stub):
    response = stub.findElements(ReqFindElements(textField='Accessibility'))

    if len(response.elements) <= 0: return False

    for el in response.elements:
        stub.click(ReqClick(elementId=el.elementId, type='ELEMENTID'))

    response = stub.findElements(ReqFindElements(textField='Screen Reader'))
    if len(response.elements) <= 0: return False

    for el in response.elements:
        stub.click(ReqClick(coordination=Point(x=320, y=130), type='COORD'))

    return True
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> longClick </td>
    <td valign="top"> Generate longclick (50ms) event </td>
    <td valign="top"> rpc longClick(ReqClick) returns (RspClick) {} </td>
    <td valign="top">

  ```python

 def longClickTest(stub):
    stub.longClick(ReqClick(coordination=Point(x=160, y=160), type='COORD'))

    return False
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> flick </td>
    <td valign="top"> Generate flick event </td>
    <td valign="top"> rpc flick(ReqFlick) returns (RspFlick) {} </td>
    <td valign="top">

  ```python

 def flickTest(stub):
    for tryCnt in range(10):
        print('Flick to bottom to find "Spinner" item @ tries:', tryCnt)
        stub.flick(ReqFlick(startPoint=Point(x=160, y=359), endPoint=Point(x=160, y=1), durationMs=110))
        response = stub.findElements(ReqFindElements(textField='Spinner', isShowing=True)

        for el in response.elements:
            return True

    return False
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> touchDown </td>
    <td valign="top"> Generate touchDown event </td>
    <td valign="top"> rpc touchDown(ReqTouchDown) returns (RspTouchDown) {} </td>
    <td valign="top">

  ```python
 def touchTest(stub):
    res = stub.touchDown(ReqTouchDown(coordination=Point(x=160,y=640)))
    print(res)

    seq = res.seqId
    if seq < 0: return False

    for yy in range(640, 30, -10):
        stub.touchMove(ReqTouchMove(coordination=Point(x=160,y=yy), seqId=seq))

    stub.touchUp(ReqTouchUp(coordination=Point(x=160,y=30), seqId=seq))

    return True
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> touchMove </td>
    <td valign="top"> Generate touchMove event </td>
    <td valign="top"> rpc touchMove(ReqTouchMove) returns (RspTouchMove) {} </td>
    <td valign="top">

  ```python
#See touchDown
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> touchUp </td>
    <td valign="top"> Generate touchUp event </td>
    <td valign="top"> rpc touchUp(ReqTouchUp) returns (RspTouchUp) {} </td>
    <td valign="top">

  ```python
#See touchDown
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> installApp </td>
    <td valign="top"> Install application </td>
    <td valign="top"> rpc installApp(stream ReqInstallApp) returns (RspInstallApp) {} </td>
    <td valign="top">

  ```python
 def installAppTest(stub):
    response = stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents'))
    if (response.isInstalled): return True

    tpkFile = './org.tizen.uicomponents.arm.tpk'
    binaryChunk = get_file_chunks(tpkFile)
    response = stub.installApp(binaryChunk)

    for waitCnt in range(10):
        response = stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents'))
        print('tries:', waitCnt, 'isInstalled:', response.isInstalled)
        time.sleep(1)

        if response.isInstalled: return True

    return False
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> removeApp </td>
    <td valign="top"> Remove application </td>
    <td valign="top"> rpc removeApp(ReqRemoveApp) returns (RspRemoveApp) {} </td>
    <td valign="top">

  ```python

 def removeAppTest(stub):
    response = stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents'))
    if (response.isInstalled): response = stub.removeApp(ReqRemoveApp(packageName='org.example.uicomponents'))

    for waitCnt in range(10):
        response = stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents'))
        print('tries:', waitCnt, 'isInstalled:', response.isInstalled)
        time.sleep(1)

        if response.isInstalled != True: return True

    return False
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> getAppInfo </td>
    <td valign="top"> Get application info </td>
    <td valign="top"> rpc getAppInfo(ReqGetAppInfo) returns (RspGetAppInfo) {}<br><br>ReqGetAppInfo :<br>isRunning<br>isInstalled<br>isFocused </td>
    <td valign="top">

  ```python
 def getAppInfoTest(stub):
    return stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents')).isRunning
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> launchApp </td>
    <td valign="top"> Launch application </td>
    <td valign="top"> rpc launchApp(ReqLaunchApp) returns (RspLaunchApp) {} </td>
    <td valign="top">

  ```python
def launchAppTest(stub):
    return stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents')).isRunning
  ```
</td>
  </tr>  


  <tr>
    <td valign="top"> closeApp </td>
    <td valign="top"> Close application </td>
    <td valign="top"> rpc closeApp(ReqCloseApp) returns (RspCloseApp) {} </td>
    <td valign="top">

  ```python
#Add code
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> getDeviceTime </td>
    <td valign="top"> Get device time </td>
    <td valign="top"> rpc getDeviceTime(ReqGetDeviceTime) returns (RspGetDeviceTime) {}<br><br>ReqGetDeviceTime :<br>SYSTEM<br>WALLCLOCK </td>
    <td valign="top">

  ```python
 def getDeviceTimeTest(stub):
    response1 = stub.getDeviceTime(ReqGetDeviceTime(type='SYSTEM'))
    response2 = stub.getDeviceTime(ReqGetDeviceTime(type='WALLCLOCK'))
    print(response1, response2)

    return response2.timestampUTC > response1.timestampUTC;
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> sendKey </td>
    <td valign="top"> Generate key event </td>
    <td valign="top"> rpc sendKey(ReqKey) returns (RspKey) {}<br><br>ReqKey :<br>BACK<br>MENU<br>HOME<br>VOLUP<br>VOLDOWN<br>POWER<br>XF86<br>WHEELUP<br>WHEELDOWN<br> actionType :<br>STROKE<br>LONG_STROKE<br>PRESS<br>RELEASE<br>REPEAT<br><br>XF86KeyCode<br>durationMs<br>intervalMs</td>
    <td valign="top">

  ```python
 def sendKeyTest(stub):
    response = stub.sendKey(ReqKey(type='WHEELUP', actionType='STROKE'))
    time.sleep(1)

    response = stub.sendKey(ReqKey(type='WHEELUP', actionType='STROKE'))
    time.sleep(1)

    response = stub.sendKey(ReqKey(type='WHEELDOWN', actionType='STROKE'))
    time.sleep(1)

    response = stub.sendKey(ReqKey(type='POWER', actionType='STROKE'))
    time.sleep(1)

    response = stub.sendKey(ReqKey(type='POWER', actionType='STROKE'))
    time.sleep(1)

    response = stub.sendKey(ReqKey(type='BACK', actionType='STROKE'))
    time.sleep(1)

    return True
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> takeScreenshot </td>
    <td valign="top"> Take screenshot </td>
    <td valign="top"> rpc takeScreenshot(ReqTakeScreenshot) returns (stream RspTakeScreenshot) {} </td>
    <td valign="top">

  ```python
 def takeScreenshotTest(stub):
    responses = stub.takeScreenshot(ReqTakeScreenshot())
    image = open("screenshot.png", "wb")

    for response in responses:
        image.write(response.image)
    image.close()

    return True;
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> dumpObjectTree </td>
    <td valign="top"> Get object infomation </td>
    <td valign="top"> rpc dumpObjectTree(ReqDumpObjectTree) returns (RspDumpObjectTree) {} </td>
    <td valign="top">

  ```python
 def dumpObject(stub):
    response = stub.findElements(ReqFindElements(textField='Bg', isShowing=True))
    if len(response.elements) <= 0: return False

    for ei in response.elements:
        response = stub.dumpObjectTree(ReqDumpObjectTree(elementId=ei.elementId))

    print(response.roots)
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> setFocus </td>
    <td valign="top"> Set focus to specific UI Object </td>
    <td valign="top"> rpc setFocus(ReqSetFocus) returns (RspSetFocus) {} </td>
    <td valign="top">

  ```python

def setFocus(stub):
    response = stub.findElements(ReqFindElements(textField='Button'))
    if len(response.elements) <= 0: return False

    stub.setFocus(ReqSetFocus(elementId=response.elements[0).elementId)
  ```
</td>
  </tr>


  <tr>
    <td valign="top"> actionAndWaitEvent </td>
    <td valign="top"> Do action and wait event </td>
    <td valign="top"> rpc actionAndWaitEvent(ReqActionAndWaitEvent) returns (RspActionAndWaitEvent) {}<br><br>ActionType :<br>CLICK<br>KEY<br>EventType<br>  EVENT_WINDOW_ACTIVATE<br>EVENT_WINDOW_DEACTIVATE<br>EVENT_STATE_CHANGED_FOCUSED</td>
    <td valign="top">

  ```python
def actionAndWaitEvent(stub):
    stub.actionAndWaitEvent(ReqActionAndWaitEvent(type=KEY, XF86keyCode='Return',
                                             eventType=EVENT_STATE_CHANGE_FOCUSED, timeoutMs=1))

    response = stub.findElements(ReqFindElements(textField='Button'))
    if len(response.elements) <= 0: return False

    stub.actionAndWaitEvent(ReqActionAndWaitEvent(type=CLICK, elementId=response.elements[0).elementId,
                                             eventType=EVENT_WINDOW_ACTIVATE, timeoutMs=5))
  ```
</td>
  </tr>

</table>

### Running on TV
- Known issue

  grpc not support environment that uses proxy on host PC

- Pre-condition

  The TV device and Host PC should be already connected through SDB

- Gets Aurum latest version and checkout to tizen branch

  [Aurum Github](https://github.sec.samsung.net/tizen/aurum)

- Set up a python virtual environment and run UI Automation (working directory: aurum/)

  (host) cd ui_automation/python/tv

  #### Create virtual env
  (host) python3 -m venv v

  #### Activate a virtual env
  Linux
  (host) source v/bin/activate
  Window
  (host) v/Scripts/activate.bat

  #### Install required pkg (only once)
  (python_virtual) pip3 install -r ../../../protocol/resources/python/requirements.txt

  #### Generate aurum.proto file for python (only once)
  (python_virtual) python3 -m grpc_tools.protoc --python_out=./ --grpc_python_out=./ -I ./../../../protocol/ ../../../protocol/aurum.proto

  #### Target setup such as sdb forward, bootstrap execution
  (python_virtual) python3 ../../../protocol/resources/python/tv/tvSetup.py

  #### Then add your script then run it (Please refer sample scripts in 'aurum/protocol/resources/python/tv')
  (python_virtual) python3 sampleWithUtils.py

  #### Deactivate a virtual env
  (python_virtual) deactivate

### Reference and Tip
- [How to run aurum on TM1](https://code.sec.samsung.net/confluence/display/GFX/04.+NUITizenGallery+Test+Script+Guide)
- [NUI automation test](https://github.com/nui-dali/NUIAutomationTest)
- [Aurum test sciprts(samples)](https://github.sec.samsung.net/NUI/AurumTestScript)
- [Automation application(C++) in target](https://github.sec.samsung.net/TizenUIFW/aurum-test-sample)
   * Aurum API is not public. we dont recommand direct calling aurum API in target.
