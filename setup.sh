#!/bin/sh

if [ -z "$1" ]
then
      CONFIG=Debug
else
      CONFIG=$1
fi

PROJECT=clip
BUILDER=Ninja

mkdir -p build/${CONFIG}
mkdir -p installed/${CONFIG}

mkdir -p build/${CONFIG}/hl-${PROJECT}
pushd build/${CONFIG}/hl-${PROJECT}
cmake -G${BUILDER} -DCMAKE_BUILD_TYPE=${CONFIG} -DCMAKE_INSTALL_PREFIX=../../../installed/${CONFIG} ../../.. 
popd

