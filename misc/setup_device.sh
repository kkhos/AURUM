#!/bin/bash
sdb forward --remove-all
sdb forward tcp:50051 tcp:50051
sdb shell app_launcher -t org.tizen.aurum-bootstrap
sdb shell app_launcher -s org.tizen.aurum-bootstrap
