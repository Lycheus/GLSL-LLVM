#CC := /usr/local/arm-2010q1/bin/arm-none-linux-gnueabi-gcc
CC := gcc
#AR := /usr/local/arm-2010q1/bin/arm-none-linux-gnueabi-ar
AR := ar

#RANLIB := /usr/local/arm-2010q1/bin/arm-none-linux-gnueabi-ranlib
RANLIB := ranlib

SRCS := scanner.c atom.c memory.c tokens.c cpp.c cppstruct.c symbols.c
OBJECTS := $(SRCS:.c=.o)

default: all
all : libPreprocessor.a
libPreprocessor.a : $(OBJECTS)
	$(AR) rvu $@ $(OBJECTS)
	$(RANLIB) $@

%.o : %.c
	$(CC) -fPIC -Wall -c $<

#
# Cleanup
#
.PHONY : clean
clean :
	$(RM) *.o *.a

depend:
	$(CC) -M $(SRCS) > .depend

-include .depend

scanner.o: slglobals.h memory.h atom.h scanner.h parser.h cpp.h tokens.h
scanner.o: symbols.h compile.h
atom.o: slglobals.h memory.h atom.h scanner.h parser.h cpp.h tokens.h
atom.o: symbols.h compile.h
memory.o: memory.h
cpp.o: slglobals.h memory.h atom.h scanner.h parser.h cpp.h tokens.h
cpp.o: symbols.h compile.h
cppstruct.o: slglobals.h memory.h atom.h scanner.h parser.h cpp.h tokens.h
cppstruct.o: symbols.h compile.h
symbols.o: slglobals.h memory.h atom.h scanner.h parser.h cpp.h tokens.h
symbols.o: symbols.h compile.h

