from __future__ import print_function
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

def findElementTest(stub):
    response = stub.findElement(ReqFindElement(isClickable=True))
    for el in response.elements:
        return True
    return False

def getValueTest(stub):
    response = stub.findElement(ReqFindElement(textField='Button'))
    print("els", response)
    for el in response.elements:
        response = stub.getValue(ReqGetValue(elementId=el.elementId))
        return response.stringValue == 'Button'
    return False

def setValueClearTest(stub):
    def inScreen(size):
        if size.x < 0: return False
        if size.y < 0: return False
        if size.x >= 720: return False
        if size.y >= 1280: return False
        return True

    for tryCnt in range(10):
        stub.flick(ReqFlick(startPoint=Point(x=160, y=350), endPoint=Point(x=160, y=10), durationMs=500))
        response = stub.findElement(ReqFindElement(textField='Entry'))
        if len(response.elements) <= 0: continue
        targetObj = response.elements[0].elementId
        response = stub.getSize(ReqGetSize(elementId=targetObj))
        if inScreen(response.size):
            stub.click(ReqClick(type='ELEMENTID', elementId=targetObj))
            break

    for tryCnt in range(10):
        stub.flick(ReqFlick(startPoint=Point(x=160, y=350), endPoint=Point(x=160, y=10), durationMs=500))
        response = stub.findElement(ReqFindElement(textField='Singleline Entry'))
        if len(response.elements) <= 0: continue
        targetObj = response.elements[0].elementId
        response = stub.getSize(ReqGetSize(elementId=targetObj))
        isShowing = stub.getAttribute(ReqGetAttribute(elementId=targetObj, attribute='SHOWING')).boolValue
        if inScreen(response.size) or isShowing:
            stub.click(ReqClick(type='ELEMENTID', elementId=targetObj))
            break

    response = stub.findElement(ReqFindElement(widgetType='Elm_Entry'))
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

def getSizeTest(stub):
    response = stub.findElement(ReqFindElement(textField='Button'))
    print("els", response)
    for el in response.elements:
        response = stub.getSize(ReqGetSize(elementId=el.elementId))
        print(response)
        return response.size.width + response.size.height > 0
    return False

def getAttributeTest(stub):
    response = stub.findElement(ReqFindElement(textField='Button'))
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

    response = stub.findElement(ReqFindElement(textField='Check'))
    if len(response.elements) <= 0: return False

    checkList = [
            ['VISIBLE',     True],
            ['FOCUSABLE',   True],
            ['FOCUSED',     False],
            ['ENABLED',     True],
            ['CLICKABLE',   True],
            ['SCROLLABLE',  False],
            ['CHECKABLE',   False],
            ['CHECKED',     False],
            ['SELECTED',    False],
            ['SELECTABLE',  True],
            ['SHOWING',     True],
    ]
    isFailed = False
    for el in response.elements:
        for attr in checkList:
            if stub.getAttribute(ReqGetAttribute(elementId=el.elementId, attribute=attr[0])).boolValue != attr[1]:
              isFailed = True

    return isFailed == False

def clickTest(stub):
    response = stub.findElement(ReqFindElement(textField='Accessibility'))
    if len(response.elements) <= 0: return False

    for el in response.elements:
        stub.click(ReqClick(elementId=el.elementId, type='ELEMENTID'))

    response = stub.findElement(ReqFindElement(textField='Screen Reader'))
    if len(response.elements) <= 0: return False

    for el in response.elements:
        stub.click(ReqClick(coordination=Point(x=320, y=130), type='COORD'))

    response = stub.findElement(ReqFindElement(textField='UI Descriptions'))
    if len(response.elements) <= 0: return False

    return True

def longClickTest(stub):
    response = stub.sendKey(ReqKey(type='HOME', actionType='STROKE'))
    stub.click(ReqClick(coordination=Point(x=160, y=160), type='COORD'))
    # TODO : find out something changed
    return False

def flickTest(stub):
    response = stub.findElement(ReqFindElement(textField='Bg', isShowing=True))
    if len(response.elements) <= 0: return False
    targetObj = response.elements[0].elementId

    for tryCnt in range(10):
        print('Flick to bottom to find "Spinner" item @ tries:', tryCnt)
        stub.flick(ReqFlick(startPoint=Point(x=160, y=359), endPoint=Point(x=160, y=1), durationMs=110))
        response = stub.findElement(ReqFindElement(textField='Spinner'))
        time.sleep(0.01)
        print(response)
        if len(response.elements) > 0:
            for el in response.elements:
                if stub.getAttribute(ReqGetAttribute(elementId=el.elementId, attribute='SHOWING')).boolValue: return True

    return False

def touchTest(stub):
    res = stub.touchDown(ReqTouchDown(coordination=Point(x=160,y=640)))
    print(res)
    seq = res.seqId
    if seq < 0: return False
    for yy in range(640, 30, -10):
        stub.touchMove(ReqTouchMove(coordination=Point(x=160,y=yy), seqId=seq))
    stub.touchUp(ReqTouchUp(coordination=Point(x=160,y=30), seqId=seq))

    return True


