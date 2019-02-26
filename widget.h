#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include "avl_tree.h"
#include "position.h"



namespace Ui
{
class Widget;
}

enum STATUS
{
    LOGIN_SUCCEED,
    IS_NOT_REGISTER,
    REGISTER_SUCCEED,
    PLEASE_CHECK_YOUR_ACCOUNT_OR_PASSWARD,
    DELETE_SUCCEED,
    CHANGE_PASSWARD_SUCCEED,
    CLEAR_ALL_SUCCEED,
    PLEASE_INPUT_ACCOUNT,
    ACCOUNT_EXIST_ALREADY
};

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    std::vector<position> pos;
    avl_tree* root;
    my_data stu_info;
    int flag;
public slots:
    void get_name();
    void get_passward();
    void insert_click_button();
    void login_click_button();
    void delete_click_button();
    void clear_all_data();
    void update_data();
    void show_text();
private:
    Ui::Widget *ui;
    void paintEvent(QPaintEvent *);
    void paint_tree(avl_tree* node);
    void print_to_file() const;
    void read_from_file() const;
};

#endif // WIDGET_H
