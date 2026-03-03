#!/bin/bash
# 
# Build the project

if [ ! -d "zmqscript" ];
then
  echo "zmqscript project missing."
  exit 1
fi

uname -a | grep "6.17.0-1008-raspi"
if [ "$?" == "0" ]; then
  NPROC="-j2"
else
  NPROC="-j4"
fi

echo "Building with $NPROC"

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
make $NPROC
[ "$?" != "0" ] && exit 1
make test
[ "$?" != "0" ] && exit 1
popd
