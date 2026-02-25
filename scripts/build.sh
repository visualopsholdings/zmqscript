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
make -j4
make test
popd
