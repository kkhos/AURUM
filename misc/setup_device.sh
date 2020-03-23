#!/bin/bash

sdb forward --remove-all
sdb forward tcp:50051 tcp:50051
sdb shell 'su - owner -c "systemctl --user start aurum-bootstrap"'
sdb shell "ps -ef | grep aurum-bootstrap"
