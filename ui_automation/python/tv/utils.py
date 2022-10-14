from __future__ import print_function
import os
import subprocess
import re
import sys
import time
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import grpc

def runCommand(command):
    stream = os.popen(command)
    output = stream.read()
    print(output)

def checkSdb():
    stream = os.popen("command -v sdb")
    output = stream.read()
    if len(output) > 0:
        return True
    else:
        print("Error - Can't run sdb")

def getDeviceInfo():
    stream = os.popen("sdb shell cat /etc/info.ini")
    output = stream.read()
    print(output)

def isAurumReady():
    stream = os.popen("sdb shell 'ps -e | grep aurum-bootstrap'")
    output = stream.read()
    if len(output) > 0:
        return True
    else:
        print("Error - aurum-bootstrap not launched on target")
        print("Try to launch aurum-bootstrap")
        stream = os.popen("sdb shell app_launcher -s org.tizen.aurum-bootstrap")
        output = stream.read()
        time.sleep(1)
        if len(output) > 0:
            print("Success to launch aurum-bootstrap")
            return True

        print("Error - Can't launch aurum-bootstrap")
        return False

# display always on
def displayStop():
    runCommand("sdb shell devicectl display stop")
    runCommand("stty sane")


# NOTE: It may not work depending on how user sets the proxy.
def getStub():
    stream = os.popen("env | grep proxy")
    output = stream.read()
    if len(output) > 0:
        stub = BootstrapStub(grpc.insecure_channel('l27.0.0.1:50051', options=(('grpc.enable_http_proxy', 0),)))
        return stub
    else:
        stub = BootstrapStub(grpc.insecure_channel('127.0.0.1:50051'))
        return stub