def get_file_chunks(filename):
   CHUNK_SIZE = 1024 * 1024
   with open(filename, 'rb') as f:
       while True:
           piece = f.read(CHUNK_SIZE)
           if len(piece) == 0:
               return
           yield ReqInstallApp(package=piece)


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

def removeAppTest(stub):
    response = stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents'))
    if (response.isInstalled): response = stub.removeApp(ReqRemoveApp(packageName='org.example.uicomponents'))
    for waitCnt in range(10):
        response = stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents'))
        print('tries:', waitCnt, 'isInstalled:', response.isInstalled)
        time.sleep(1)
        if response.isInstalled != True: return True
    return False

def getAppInfoTest(stub):
    return stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents')).isRunning

def launchAppTest(stub):
    print('launch result', stub.launchApp(ReqLaunchApp(packageName='org.example.uicomponents')))

    return stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents')).isRunning

def closeAppTest(stub):
    print('close result',stub.closeApp(ReqCloseApp(packageName='org.example.uicomponents')))
    return stub.getAppInfo(ReqGetAppInfo(packageName='org.example.uicomponents')).isRunning != True

def sendKeyTest(stub):
    response = stub.sendKey(ReqKey(type='WHEELUP', actionType='STROKE'))
    time.sleep(0.3)
    response = stub.sendKey(ReqKey(type='WHEELUP', actionType='STROKE'))
    time.sleep(0.3)
    response = stub.sendKey(ReqKey(type='WHEELDOWN', actionType='STROKE'))
    time.sleep(0.3)
    response = stub.sendKey(ReqKey(type='POWER', actionType='STROKE'))
    time.sleep(1)
    response = stub.sendKey(ReqKey(type='POWER', actionType='STROKE'))
    time.sleep(1)
    response = stub.sendKey(ReqKey(type='BACK', actionType='STROKE'))
    time.sleep(1)
    response = stub.sendKey(ReqKey(type='MENU', actionType='STROKE'))
    time.sleep(1)
    return True

def getDeviceTimeTest(stub):
    response1 = stub.getDeviceTime(ReqGetDeviceTime(type='WALLCLOCK'))
    response2 = stub.getDeviceTime(ReqGetDeviceTime(type='WALLCLOCK'))
    print(response1, response2)
    return response2.timestampUTC > response1.timestampUTC;

def getLocationTest(stub):
    response = stub.getLocation(ReqGetLocation())

    if response.alt < 0: return False
    if response.lat < 0: return False
    return True

def takeScreenshotTest(stub):
    responses = stub.takeScreenshot(ReqTakeScreenshot())
    image = open("screenshot.png", "wb")
    for response in responses:
        image.write(response.image)
    image.close()
    return True;

def defaultSetup(stub):
    if stub.getAppInfo(ReqGetAppInfo(packageName='org.tizen.elm-demo-tizen-mobile')).isRunning:
        stub.closeApp(ReqCloseApp(packageName='org.tizen.elm-demo-tizen-mobile'))

    stub.launchApp(ReqLaunchApp(packageName='org.tizen.elm-demo-tizen-mobile'))

def defaultTearDown(stub):
    stub.closeApp(ReqCloseApp(packageName='org.tizen.elm-demo-tizen-mobile'))

def runTest(stub, testFunc, setup=defaultSetup, tearDown=defaultTearDown, alwaySucceed=False):
    print("Testing started :", testFunc)

    setup(stub)
    result = testFunc(stub)
    tearDown(stub)

    print("Testing result :", result)
    if alwaySucceed: return True
    assert True == result

def runTestWithoutSetupAndTearDown(stub, testFunc, setup=defaultSetup, tearDown=defaultTearDown):
    def Empty(stub):
        pass

    runTest(stub, testFunc, Empty, Empty)


def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)

        runTest(stub, getDeviceTimeTest)
        runTest(stub, findElementTest)
        runTest(stub, getValueTest)
        runTest(stub, getSizeTest)
        runTest(stub, getAttributeTest)
        runTest(stub, clickTest)
        runTest(stub, flickTest)
        runTest(stub, touchTest)
        runTest(stub, sendKeyTest)
        runTest(stub, setValueClearTest)
        runTest(stub, takeScreenshotTest)
        runTest(stub, longClickTest, alwaySucceed=True)
        runTest(stub, getLocationTest, alwaySucceed=True)

        runTestWithoutSetupAndTearDown(stub, installAppTest)
        runTestWithoutSetupAndTearDown(stub, launchAppTest)
        runTestWithoutSetupAndTearDown(stub, getAppInfoTest)
        runTestWithoutSetupAndTearDown(stub, closeAppTest)
        runTestWithoutSetupAndTearDown(stub, removeAppTest)

if __name__ == '__main__':
    logging.basicConfig()
    run()
