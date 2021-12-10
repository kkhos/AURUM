from __future__ import print_function
import os
import sys
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
from domain_test_utils import *
import logging
import grpc
import time
import subprocess
import re

success = 0
fail = 0

def csfsNormalExecutionTest(stub):
    HOME_KEY(stub)
    return IS_APP_RUNNING(stub, 'com.samsung.tv.csfs')

def csfsDirectionMovingAndFocusTest(stub):
    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    prevObj = response.elements[0].elementId

    DOWN_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False
    prevObj = response.elements[0].elementId

    LEFT_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False
    prevObj = response.elements[0].elementId

    RIGHT_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False
    prevObj = response.elements[0].elementId

    UP_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False

    return True

def csfsSettingsPreview(stub):
    for i in range(1,3):
        DOWN_KEY(stub)

        response = FIND_FOCUSED_ELEMENT(stub)
        if CHECK_ELEMENT(response) == False: return False
        prevObj = response.elements[0].elementId

        RIGHT_LONG_KEY(stub)

        response = FIND_FOCUSED_ELEMENT(stub)
        if CHECK_ELEMENT(response) == False: return False
        if COMP_ID(prevObj, response.elements[0].elementId) == False: return False

    return True

def csfsTileEditing(stub):
    UP_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    prevObj = response.elements[0].elementId

    RETURN_KEY(stub)
    DOWN_KEY(stub)
    RETURN_KEY(stub)
    LEFT_KEY(stub)
    LEFT_KEY(stub)
    RETURN_KEY(stub)
    RIGHT_KEY(stub)
    RIGHT_KEY(stub)
    RIGHT_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) != False: return False

    return True

def csfsTileEditingPopup(stub):
    RETURN_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    prevObj = response.elements[0].elementId

    DOWN_KEY(stub)
    DOWN_KEY(stub)
    RETURN_KEY(stub)
    LEFT_KEY(stub) 
    RETURN_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False

    return True

def searchAllNormalExecutionTest(stub):
    SEND_KEY(stub, 'XF86Search', 8)
    return IS_APP_RUNNING(stub, 'com.samsung.tv.searchall')

def searchAllKeysInputTest(stub):
    response = FIND_EDITABLE_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False

    targetObj = response.elements[0].elementId
    SET_VALUE(stub, targetObj, 'Movie')

    time.sleep(ACTION_DELAY_3000MS)

    value = GET_VALUE(stub, targetObj)
    if value.stringValue != 'Movie':
        print("Fail to set text")
        return False

    return True

def searchAllKeyNavigationTest(stub):
    # Application Focus policy is weird... So it goes up first then move to Down for the thumbnail list
    UP_KEY(stub)
    DOWN_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    prevObj = response.elements[0].elementId

    RIGHT_LONG_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False

    return True

def searchAllReturnExitKeyTest(stub):
    EXIT_KEY(stub)
    if IS_APP_RUNNING(stub, 'com.samsung.tv.searchall') == True:
        print("Fail to exit application")
        return False
    return True

def appsNormalExecutionTest(stub):
    APP_LAUNCH(stub, 'com.samsung.tv.store')
    return stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.store')).isRunning

def appsKeyNavigationTest(stub):
    DOWN_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    prevObj = response.elements[0].elementId

    RIGHT_LONG_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False

    DOWN_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    prevObj = response.elements[0].elementId

    LEFT_LONG_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if CHECK_ELEMENT(response) == False: return False
    if COMP_ID(prevObj, response.elements[0].elementId) == False: return False

    return True

def appsReturnExitKeyTest(stub):
    EXIT_KEY(stub)
    if IS_APP_RUNNING(stub, 'com.samsung.tv.store') == True:
        print("Fail to exit application")
        return False
    return True

