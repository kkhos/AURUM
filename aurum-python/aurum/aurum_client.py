import grpc
import cv2
import numpy as np
import time
from skimage.metrics import structural_similarity

from typing import List

from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub

DEFAULT_HOST = '127.0.0.1'
DEFAULT_PORT = 50051

class AurumClient:
    def __init__(self, host = DEFAULT_HOST, port = DEFAULT_PORT):
        self.host = host
        self.port = port
        self.stub = None

    def start(self) -> bool:
        self.stub = BootstrapStub(grpc.insecure_channel(self.host + ':' + str(self.port), options = [('grpc.enable_http_proxy', 0)]))
        return self.is_running

    def stop(self) -> bool:
        is_terminated = False
        if self.is_running:
            self.stub.close()
            is_terminated = True
        self.stub = None
        return is_terminated

    @property
    def is_running(self) -> bool:
        return self.stub is not None

    def get_processes(self) -> List[str]:
        list = []
        els = self.stub.findElements(ReqFindElements(maxDepth = 1, isShowing = True))
        for el in els.elements:
            list.append(el.package)
        return list