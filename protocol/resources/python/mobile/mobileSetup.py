import os
import subprocess
import re
import sys
import time

def run_command(command):
    stream = os.popen(command)
    output = stream.read()

# Start scrip here
run_command("sdb forward tcp:50051 tcp:50051")
run_command("sdb shell tpk-backend -y org.tizen.aurum-bootstrap --preload")
run_command("sdb shell app_launcher -s org.tizen.aurum-bootstrap")
# Wait 1 sec till bootstrap launched
time.sleep(1)