def healthInstallLaunchSamsungHealthAppTest(stub):
    # Move focus to search icon in top right of app store.
    UP_KEY(stub)
    UP_KEY(stub)
    RIGHT_KEY(stub)
    RETURN_KEY(stub)

    # Find input box and input 'Samsung Health'
    response = FIND_EDITABLE_ELEMENT(stub)
    if len(response.elements) <= 0:
        print("Fail to find text field!")
        return False

    targetObj = response.elements[0].elementId
    SET_VALUE(stub, targetObj, 'Samsung Health')
    time.sleep(3)

    # Make sure focus is moved to text field.
    for i in range(1,4):
        UP_KEY(stub)

    # Move focus to health icon.
    DOWN_KEY(stub)
    RETURN_KEY(stub)
    time.sleep(3)

    # Check if app is running or not.
    if IS_APP_INSTALLED(stub, 'com.samsung.tv.samsung-health'):
        return True

    # Install button click
    RETURN_KEY(stub)
    time.sleep(10)

    # Check if app is installed or not.
    if IS_APP_INSTALLED(stub, 'com.samsung.tv.samsung-health') != True:
        print("Fail to install Health App!!")
        return False

    return True

def healthCheckNormalExecutionTest(stub):
    # Launch app store.
    HOME_KEY(stub)
    UP_KEY(stub)
    LEFT_KEY(stub)
    RETURN_KEY(stub)

    # Check if app store is running or not.
    if IS_APP_RUNNING(stub, 'com.samsung.tv.store') == False:
        print("Fail to launch tv store!")
        return False

    # Check if app is installed or not.
    if IS_APP_INSTALLED(stub, 'com.samsung.tv.samsung-health'):
        APP_LAUNCH(stub, 'com.samsung.tv.samsung-health')
        time.sleep(10)  # It takes a long time to launch health app.
        return IS_APP_RUNNING(stub, 'com.samsung.tv.samsung-health')

    # If health app is not installed, install and launch it.
    if healthInstallLaunchSamsungHealthAppTest(stub) == False:
        print("Fail to install samsung health!")
        return False

    # Make sure that health app is focused.
    APP_LAUNCH(stub, 'com.samsung.tv.samsung-health')
    return IS_APP_RUNNING(stub, 'com.samsung.tv.samsung-health')

def healthCheckKeyNavigationTest(stub):
    response = FIND_FOCUSED_ELEMENT(stub)
    if len(response.elements) <= 0:
        print("Fail to find the first focused object!")
        return False
    prevObj = response.elements[0].elementId

    # Move focus to next item.
    DOWN_KEY(stub)

    response = FIND_FOCUSED_ELEMENT(stub)
    if len(response.elements) <= 0:
        print("Fail to find the second focused object!!")
        return False

    if prevObj == response.elements[0].elementId:
        print("Focus is not moved!")
        return False

    return True

def healthCheckReturnExitKeyTest(stub):
    # Send exit key.
    EXIT_KEY(stub)

    # Check if the health app exited or not.
    return IS_APP_FOCUSED(stub, 'com.samsung.tv.samsung-health') != True

def ambientCheckNormalExecutionTest(stub):
    # Enter 'Media' home.
    HOME_KEY(stub)

    # Move focus to 'Media' item.
    LEFT_KEY(stub)

    # Move focus to 'Ambient' item.
    UP_KEY(stub)

    # Enter ambient.
    RETURN_KEY(stub)

    # Enter the first embient app.
    RETURN_KEY(stub)
    time.sleep(5) #Because it takes a long time to load ambient app.

    # If the embient app is launched, focus of home app would be lost.
    return IS_APP_FOCUSED(stub, 'com.samsung.tv.csfs') != True

def ambientCheckKeyNavigationTest(stub):
    # Return to Home.
    EXIT_KEY(stub)

    # Query focused element.
    response = FIND_FOCUSED_ELEMENT(stub)
    if len(response.elements) <= 0:
        print("Fail to find focused object!")
        return False
    prevObj = response.elements[0].elementId

    # Go to the second ambient app.
    DOWN_KEY(stub)

    # Enter the second embient app.
    RETURN_KEY(stub)
    time.sleep(5) #Because it takes a long time to load ambient app.

    # Check if the second embient app is launched or not.
    if IS_APP_FOCUSED(stub, 'com.samsung.tv.csfs'):
        print("Fail to launch ambient app!")
        return False

    # Return to Home.
    EXIT_KEY(stub)

    # Query focused element.
    response = FIND_FOCUSED_ELEMENT(stub)
    if len(response.elements) <= 0:
        print("Fail to find focused object!!")
        return False

    # Check if focus is moved or not.
    if prevObj == response.elements[0].elementId:
        print("Focus not moving！")
        return False

    # If the embient app exited, home app would be focused.
    return IS_APP_FOCUSED(stub, 'com.samsung.tv.csfs')

