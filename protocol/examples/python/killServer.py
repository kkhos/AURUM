from __future__ import print_function
import aurum_pb2
import aurum_pb2_grpc
import logging
import grpc

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = aurum_pb2_grpc.BootstrapStub(channel)
        stub.killServer(aurum_pb2.ReqEmpty())

if __name__ == '__main__':
    logging.basicConfig()
    run()