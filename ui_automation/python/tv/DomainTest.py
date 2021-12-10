from __future__ import print_function
import os
import sys
sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(os.path.dirname(__file__)))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

def runTest(stub, testFunc):
    print("Testing started :", testFunc)
    result = testFunc(stub)
    print("Testing result :", result)

def csfsTest(stub):
    # 1.Check automatic run when booting
    # 2.Check SmartHub / Run / Exit  key 
    # 3.Check Return/Exit key function
    # 4.Check Up/Down/Left/Right and Focus key function
    # 5.Check Settings Preview 
    # 6.Check Apps Preview
    # 7.Check Source Preview
    # 8.Check tile editing 
    # 9.Check tile editing popup
    # 10.Check tile add
    # 11.Check Visual Defect

def searchAllTest(stub):
    # 12.Check normal execution
    # 13.Check keys input 
    # 14.Check Key Navigation
    # 15.Check Return/Exit key
    # 16.Check if Crash occurs
    # 17.Check the alignment of the entered text (Input Field)

def myContentsTest(stub):
    # 18.Check normal execution
    # 19.Check Key Navigation
    # 20.Check Return/Exit key
    # 21.Check Visual Defect
    # 22.Check Music Player
    # 23.Check Vidoe Player
    # 24.Check Photo Player 
    # 25.Check if Crash occur

def appsTest(stub)
    # 26.Check the normal execution
    # 27.Check Key Navigation
    # 28.Check Return/Exit key
    # 29.Check Visual Defect
    # 30.RTL Language Test
    # 31.Check if Crash occur

def galleryTest(stub)
    # 32.Check normal execution
    # 33.Check Key input
    # 34.Check Key Navigation
    # 35.Check Return/Exit key
    # 36.Check if Crash occur

def ambientTest(stub)
    # 37.Check normal execution
    # 38.Check Key Navigation
    # 39.Check Return/Exit key
    # 40.Check if Crash occur

def healthAppTest(stub)
    # 41.Check normal execution
    # 42.Check key Navigation
    # 43.Check Return/Exit key
    # 44.Check if Crash occur

def multiViewTest(stub)
    # 45.Check normal execution
    # 46.Check key navigation 
    # 47.Check Visual Defect 
    # 48.Check Return/Exit key 
    # 49.Check if Crash occur 

def notificationTest(stub):
    # 50.SKIP
    # 51.SKIP

def billingApp(stub):
    # 52.BillingPinPopup Test

def ssoAppTest(stub):
    # 54.Check Vector resources
    # 55.Check TextPageUtil

def searchAppsCsfsTest(stub)
    # 56.Check Window Rotation

def searchAllCsfsTest(stub)
    # 57.Memory Leak Test

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        # Domain Test Start
        #runTestWithoutSetupAndTearDown(stub, launchAppTest)
        #runTestWithoutSetupAndTearDown(stub, PickerExecuteTestWithText)
        #runTestWithoutSetupAndTearDown(stub, PickerScrollTest)
        #runTestWithoutSetupAndTearDown(stub, closeAppTest)

if __name__ == '__main__':
    logging.basicConfig()
    run()
