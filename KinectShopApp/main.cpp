#include <QApplication>
#include "html5applicationviewer.h"

#include <QtSql>

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "con1");
    db.setHostName("kinectsrv.lfb.rwth-aachen.de");
    db.setDatabaseName("kinectshop2015");
    db.setUserName("kinectshopClient");
    db.setPassword("lfb-student2015");

    QApplication app(argc, argv);

    Html5ApplicationViewer viewer;
    viewer.setOrientation(Html5ApplicationViewer::ScreenOrientationAuto);
    viewer.showExpanded();
    viewer.loadFile(QLatin1String("src/index.html"));
    viewer.setFixedSize(1000, 800);

    return app.exec();
}
