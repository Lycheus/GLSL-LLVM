#############################################################################
# Makefile for building: showimg
# Generated by qmake (1.07a) (Qt 3.3.8b) on: Fri Mar 20 16:15:50 2015
# Project:  showimg.pro
# Template: app
# Command: $(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o Makefile showimg.pro
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

HEADERS = showimg.h \
		imagetexteditor.h \
		imagefip.h
SOURCES = main.cpp \
		imagetexteditor.cpp \
		showimg.cpp \
		imagefip.cpp
OBJECTS = .obj/release-shared/main.o \
		.obj/release-shared/imagetexteditor.o \
		.obj/release-shared/showimg.o \
		.obj/release-shared/imagefip.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = .moc/release-shared/moc_showimg.cpp \
		.moc/release-shared/moc_imagetexteditor.cpp \
		.moc/release-shared/moc_imagefip.cpp
OBJMOC = .obj/release-shared/moc_showimg.o \
		.obj/release-shared/moc_imagetexteditor.o \
		.obj/release-shared/moc_imagefip.o
DIST	   = showimg.pro
QMAKE_TARGET = showimg
DESTDIR  = 
TARGET   = showimg

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

Makefile: showimg.pro ../../.qmake.cache /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++/qmake.conf ../../lib/libqt.prl
	$(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o Makefile showimg.pro
qmake: 
	@$(QMAKE) -spec /home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/mkspecs/linux-g++ -o Makefile showimg.pro

dist: 
	@mkdir -p .obj/release-shared/showimg && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared/showimg/ && ( cd `dirname .obj/release-shared/showimg` && $(TAR) showimg.tar showimg && $(GZIP) showimg.tar ) && $(MOVE) `dirname .obj/release-shared/showimg`/showimg.tar.gz . && $(DEL_FILE) -r .obj/release-shared/showimg

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:

yaccclean:
lexclean:
clean: mocclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/release-shared/main.o: main.cpp showimg.h \
		imagefip.h \
		../../include/qapplication.h \
		../../include/qimage.h \
		../../include/qwidget.h \
		../../include/qwindowdefs.h \
		../../include/qobject.h \
		../../include/qpaintdevice.h \
		../../include/qpalette.h \
		../../include/qfont.h \
		../../include/qfontmetrics.h \
		../../include/qfontinfo.h \
		../../include/qsizepolicy.h \
		../../include/qwinexport.h \
		../../include/qobjectdefs.h \
		../../include/qstring.h \
		../../include/qnamespace.h \
		../../include/qglobal.h \
		../../include/qconfig-minimal.h \
		../../include/qconfig-small.h \
		../../include/qconfig-medium.h \
		../../include/qconfig-large.h \
		../../include/qconfig.h \
		../../include/qmodules.h \
		../../include/qfeatures.h \
		../../include/qcstring.h \
		../../include/qmemarray.h \
		../../include/qgarray.h \
		../../include/qshared.h \
		../../include/qevent.h \
		../../include/qregion.h \
		../../include/qmime.h \
		../../include/qpair.h \
		../../include/qrect.h \
		../../include/qsize.h \
		../../include/qpoint.h \
		../../include/qmap.h \
		../../include/qptrlist.h \
		../../include/qdatastream.h \
		../../include/qvaluelist.h \
		../../include/qiodevice.h \
		../../include/qtl.h \
		../../include/qtextstream.h \
		../../include/qglist.h \
		../../include/qptrcollection.h \
		../../include/qcolor.h \
		../../include/qbrush.h \
		../../include/qstringlist.h \
		../../include/qstrlist.h \
		../../include/qpixmap.h \
		../../include/qfiledialog.h \
		../../include/qdir.h \
		../../include/qdialog.h \
		../../include/qurloperator.h \
		../../include/qurlinfo.h \
		../../include/qfileinfo.h \
		../../include/qfile.h \
		../../include/qdatetime.h \
		../../include/qurl.h \
		../../include/qnetworkprotocol.h \
		../../include/qdict.h \
		../../include/qgdict.h \
		../../include/qdesktopwidget.h \
		../../include/qasciidict.h \
		../../include/qtranslator.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/main.o main.cpp

.obj/release-shared/imagetexteditor.o: imagetexteditor.cpp imagetexteditor.h \
		../../include/qimage.h \
		../../include/qlayout.h \
		../../include/qgrid.h \
		../../include/qvbox.h \
		../../include/qhbox.h \
		../../include/qcombobox.h \
		../../include/qmultilineedit.h \
		../../include/qlabel.h \
		../../include/qlineedit.h \
		../../include/qlistbox.h \
		../../include/qpushbutton.h \
		../../include/qdialog.h \
		../../include/qwidget.h \
		../../include/qwindowdefs.h \
		../../include/qobject.h \
		../../include/qpaintdevice.h \
		../../include/qpalette.h \
		../../include/qfont.h \
		../../include/qfontmetrics.h \
		../../include/qfontinfo.h \
		../../include/qsizepolicy.h \
		../../include/qwinexport.h \
		../../include/qobjectdefs.h \
		../../include/qstring.h \
		../../include/qnamespace.h \
		../../include/qglobal.h \
		../../include/qconfig-minimal.h \
		../../include/qconfig-small.h \
		../../include/qconfig-medium.h \
		../../include/qconfig-large.h \
		../../include/qconfig.h \
		../../include/qmodules.h \
		../../include/qfeatures.h \
		../../include/qcstring.h \
		../../include/qmemarray.h \
		../../include/qgarray.h \
		../../include/qshared.h \
		../../include/qevent.h \
		../../include/qregion.h \
		../../include/qmime.h \
		../../include/qpair.h \
		../../include/qrect.h \
		../../include/qsize.h \
		../../include/qpoint.h \
		../../include/qmap.h \
		../../include/qptrlist.h \
		../../include/qdatastream.h \
		../../include/qvaluelist.h \
		../../include/qiodevice.h \
		../../include/qtl.h \
		../../include/qtextstream.h \
		../../include/qglist.h \
		../../include/qptrcollection.h \
		../../include/qcolor.h \
		../../include/qbrush.h \
		../../include/qstringlist.h \
		../../include/qstrlist.h \
		../../include/qpixmap.h \
		../../include/qframe.h \
		../../include/qtextedit.h \
		../../include/qscrollview.h \
		../../include/qstylesheet.h \
		../../include/qptrvector.h \
		../../include/qscrollbar.h \
		../../include/qrangecontrol.h \
		../../include/qdict.h \
		../../include/qgvector.h \
		../../include/qgdict.h \
		../../include/qbutton.h \
		../../include/qiconset.h \
		../../include/qkeysequence.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/imagetexteditor.o imagetexteditor.cpp

.obj/release-shared/showimg.o: showimg.cpp showimg.h \
		imagetexteditor.h \
		../../include/qmenubar.h \
		../../include/qfiledialog.h \
		../../include/qmessagebox.h \
		../../include/qpopupmenu.h \
		../../include/qlabel.h \
		../../include/qpainter.h \
		../../include/qapplication.h \
		../../include/qclipboard.h \
		../../include/qwidget.h \
		../../include/qimage.h \
		../../include/qwindowdefs.h \
		../../include/qobject.h \
		../../include/qpaintdevice.h \
		../../include/qpalette.h \
		../../include/qfont.h \
		../../include/qfontmetrics.h \
		../../include/qfontinfo.h \
		../../include/qsizepolicy.h \
		../../include/qwinexport.h \
		../../include/qobjectdefs.h \
		../../include/qstring.h \
		../../include/qnamespace.h \
		../../include/qglobal.h \
		../../include/qconfig-minimal.h \
		../../include/qconfig-small.h \
		../../include/qconfig-medium.h \
		../../include/qconfig-large.h \
		../../include/qconfig.h \
		../../include/qmodules.h \
		../../include/qfeatures.h \
		../../include/qcstring.h \
		../../include/qmemarray.h \
		../../include/qgarray.h \
		../../include/qshared.h \
		../../include/qevent.h \
		../../include/qregion.h \
		../../include/qmime.h \
		../../include/qpair.h \
		../../include/qrect.h \
		../../include/qsize.h \
		../../include/qpoint.h \
		../../include/qmap.h \
		../../include/qptrlist.h \
		../../include/qdatastream.h \
		../../include/qvaluelist.h \
		../../include/qiodevice.h \
		../../include/qtl.h \
		../../include/qtextstream.h \
		../../include/qglist.h \
		../../include/qptrcollection.h \
		../../include/qcolor.h \
		../../include/qbrush.h \
		../../include/qstringlist.h \
		../../include/qstrlist.h \
		../../include/qpixmap.h \
		../../include/qdialog.h \
		../../include/qframe.h \
		../../include/qmenudata.h \
		../../include/qiconset.h \
		../../include/qkeysequence.h \
		../../include/qsignal.h \
		../../include/qvariant.h \
		../../include/qdir.h \
		../../include/qurloperator.h \
		../../include/qurlinfo.h \
		../../include/qfileinfo.h \
		../../include/qfile.h \
		../../include/qdatetime.h \
		../../include/qurl.h \
		../../include/qnetworkprotocol.h \
		../../include/qdict.h \
		../../include/qgdict.h \
		../../include/qpen.h \
		../../include/qpointarray.h \
		../../include/qwmatrix.h \
		../../include/qdesktopwidget.h \
		../../include/qasciidict.h \
		../../include/qtranslator.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/showimg.o showimg.cpp

.obj/release-shared/imagefip.o: imagefip.cpp imagefip.h \
		../../include/qimage.h \
		../../include/qfiledialog.h \
		../../include/qstrlist.h \
		../../include/qpixmap.h \
		../../include/qdir.h \
		../../include/qdialog.h \
		../../include/qurloperator.h \
		../../include/qurlinfo.h \
		../../include/qglobal.h \
		../../include/qfileinfo.h \
		../../include/qconfig-minimal.h \
		../../include/qconfig-small.h \
		../../include/qconfig-medium.h \
		../../include/qconfig-large.h \
		../../include/qconfig.h \
		../../include/qmodules.h \
		../../include/qfeatures.h \
		../../include/qstring.h \
		../../include/qptrlist.h \
		../../include/qdatastream.h \
		../../include/qcstring.h \
		../../include/qwinexport.h \
		../../include/qmemarray.h \
		../../include/qgarray.h \
		../../include/qshared.h \
		../../include/qglist.h \
		../../include/qptrcollection.h \
		../../include/qiodevice.h \
		../../include/qfile.h \
		../../include/qdatetime.h \
		../../include/qnamespace.h \
		../../include/qwidget.h \
		../../include/qwindowdefs.h \
		../../include/qobject.h \
		../../include/qpaintdevice.h \
		../../include/qpalette.h \
		../../include/qfont.h \
		../../include/qfontmetrics.h \
		../../include/qfontinfo.h \
		../../include/qsizepolicy.h \
		../../include/qobjectdefs.h \
		../../include/qevent.h \
		../../include/qregion.h \
		../../include/qmime.h \
		../../include/qpair.h \
		../../include/qrect.h \
		../../include/qsize.h \
		../../include/qpoint.h \
		../../include/qmap.h \
		../../include/qvaluelist.h \
		../../include/qtl.h \
		../../include/qtextstream.h \
		../../include/qcolor.h \
		../../include/qbrush.h \
		../../include/qstringlist.h \
		../../include/qurl.h \
		../../include/qnetworkprotocol.h \
		../../include/qdict.h \
		../../include/qgdict.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/imagefip.o imagefip.cpp

.obj/release-shared/moc_showimg.o: .moc/release-shared/moc_showimg.cpp  showimg.h ../../include/qwidget.h \
		../../include/qimage.h \
		../../include/qwindowdefs.h \
		../../include/qobject.h \
		../../include/qpaintdevice.h \
		../../include/qpalette.h \
		../../include/qfont.h \
		../../include/qfontmetrics.h \
		../../include/qfontinfo.h \
		../../include/qsizepolicy.h \
		../../include/qwinexport.h \
		../../include/qobjectdefs.h \
		../../include/qstring.h \
		../../include/qnamespace.h \
		../../include/qglobal.h \
		../../include/qconfig-minimal.h \
		../../include/qconfig-small.h \
		../../include/qconfig-medium.h \
		../../include/qconfig-large.h \
		../../include/qconfig.h \
		../../include/qmodules.h \
		../../include/qfeatures.h \
		../../include/qcstring.h \
		../../include/qmemarray.h \
		../../include/qgarray.h \
		../../include/qshared.h \
		../../include/qevent.h \
		../../include/qregion.h \
		../../include/qmime.h \
		../../include/qpair.h \
		../../include/qrect.h \
		../../include/qsize.h \
		../../include/qpoint.h \
		../../include/qmap.h \
		../../include/qptrlist.h \
		../../include/qdatastream.h \
		../../include/qvaluelist.h \
		../../include/qiodevice.h \
		../../include/qtl.h \
		../../include/qtextstream.h \
		../../include/qglist.h \
		../../include/qptrcollection.h \
		../../include/qcolor.h \
		../../include/qbrush.h \
		../../include/qstringlist.h \
		../../include/qstrlist.h \
		../../include/qpixmap.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/moc_showimg.o .moc/release-shared/moc_showimg.cpp

.obj/release-shared/moc_imagetexteditor.o: .moc/release-shared/moc_imagetexteditor.cpp  imagetexteditor.h ../../include/qdialog.h \
		../../include/qwidget.h \
		../../include/qwindowdefs.h \
		../../include/qobject.h \
		../../include/qpaintdevice.h \
		../../include/qpalette.h \
		../../include/qfont.h \
		../../include/qfontmetrics.h \
		../../include/qfontinfo.h \
		../../include/qsizepolicy.h \
		../../include/qwinexport.h \
		../../include/qobjectdefs.h \
		../../include/qstring.h \
		../../include/qnamespace.h \
		../../include/qglobal.h \
		../../include/qconfig-minimal.h \
		../../include/qconfig-small.h \
		../../include/qconfig-medium.h \
		../../include/qconfig-large.h \
		../../include/qconfig.h \
		../../include/qmodules.h \
		../../include/qfeatures.h \
		../../include/qcstring.h \
		../../include/qmemarray.h \
		../../include/qgarray.h \
		../../include/qshared.h \
		../../include/qevent.h \
		../../include/qregion.h \
		../../include/qmime.h \
		../../include/qpair.h \
		../../include/qrect.h \
		../../include/qsize.h \
		../../include/qpoint.h \
		../../include/qmap.h \
		../../include/qptrlist.h \
		../../include/qdatastream.h \
		../../include/qvaluelist.h \
		../../include/qiodevice.h \
		../../include/qtl.h \
		../../include/qtextstream.h \
		../../include/qglist.h \
		../../include/qptrcollection.h \
		../../include/qcolor.h \
		../../include/qbrush.h \
		../../include/qstringlist.h \
		../../include/qstrlist.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/moc_imagetexteditor.o .moc/release-shared/moc_imagetexteditor.cpp

.obj/release-shared/moc_imagefip.o: .moc/release-shared/moc_imagefip.cpp  imagefip.h ../../include/qfiledialog.h \
		../../include/qstrlist.h \
		../../include/qpixmap.h \
		../../include/qdir.h \
		../../include/qdialog.h \
		../../include/qurloperator.h \
		../../include/qurlinfo.h \
		../../include/qglobal.h \
		../../include/qfileinfo.h \
		../../include/qconfig-minimal.h \
		../../include/qconfig-small.h \
		../../include/qconfig-medium.h \
		../../include/qconfig-large.h \
		../../include/qconfig.h \
		../../include/qmodules.h \
		../../include/qfeatures.h \
		../../include/qstring.h \
		../../include/qptrlist.h \
		../../include/qdatastream.h \
		../../include/qcstring.h \
		../../include/qwinexport.h \
		../../include/qmemarray.h \
		../../include/qgarray.h \
		../../include/qshared.h \
		../../include/qglist.h \
		../../include/qptrcollection.h \
		../../include/qiodevice.h \
		../../include/qfile.h \
		../../include/qdatetime.h \
		../../include/qnamespace.h \
		../../include/qwidget.h \
		../../include/qwindowdefs.h \
		../../include/qobject.h \
		../../include/qpaintdevice.h \
		../../include/qpalette.h \
		../../include/qfont.h \
		../../include/qfontmetrics.h \
		../../include/qfontinfo.h \
		../../include/qsizepolicy.h \
		../../include/qobjectdefs.h \
		../../include/qevent.h \
		../../include/qregion.h \
		../../include/qmime.h \
		../../include/qpair.h \
		../../include/qrect.h \
		../../include/qsize.h \
		../../include/qpoint.h \
		../../include/qmap.h \
		../../include/qvaluelist.h \
		../../include/qtl.h \
		../../include/qtextstream.h \
		../../include/qcolor.h \
		../../include/qbrush.h \
		../../include/qstringlist.h \
		../../include/qurl.h \
		../../include/qnetworkprotocol.h \
		../../include/qdict.h \
		../../include/qgdict.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared/moc_imagefip.o .moc/release-shared/moc_imagefip.cpp

.moc/release-shared/moc_showimg.cpp: $(MOC) showimg.h
	$(MOC) showimg.h -o .moc/release-shared/moc_showimg.cpp

.moc/release-shared/moc_imagetexteditor.cpp: $(MOC) imagetexteditor.h
	$(MOC) imagetexteditor.h -o .moc/release-shared/moc_imagetexteditor.cpp

.moc/release-shared/moc_imagefip.cpp: $(MOC) imagefip.h
	$(MOC) imagefip.h -o .moc/release-shared/moc_imagefip.cpp

####### Install

install:  

uninstall:  

