#include <QtGui/QApplication>
#include "VSXuWidget.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    VSXuWidget foo;
    foo.show();
    return app.exec();
}
