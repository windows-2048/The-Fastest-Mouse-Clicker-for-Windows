// Qt5 docs: https://doc.qt.io/archives/qt-5.15/qthread.html
// TODO: https://github.com/kwhat/libuiohook/blob/1.2/demo/demo_hook.c
// TODO2: C:\Users\test\Downloads\winrec\main.cpp

#include "TheFastestMouseClicker.h"
#include "Application.h"

#include <QtWidgets/QApplication>
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>

#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QWindowsVistaStylePlugin)


TheFastestMouseClicker* g_tfmc = nullptr;

/////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    /*
    QString strScaleFactor = "1";
    {
        QApplication a(argc, argv);
        QScreen* pScreen = QGuiApplication::primaryScreen();
        int height = pScreen->geometry().height();
        if (height > 1500)
            strScaleFactor = "2";
    }
    qputenv("QT_SCALE_FACTOR", strScaleFactor.toStdString().c_str());
    */

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
    Application a(argc, argv);
    //QApplication a(argc, argv);

    // Set application properties
    a.setApplicationName("The Fastest Mouse Clicker for Windows version 3");
    a.setApplicationVersion("3.0.1.0");
    a.setOrganizationName("The Fastest Mouse Clicker for Windows");

    // Set application icon from Qt resources
    // This will use the same icon from resources.qrc
    QIcon appIcon(":/Resources/favicon.ico");
    a.setWindowIcon(appIcon);

    TheFastestMouseClicker mainWin;

    mainWin.setWindowIcon(appIcon);
    mainWin.setWindowTitle("The Fastest Mouse Clicker for Windows version 3 v3.0.1.0");

    mainWin.setWindowFlags(Qt::WindowStaysOnTopHint);

    g_tfmc = &mainWin;
    
    mainWin.show();

    //a.tfmc = &mainWin;
    //a.setAttribute(Qt::AA_EnableHighDpiScaling);
    return a.exec(); // .exec starts QApplication and related GUI, this line starts 'event loop'    
}
