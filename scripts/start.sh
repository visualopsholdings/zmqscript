#!/bin/bash
# 
# Start zmqscript

if [ ! -f "zmqscript/build/zmqscript" ];
then
  echo "Need to build or install project first"
  exit 1
fi

LIST=`ps -ef | grep "build/zmqscript" | grep -v grep | awk '{print $2}'`
if [ ! -z "$LIST" ]; then
	echo "Already running"
	exit 0
fi

if [ -d nodes-lib ];
then
  export LD_LIBRARY_PATH=`pwd`/nodes-lib:/usr/local/lib:/usr/local/lib:$LD_LIBRARY_PATH
  export DYLD_LIBRARY_PATH=`pwd`/nodes-lib:/usr/local/lib:/usr/local/lib:$DYLD_LIBRARY_PATH
else
  export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
  export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH
fi

./zmqscript/build/zmqscript \
  --logLevel=debug \
  > zmqscript.log 2>&1 &
