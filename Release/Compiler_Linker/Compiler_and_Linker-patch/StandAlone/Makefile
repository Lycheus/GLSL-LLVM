OBJECTPATH=./../glslang/MachineIndependent
LIBPATH=./../glslang/MachineIndependent/lib
TARGETNAME=StandAlone
SRCS=$(TARGETNAME).cpp
#CXX=/usr/local/arm-2010q1/bin/arm-none-linux-gnueabi-g++
CXX=g++

TARGETOBJECT=$(TARGETNAME).o

all: SHAREDOBJECT $(TARGETNAME)

$(TARGETNAME): $(TARGETOBJECT) 
#$(CXX) StandAlone.o ConvertSO.a -o tset_StandAlone -lpthread

$(CXX) -g -o $@ $(TARGETOBJECT) -L $(LIBPATH) -lglslang -lpthread -lm -lstdc++

SHAREDOBJECT:
	cd $(OBJECTPATH); make all

%.o : %.cpp
	$(CXX) -c $<

#
# Cleanup
#
.PHONY : clean SHAREDOBJECT

clean :
	$(RM) *.o StandAlone *.so
	cd $(OBJECTPATH); make clean

depend:
	$(CXX) -M $(SRCS) > .depend
	cd $(OBJECTPATH); make depend

-include .depend

$(TARGETOBJECT): ./../glslang/Include/ShHandle.h
$(TARGETOBJECT): ./../glslang/Public/ShaderLang.h
$(TARGETOBJECT): ./../glslang/Include/ResourceLimits.h
$(TARGETOBJECT): ./../glslang/Include/InfoSink.h ./../glslang/Include/Common.h
$(TARGETOBJECT): ./../glslang/Include/PoolAlloc.h
$(TARGETOBJECT): ./../glslang/Public/ShaderLang.h


