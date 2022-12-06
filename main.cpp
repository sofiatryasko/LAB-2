#include "notes.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    notes w;
    w.show();
    return a.exec();
}
