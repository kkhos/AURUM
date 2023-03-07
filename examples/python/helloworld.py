import grpc
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub

def run():                                                         
    with grpc.insecure_channel('localhost:50051') as channel:      
#   If you have a proxy problem, use the below code instead of the above code.
#   with grpc.insecure_channel('localhost:50051', options=(('grpc.enable_http_proxy', 0),)) as channel:
        stub = BootstrapStub(channel)
        getDeviceTime(stub)

def getDeviceTime(stub):
    response = stub.getDeviceTime(ReqGetDeviceTime(type='SYSTEM'))
    print(response)

if __name__ == '__main__':                                         
    run()
