import os
import subprocess
import re
import sys
import time

def run_command(command):
    stream = os.popen(command)
    output = stream.read()

# Start scrip here
run_command("sdb root on")
# To run bootstrap as command
run_command("sdb shell tpk-backend -y org.tizen.aurum-bootstrap --preload")
# TV need to enable touch
run_command("sdb shell vconftool set -f -t bool memory/window_system/input/force_enable_touch 1")
# Dut to input generator issue, temporarily do below command
run_command("sdb shell winfo -init_device --type=touch")
run_command("sdb forward tcp:50051 tcp:50051")
run_command("sdb shell app_launcher -s org.tizen.aurum-bootstrap")
# Wait 1 sec till bootstrap launched
time.sleep(1)
