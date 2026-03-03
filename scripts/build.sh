#!/bin/bash
# 
# Build the project

if [ ! -d "zmqscript" ];
then
  echo "zmqscript project missing."
  exit 1
fi

if [ ! -d "zmqscript/build" ];
then
  mkdir zmqscript/build
fi
pushd zmqscript/build
git pull
cmake ..
if [ "$1" == "newflo" ]; then
  rm -rf _deps/flo-*
fi
make
[ "$?" != "0" ] && exit 1
make test
[ "$?" != "0" ] && exit 1
popd