def ambientCheckReturnExitKeyTest(stub):
    return True

def csfsRotationTest(stub):
    response = run_command("sdb shell enlightenment_info -rotation set 90")

    return True


def runTest(stub, testFunc):
    global success
    global fail
    print("Testing started :", testFunc)

    result = testFunc(stub)
    if result == True: success += 1
    else: fail +=1

    print("Testing result :", result)
    return True

def setup(stub):
    CHECK_NOTI_POPUP(stub)
    global success
    global fail
    success = 0
    fail = 0

def run_command(command):
    stream = os.popen(command)
    output = stream.read()

def ambientTest(stub):
    global success
    global fail
    print("=================== Ambient Apps test Start ===================")
    setup(stub)
    # Launch application. it returns application running state
    runTest(stub, ambientCheckNormalExecutionTest)
    # Check focus is moved normally.
    runTest(stub, ambientCheckKeyNavigationTest)
    # Check the Exit key event is generated well or not
    runTest(stub, ambientCheckReturnExitKeyTest)
    print("=================== Ambient Apps test Finish (P:", success, " F:", fail, ") ===================")

def healthTest(stub):
    global success
    global fail
    print("=================== Samsung Health App test Start ===================")
    setup(stub)
    # Launch application. it returns application running state
    runTest(stub, healthCheckNormalExecutionTest)
    # Check focus is moved normally.
    runTest(stub, healthCheckKeyNavigationTest)
    # Check the Exit key event is generated well or not
    runTest(stub, healthCheckReturnExitKeyTest)
    print("=================== Samsung Health App test Finish (P:", success, " F:", fail, ") ===================")

def csfsTearDown(stub):
    UP_KEY(stub)
    RETURN_KEY(stub)

def csfsTest(stub):
    global success
    global fail
    print("=================== CSFS App test Start ===================")
    setup(stub)
    # Launch application. it returns application running state
    runTest(stub, csfsNormalExecutionTest)
    # Find Foused item and move to Up/Down/Left/Right then check the focus moved well or not
    runTest(stub, csfsDirectionMovingAndFocusTest)
    # Long press list toggle 2 lists and check the focused item
    runTest(stub, csfsSettingsPreview)
    # Go to edit mode and move item to left then check the item moved well or not
    runTest(stub, csfsTileEditing)
    # Go to edit mode and delete tile then check the tile deleted well or not
    runTest(stub, csfsTileEditingPopup)
    csfsTearDown(stub)
    print("=================== CSFS App test Finish (P:", success, " F:", fail, ") ===================")

def searchAllTest(stub):
    global success
    global fail
    print("=================== SearchAll App test Start ===================")
    setup(stub)
    # Launch application. it returns application running state
    runTest(stub, searchAllNormalExecutionTest)
    # Find textfield and input 'Movie' then check the textfield has text as 'Movie'
    runTest(stub, searchAllKeysInputTest)
    # Find Foused item and move focus to right then compare focused item with previous one
    runTest(stub, searchAllKeyNavigationTest)
    # Check the Exit Key event generate well or not
    runTest(stub, searchAllReturnExitKeyTest)
    print("=================== SearchAll App test Finish (P:", success, " F:", fail, ") ===================")

def appsTest(stub):
    global success
    global fail
    print("=================== Store App test Start ===================")
    setup(stub)
    # Launch application. it returns application running state
    runTest(stub, appsNormalExecutionTest)
    # Find Foused item and move focus to right then compare focused item with previous one
    runTest(stub, appsKeyNavigationTest)
    # Check the Exit Key event generate well or not
    runTest(stub, appsReturnExitKeyTest)
    print("=================== Store App test Finish (P:", success, " F:", fail, ") ===================")

def rotationTest(stub):
    global success
    global fail
    print("=================== CSFS App rotation test Start ===================")
    setup(stub)
    # Launch application. it returns application running state
    runTest(stub, csfsNormalExecutionTest)
    # rotatt csfs
    runTest(stub, csfsRotationTest)
    csfsTearDown(stub)
    print("=================== CSFS App rotation test Finish (P:", success, " F:", fail, ") ===================")

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        rotationTest(stub)

if __name__ == '__main__':
    logging.basicConfig()
    run()
