from __future__ import print_function
import os
import sys
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

# Launch application. it returns application running state
def CheckNormalExecutionTest(stub):
    stub.launchApp(ReqLaunchApp(packageName='com.samsung.tv.store'))
    # Wait until app launch
    time.sleep(5)
    return stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.store')).isRunning

# Install BillingApp in Apps
def InstallBillingAppTest(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Right'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(0.1)

    response = stub.findElement(ReqFindElement(widgetType='TextField'))
    if len(response.elements) <= 0:
        print("Fail to find text field.")
        return False

    targetObj = response.elements[0].elementId
    stub.setValue(ReqSetValue(elementId=targetObj, stringValue='Billing'))
    time.sleep(3)

    # The IME Focus policy is weird so i move to focus to Down here in test scenario
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Down'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(0.1)

    # Install button click
    time.sleep(3)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))

    # Change this waiting to isInstalled()
    time.sleep(10)
    if stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.CSBilling')).isInstalled != True:
        print("Fail to install Billing App")
        return False

    return True

# Launch BillingApp and check it runs well or not
def CheckLaunchBillingAppTest(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(10)

    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(10)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Down'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Down'))
    time.sleep(0.1)
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(0.1)
    
    time.sleep(5)
    if stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.CSBilling')).isRunning != True:
        print("Fail to launch Billing App")
        return False

    return True

def runTest(stub, testFunc):
    print("Testing started :", testFunc)

    result = testFunc(stub)

    print("Testing result :", result)
    return True

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        runTest(stub, CheckNormalExecutionTest)
        runTest(stub, InstallBillingAppTest)
        runTest(stub, CheckLaunchBillingAppTest)
        #runTest(stub, CheckPhoneBillingViewTest)
        #runTest(stub, CheckTermsofUseViewTest)
        #runTest(stub, CheckCrashOccurs)

if __name__ == '__main__':
    logging.basicConfig()
    run()
