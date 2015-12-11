INCLUDE = -I. -I../glslang -I../glslang/Include -I../glslang/OSDependent/Linux -I../glslang/MachineIndependent
WARNINGS = -Wall -Wwrite-strings -Wpointer-arith -Wcast-align -Wstrict-prototypes \
	-Wnested-externs
DEFINE =

#CXX = /usr/local/arm-2010q1/bin/arm-none-linux-gnueabi-g++
CXX = g++


CXXOPTIONS = -O3 -g -Wno-deprecated -D_ALT_NS=1 -fPIC
CXXFLAGS = $(CXXOPTIONS) $(DEFINE) $(INCLUDE)

#
# Linking related
#

#AR = /usr/local/arm-2010q1/bin/arm-none-linux-gnueabi-ar
AR = ar
STATIC_OPTION = rcs

#
# Misc
#
export PERL = perl
export RM = rm -f
export MV = mv -f
export DEPEND = $(CXX) -M

#
# Object file variables are defined here.
#
OSSRCS = InitializeDll.cpp
OSOBJS = InitializeDll.o

LIBNAME = libInitializeDll.a

all : $(LIBNAME)

$(LIBNAME) : $(OSOBJS)
	 $(AR) $(STATIC_OPTION) $(LIBNAME) $(OSOBJS) 

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

#
# Dependency
#
depend : $(OSSRCS)
	$(DEPEND) $(CXXFLAGS) $(OSSRCS) > .depend

-include .depend

#
# Cleanup
#
.PHONY : clean
clean :
	$(RM) *.o *.a
