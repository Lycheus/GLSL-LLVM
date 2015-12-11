/* Install paths from configure */

static const char QT_INSTALL_PREFIX      [267] = "qt_nstpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b";
static const char QT_INSTALL_BINS        [267] = "qt_binpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/bin";
static const char QT_INSTALL_DOCS        [267] = "qt_docpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/doc";
static const char QT_INSTALL_HEADERS     [267] = "qt_hdrpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/include";
static const char QT_INSTALL_LIBS        [267] = "qt_libpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/lib";
static const char QT_INSTALL_PLUGINS     [267] = "qt_plgpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/plugins";
static const char QT_INSTALL_DATA        [267] = "qt_datpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b";
static const char QT_INSTALL_TRANSLATIONS[267] = "qt_trnpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/translations";
static const char QT_INSTALL_SYSCONF     [267] = "qt_cnfpath=/home/billy/3D/3D_src/QT_showframe/qt_test/qt-x11-free-3.3.8b/etc/settings";

/* strlen( "qt_xxxpath=" ) == 11 */
const char *qInstallPath()             { return QT_INSTALL_PREFIX       + 11; }
const char *qInstallPathDocs()         { return QT_INSTALL_DOCS         + 11; }
const char *qInstallPathHeaders()      { return QT_INSTALL_HEADERS      + 11; }
const char *qInstallPathLibs()         { return QT_INSTALL_LIBS         + 11; }
const char *qInstallPathBins()         { return QT_INSTALL_BINS         + 11; }
const char *qInstallPathPlugins()      { return QT_INSTALL_PLUGINS      + 11; }
const char *qInstallPathData()         { return QT_INSTALL_DATA         + 11; }
const char *qInstallPathTranslations() { return QT_INSTALL_TRANSLATIONS + 11; }
const char *qInstallPathSysconf()      { return QT_INSTALL_SYSCONF      + 11; }
