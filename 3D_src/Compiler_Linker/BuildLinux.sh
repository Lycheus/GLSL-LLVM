#! /bin/sh

# build the StandAlone app and all it's dependencies
#make -C StandAlone

# so we can find the shared library
#LD_LIBRARY_PATH=`pwd`/glslang/MachineIndependent/lib:${LD_LIBRARY_PATH}
#LD_LIBRARY_PATH=`pwd`/glslang/MachineIndependent/lib
#export LD_LIBRARY_PATH

# run using test data
cd $1/Compiler_and_Linker-patch/StandAlone 
#pwd
#echo "./StandAlone -a $@"
./StandAlone -a $2
# ./StandAlone -a brick.vert brick.frag

