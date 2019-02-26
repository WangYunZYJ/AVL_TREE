#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QMouseEvent>
class my_button : public QPushButton
{
protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // BUTTON_H
