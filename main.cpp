#include <QApplication>

#include "TuxEyes.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    TuxEyes tuxeyes;
    tuxeyes.show();

    return app.exec();
}
