#include <QApplication>
#include "html5applicationviewer.h"

#include "sqlfunctions.h"

int main(int argc, char *argv[])
{
    sqlfunctions* obj = new sqlfunctions;

    QApplication app(argc, argv);

    Html5ApplicationViewer viewer;
    viewer.setOrientation(Html5ApplicationViewer::ScreenOrientationAuto);
    viewer.showExpanded();
    viewer.loadFile(QLatin1String("src/index.html"));
    viewer.setFixedSize(1200, 900);

    return app.exec();
}
