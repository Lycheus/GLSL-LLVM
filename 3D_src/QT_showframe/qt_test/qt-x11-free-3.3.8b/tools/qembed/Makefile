#############################################################################
# Makefile for building: qembed
# Generated by qmake (1.07a) (Qt 3.3.8b) on: Fri Mar 20 16:15:51 2015
# Project:  qembed.pro
# Template: app
# Command: $(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o Makefile qembed.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED
CXXFLAGS = -pipe -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -I. -I../../include -I.moc/release-shared/
LINK     = g++
LFLAGS   =  -Wl,-rpath,/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/lib
LIBS     = $(SUBLIBS) -L/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/lib -L/usr/X11R6/lib -lqt -lXext -lX11 -lm -lSM -lICE -ldl
AR       = ar cqs
RANLIB   = 
MOC      = /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/bin/moc
UIC      = /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/bin/uic -L /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/plugins
QMAKE    = /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/bin/qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = .obj/release-shared/

####### Files

HEADERS = 
SOURCES = qembed.cpp
OBJECTS = .obj/release-shared/qembed.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DIST	   = qembed.pro
QMAKE_TARGET = qembed
DESTDIR  = 
TARGET   = qembed

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/src/moc && $(MAKE) )

Makefile: qembed.pro ../../.qmake.cache /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++/qmake.conf ../../lib/libqt.prl
	$(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o Makefile qembed.pro
qmake: 
	@$(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o Makefile qembed.pro

dist: 
	@mkdir -p .obj/release-shared/qembed && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared/qembed/ && ( cd `dirname .obj/release-shared/qembed` && $(TAR) qembed.tar qembed && $(GZIP) qembed.tar ) && $(MOVE) `dirname .obj/release-shared/qembed`/qembed.tar.gz . && $(DEL_FILE) -r .obj/release-shared/qembed

mocclean:

uiclean:

yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/release-shared/qembed.o: qembed.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/qembed.o qembed.cpp

####### Install

install:  

uninstall:  

