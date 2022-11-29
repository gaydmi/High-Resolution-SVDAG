#!/bin/bash

## SPECIFY TRIMESH LOCATION HERE (and do a make there first)
TRIMESH_DIR=../src/trimesh2
SOURCE_DIR=../src/svdag_construction/

## COMPILE AND LINK DEFINITIONS
COMPILE="g++ -g -c -O3 -I../src/libs/tri_tools/include/ -I ${TRIMESH_DIR}/include/ -std=c++11"
COMPILE_BINARY="g++ -c -O3 -I../src/libs/tri_tools/include/ -I ${TRIMESH_DIR}/include/ -D BINARY_VOXELIZATION -std=c++11"
LINK="g++ -g -o svdag_constructor -std=c++11"

#############################################################################################
## BUILDING STARTS HERE

## CLEAN
echo "Removing old versions ..."
rm -f svdag_constructor
rm -f *.o


echo "Compiling regular version build..."
${COMPILE} ${SOURCE_DIR}SVDAG_main.cpp
${COMPILE} ${SOURCE_DIR}SVDAG.cpp
${COMPILE} ${SOURCE_DIR}SVDAG_node.cpp
echo "Linking regular version build..."
${LINK} *.o

echo "Cleaning up ..."
rm *.o

echo "Done"
