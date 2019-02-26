#include "widget.h"
#include "data.h"
#include "avl_tree.h"
#include <iostream>
#include <string>
#include <QApplication>
#include <QPushButton>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
