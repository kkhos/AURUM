from __future__ import print_function
import os
import sys
sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(os.path.dirname(__file__)))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

# Check the object in the screen(TV) or not
def inScreen(size):
    if size.x < 0: return False
    if size.y < 0: return False
    if size.x >= 1920: return False
    if size.y >= 1080: return False
    return True

# 1. Find TextField(entry)
# 2. Set Text as Picker
# 3. Click Run button
# 4. Find PickerTest1 item on the result list
# 5. Find 'Black' textlabel on the layout
def PickerExecuteTestWithText(stub):
    # 1
    response = stub.findElement(ReqFindElement(widgetType='TextField'))
    if response.element is None: return False
    # 2
    targetObj = response.element.elementId
    testString = 'Picker'
    stub.setValue(ReqSetValue(elementId=targetObj, stringValue=testString))
    # 3
    response = stub.findElement(ReqFindElement(textField='Run'))
    if response.element is None: return False
    targetObj = response.element.elementId
    stub.click(ReqClick(type='ELEMENTID', elementId=targetObj))
    # 4
    response = stub.findElement(ReqFindElement(textField='PickerTest1'))
    if response.element is None: return False
    targetObj = response.element.elementId
    stub.click(ReqClick(type='ELEMENTID', elementId=targetObj))
    # 5
    response = stub.findElements(ReqFindElements(textField='Black'))
    if len(response.elements) <= 0: return False

    return True

# 1. Find PickerTest1 item on the list
# 2. Click PickerTest1 item
# 3. Find 'Black' textlabel on the layout
def PickerExecuteTest(stub):
    for tryCnt in range(10):
        # 1
        stub.flick(ReqFlick(startPoint=Point(x=300, y=750), endPoint=Point(x=300, y=200), durationMs=150))
        response = stub.findElement(ReqFindElement(textField='PickerTest1'))
        if response.elements is None: continue
        targetObj = response.element.elementId
        response = stub.getSize(ReqGetSize(elementId=targetObj))
        if inScreen(response.size):
            # 2
            stub.click(ReqClick(type='ELEMENTID', elementId=targetObj))
            break

    # 3
    response = stub.findElements(ReqFindElements(textField='Black'))
    if len(response.elements) <= 0: return False
    return True

# 1. Find PickerScroller(Picker's internal scroller)
# 2. Find 'Black' textlabel on the layout
# 3. Get PickerScroller geometry value for flick event
# 4. Check the loop works well while changing the picker item by flick event
def PickerScrollTest(stub):
    # 1
    response = stub.findElements(ReqFindElements(widgetType='PickerScroller'))
    if len(response.elements) <= 0: return False
    # 2
    responseText = stub.findElement(ReqFindElement(textField='Black'))
    if response.element is None: return False
    # 3
    pickerCenterX = response.element.geometry.x + (response.element.geometry.width / 2)
    pickerCenterY = response.element.geometry.y + (response.element.geometry.height / 2)

    for tryCnt in range(30):
        # 4
        stub.flick(ReqFlick(startPoint=Point(x=int(pickerCenterX), y=int(pickerCenterY)), endPoint=Point(x=int(pickerCenterX), y=int(pickerCenterY-70)), durationMs=100))
        # Wait until view update
        time.sleep(0.3)
        response = stub.findElement(ReqFindElement(textField='Black'))
        if response.element:
            if response.element.elementId == responseText.element.elementId:
                return True

    return False

# Launch application. it returns application running state
def launchAppTest(stub):
    stub.launchApp(ReqLaunchApp(packageName='org.tizen.example.NUITizenGallery'))
    return stub.getAppInfo(ReqGetAppInfo(packageName='org.tizen.example.NUITizenGallery')).isRunning

# Close application. it returns application running state
def closeAppTest(stub):
    stub.closeApp(ReqCloseApp(packageName='org.tizen.example.NUITizenGallery'))
    return stub.getAppInfo(ReqGetAppInfo(packageName='org.tizen.example.NUITizenGallery')).isRunning != True

def defaultSetup(stub):
    if stub.getAppInfo(ReqGetAppInfo(packageName='org.tizen.example.NUITizenGallery')).isRunning:
        stub.closeApp(ReqCloseApp(packageName='org.tizen.example.NUITizenGallery'))

    stub.launchApp(ReqLaunchApp(packageName='org.tizen.example.NUITizenGallery'))

def defaultTearDown(stub):
    stub.closeApp(ReqCloseApp(packageName='org.tizen.example.NUITizenGallery'))

def runTest(stub, testFunc, setup=defaultSetup, tearDown=defaultTearDown, alwaySucceed=False):
    print("Testing started :", testFunc)

    setup(stub)
    result = testFunc(stub)
    tearDown(stub)

    print("Testing result :", result)
    if alwaySucceed: return True

def runTestWithoutSetupAndTearDown(stub, testFunc, setup=defaultSetup, tearDown=defaultTearDown):
    def Empty(stub):
        pass

    runTest(stub, testFunc, Empty, Empty)

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        # Picker Test
        runTestWithoutSetupAndTearDown(stub, launchAppTest)
        runTestWithoutSetupAndTearDown(stub, PickerExecuteTestWithText)
        runTestWithoutSetupAndTearDown(stub, PickerScrollTest)
        runTestWithoutSetupAndTearDown(stub, closeAppTest)

if __name__ == '__main__':
    logging.basicConfig()
    run()
