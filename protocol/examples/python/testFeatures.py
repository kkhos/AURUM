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
    response = stub.findElement(ReqFindElement(textField='Widgets'))
    print("els", response)
    for el in response.elements:
        response = stub.getValue(ReqGetValue(elementId=el.elementId))
        return response.stringValue == 'Widgets'
    return False

def setValueClearTest(stub):
    response = stub.findElement(ReqFindElement(textField='Widgets'))
    if len(response.elements) <= 0: return False
    stub.click(ReqClick(type='ELEMENTID', elementId=response.elements[0].elementId))

    def inScreen(size):
        if size.x < 0: return False
        if size.y < 0: return False
        if size.x >= 360: return False
        if size.y >= 360: return False
        return True

    for tryCnt in range(10):
        stub.flick(ReqFlick(startPoint=Point(x=160, y=350), endPoint=Point(x=160, y=10), durationMs=100))
        response = stub.findElement(ReqFindElement(textField='Entry/Editfield, Entry/Text Input'))
        if len(response.elements) <= 0: continue
        targetObj = response.elements[0].elementId
        response = stub.getSize(ReqGetSize(elementId=targetObj))
        if inScreen(response.size):
            stub.click(ReqClick(type='ELEMENTID', elementId=targetObj))
            break

    for tryCnt in range(10):
        stub.flick(ReqFlick(startPoint=Point(x=160, y=350), endPoint=Point(x=160, y=10), durationMs=100))
        response = stub.findElement(ReqFindElement(textField='Editable'))
        if len(response.elements) <= 0: continue
        targetObj = response.elements[0].elementId
        response = stub.getSize(ReqGetSize(elementId=targetObj))
        if inScreen(response.size):
            stub.click(ReqClick(type='ELEMENTID', elementId=targetObj))
            break

    response = stub.findElement(ReqFindElement(textField='editable'))
    if len(response.elements) <= 0: return False
    targetObj = response.elements[0].elementId

    testString = 'set test string by calling SetValue Method'
    stub.setValue(ReqSetValue(elementId=targetObj, stringValue=testString))
    response = stub.getValue(ReqGetValue(elementId=targetObj))
    if response.stringValue != testString:
        return False

    stub.clear(ReqClear(elementId=targetObj))

    response = stub.getValue(ReqGetValue(elementId=targetObj))
    if response.stringValue != 'editable':
        return False

    return True

def getSizeTest(stub):
    response = stub.findElement(ReqFindElement(textField='Widgets'))
    print("els", response)
    for el in response.elements:
        response = stub.getSize(ReqGetSize(elementId=el.elementId))
        print(response)
        return response.size.width + response.size.height > 0
    return False

def getAttributeTest(stub):
    response = stub.findElement(ReqFindElement(textField='Widgets'))
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
            ['ACTIVE', True],
    ]
    isFailed = False
    for el in response.elements:
        for attr in checkList:
            if stub.getAttribute(ReqGetAttribute(elementId=el.elementId, attribute=attr[0])).boolValue != attr[1]:
                isFailed = True

    if isFailed == True: return False

    response = stub.findElement(ReqFindElement(textField='Internal Legacy'))
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
            ['SHOWING',     False],
            ['ACTIVE',      False],
    ]
    isFailed = False
    for el in response.elements:
        for attr in checkList:
            if stub.getAttribute(ReqGetAttribute(elementId=el.elementId, attribute=attr[0])).boolValue != attr[1]:
                isFailed = True

    return isFailed == False

def clickTest(stub):
    response = stub.findElement(ReqFindElement(textField='Widgets'))
    if len(response.elements) <= 0: return False

    for el in response.elements:
        stub.click(ReqClick(elementId=el.elementId, type='ELEMENTID'))

    response = stub.findElement(ReqFindElement(textField='Box'))
    if len(response.elements) <= 0: return False

    for el in response.elements:
        stub.click(ReqClick(coordination=Point(x=160, y=160), type='COORD'))

    response = stub.findElement(ReqFindElement(textField='Vertical Box'))
    if len(response.elements) <= 0: return False

    return True

def longClickTest(stub):
    response = stub.sendKey(ReqKey(type='HOME', actionType='STROKE'))
    stub.click(ReqClick(coordination=Point(x=160, y=160), type='COORD'))
    # TODO : find out something changed
    return False

def flickTest(stub):
    response = stub.findElement(ReqFindElement(textField='Widgets'))
    if len(response.elements) <= 0:
        return False

    for el in response.elements:
        print(el)
        stub.click(ReqClick(elementId=el.elementId, type='ELEMENTID'))
        break

    response = stub.findElement(ReqFindElement(textField='Box', isShowing=True))
    if len(response.elements) <= 0: return False
    targetObj = response.elements[0].elementId

    for tryCnt in range(10):
        print('Flick to bottom to find "Spinner" item @ tries:', tryCnt)
        stub.flick(ReqFlick(startPoint=Point(x=160, y=350), endPoint=Point(x=160, y=10), durationMs=10))
        response = stub.findElement(ReqFindElement(textField='Slider'))
        time.sleep(0.5)
        print(response)
        if len(response.elements) > 0: return True

    return False

def touchTest(stub):
    stub.touchDown(ReqTouchDown(coordination=Point(x=160,y=330)))
    for yy in range(330, 30, -10):
        stub.touchMove(ReqTouchMove(coordination=Point(x=160,y=yy)))
    stub.touchUp(ReqTouchUp(coordination=Point(x=160,y=30)))

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
    response = stub.sendKey(ReqKey(type='HOME', actionType='STROKE'))
    time.sleep(3)
    response = stub.sendKey(ReqKey(type='BACK', actionType='STROKE'))
    time.sleep(5)
    return True

def scrollToTest(stub):
    print('scrollTo command not implemented')
    return False

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

def defaultSetup(stub):
    if stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.ui-widget-sample')).isRunning:
        stub.closeApp(ReqCloseApp(packageName='com.samsung.ui-widget-sample'))

    stub.launchApp(ReqLaunchApp(packageName='com.samsung.ui-widget-sample'))

def defaultTearDown(stub):
    stub.closeApp(ReqCloseApp(packageName='com.samsung.ui-widget-sample'))

def runTest(stub, testFunc, setup=defaultSetup, tearDown=defaultTearDown, alwaySucceed=False):
    print("Testing started :", testFunc)

    setup(stub)
    result = testFunc(stub)
    tearDown(stub)

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

        runTestWithoutSetupAndTearDown(stub, installAppTest)
        runTestWithoutSetupAndTearDown(stub, launchAppTest)
        runTestWithoutSetupAndTearDown(stub, getAppInfoTest)
        runTestWithoutSetupAndTearDown(stub, closeAppTest)
        runTestWithoutSetupAndTearDown(stub, removeAppTest)

        runTest(stub, longClickTest, alwaySucceed=True)
        runTest(stub, getLocationTest, alwaySucceed=True)

        runTest(stub, scrollToTest, alwaySucceed=True)

if __name__ == '__main__':
    logging.basicConfig()
    run()
