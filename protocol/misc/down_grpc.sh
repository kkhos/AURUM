#!/bin/bash -xe

repo_url=http://download.tizen.org/snapshots/tizen/unified/latest/repos/standard/packages/armv7l/
repo_dbg_url=http://download.tizen.org/snapshots/tizen/unified/latest/repos/standard/debug/

libgrpc_filter=" -A libgrpc* "
grpc_filter=" -A grpc-* "
grpc_dbg_filter=" -A grpc-debug*armv7l* -A libatspi-debug*armv7l&"
libgrpc_dbg_filter=" -A libgrpc-debug*armv7l* "

test -d rpm && rm -rf rpm

wget -P rpm --recursive -nd -nH -np $repo_url $grpc_filter
wget -P rpm --recursive -nd -nH -np $repo_url $libgrpc_filter

wget -P rpm --recursive -nd -nH -np $repo_dbg_url $grpc_dbg_filter
wget -P rpm --recursive -nd -nH -np $repo_dbg_url $libgrpc_dbg_filter
