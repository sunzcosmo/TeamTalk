#!/bin/bash

CPP_DIR=../../server/src/base/pb/protocol/
SRC_DIR=./gen

# C++ dir
if [ ! -d ${CPP_DIR} ]; then
    mkdir -p ${CPP_DIR}
fi

cp -R ${SRC_DIR}/cpp/* "${CPP_DIR}"

rm -r ./gen
