from __future__ import print_function
from aurum_pb2 import *
import aurum_pb2_grpc
import logging
import grpc
import time

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = aurum_pb2_grpc.BootstrapStub(channel)

#        print(stub.getLocation(ReqGetLocation()).status)
#        print(stub.sync(ReqEmpty()))
#        print(stub.getDeviceTime(ReqGetDeviceTime(type='WALLCLOCK')))
#        print(stub.sendKey(ReqKey(type='WHEELUP')))
#        time.sleep(0.1)
#        print(stub.sendKey(ReqKey(type='WHEELDOWN')))
#        time.sleep(0.1)
#        print(stub.sendKey(ReqKey(type='HOME')))
#        print(stub.sendKey(ReqKey(type='POWER')))
#        stub.killServer(aurum_pb2.ReqEmpty())

if __name__ == '__main__':
    logging.basicConfig()
    run()
