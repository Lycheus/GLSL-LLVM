#############################################################################
# Makefile for building: precompile
# Generated by qmake (1.07a) (Qt 3.3.8b) on: Wed Apr  8 21:53:50 2015
# Project:  ../precompile.pro
# Template: app
# Command: $(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o . ../precompile.pro
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
INCPATH  = -I/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -I../../precompile -I/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/include -I/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/include -I. -I../ -I. -I.moc/release-shared/
LINK     = g++
LFLAGS   =  -Wl,-rpath,/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/lib
LIBS     = $(SUBLIBS) -L/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/lib -L/usr/X11R6/lib -lqt -lXext -lX11 -lm -lSM -lICE -ldl
AR       = ar cqs
RANLIB   = 
MOC      = /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/bin/moc
UIC      = /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/bin/uic -L /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/plugins
QMAKE    = /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/bin/qmake
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

HEADERS = ../stable.h \
		../myobject.h
SOURCES = ../main.cpp \
		../myobject.cpp \
		../util.cpp
OBJECTS = .obj/release-shared/main.o \
		.obj/release-shared/myobject.o \
		.obj/release-shared/util.o \
		.obj/release-shared/mydialog.o
FORMS = ../mydialog.ui
UICDECLS = ./mydialog.h
UICIMPLS = ./mydialog.cpp
SRCMOC   = .moc/release-shared/moc_mydialog.cpp
OBJMOC = .obj/release-shared/moc_mydialog.o
DIST	   = ../precompile.pro
QMAKE_TARGET = precompile
DESTDIR  = 
TARGET   = precompile

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c -include precompile $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: . $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/src/moc && $(MAKE) )

.: ../precompile.pro /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/.qmake.cache /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++/qmake.conf /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/obj/lib/libqt.prl
	$(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o . ../precompile.pro
qmake: 
	@$(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o . ../precompile.pro

dist: 
	@mkdir -p ../.obj/release-shared/precompile && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) ../.obj/release-shared/precompile/ && ( cd `dirname ../.obj/release-shared/precompile` && $(TAR) precompile.tar precompile && $(GZIP) precompile.tar ) && $(MOVE) `dirname ../.obj/release-shared/precompile`/precompile.tar.gz . && $(DEL_FILE) -r ../.obj/release-shared/precompile

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:
	-$(DEL_FILE) $(UICIMPLS) $(UICDECLS)

yaccclean:
lexclean:
clean: mocclean uiclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) precompile.gch/c precompile.gch/c++
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


###### Prefix headers
precompile.gch/c: ../../stable.h 
	test -d precompile.gch/ || mkdir -p precompile.gch/
	$(CC) -x c-header -c $(CFLAGS) $(INCPATH) ../../stable.h -o precompile.gch/c

precompile.gch/c++: ../../stable.h 
	test -d precompile.gch/ || mkdir -p precompile.gch/
	$(CXX) -x c++-header -c $(CXXFLAGS) $(INCPATH) ../../stable.h -o precompile.gch/c++

FORCE:

####### Compile

.obj/release-shared/main.o: ../main.cpp precompile.gch/c++ \
		../myobject.h \
		mydialog.h
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/main.o ../main.cpp

.obj/release-shared/myobject.o: ../myobject.cpp precompile.gch/c++ \
		../myobject.h
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/myobject.o ../myobject.cpp

.obj/release-shared/util.o: ../util.cpp precompile.gch/c++
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/util.o ../util.cpp

mydialog.h: ../mydialog.ui 
	$(UIC) ../mydialog.ui -o mydialog.h

mydialog.cpp: mydialog.h ../mydialog.ui 
	$(UIC) ../mydialog.ui -i mydialog.h -o mydialog.cpp

.obj/release-shared/mydialog.o: ./mydialog.cpp precompile.gch/c++ \
		./mydialog.h
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/mydialog.o ./mydialog.cpp

.obj/release-shared/moc_mydialog.o: .moc/release-shared/moc_mydialog.cpp precompile.gch/c++ ./mydialog.h 
	$(CXX) -c -include precompile $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/moc_mydialog.o .moc/release-shared/moc_mydialog.cpp

.moc/release-shared/moc_mydialog.cpp: $(MOC) ./mydialog.h
	$(MOC) ./mydialog.h -o .moc/release-shared/moc_mydialog.cpp

####### Install

install:  

uninstall:  

