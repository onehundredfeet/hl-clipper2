#!/bin/sh

if [ -z "$1" ]
then
      CONFIG=Debug
else
      CONFIG=$1
fi

PROJECT=clip

pushd build/${CONFIG}/hl-${PROJECT}
ninja install
popd
