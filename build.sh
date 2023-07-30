#########################################################################
# File Name:    stress_test_start.sh
# Author:       ryan
# mail:         xieg678@gmail.com
# Created Time: Wed Jan 30 20:05:15 2019
#########################################################################
#!/bin/bash

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-./build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-./${BUILD_TYPE}-install}
CXX=${CXX:-g++}

ln -sf $BUILD_DIR/$BUILD_TYPE/compile_commands.json
#cp $BUILD_DIR/$BUILD_TYPE/compile_commands.json ./compile_commands.json

mkdir -p $BUILD_DIR/$BUILD_TYPE \
  && cd $BUILD_DIR/$BUILD_TYPE \
  && cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           $SOURCE_DIR \
  && make $*

# Use the following command to run all the unit tests
# at the dir $BUILD_DIR/$BUILD_TYPE :
# CTEST_OUTPUT_ON_FAILURE=TRUE make test

# cd $SOURCE_DIR && doxygen