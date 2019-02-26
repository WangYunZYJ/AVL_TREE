#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPushButton>
#include <QLineEdit>
#include <iostream>
#include <algorithm>
#include "data.h"
#include "position.h"
#include <string>
#include <QString>
#include "avl_tree.h"
#include "QLabel"
#include <QFile>
#include <QTextStream>
#include <ios>
#include <fstream>

int status = -1;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    root(new avl_tree()),
    flag(0),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->name, SIGNAL(textChanged(QString)), this, SLOT(get_name()));
    connect(ui->passward, SIGNAL(textChanged(QString)), this, SLOT(get_passward()));
    connect(ui->Insert, SIGNAL(clicked(bool)), this, SLOT(insert_click_button()));
    connect(ui->Delete, SIGNAL(clicked(bool)), this, SLOT(delete_click_button()));
    connect(ui->clear_data, SIGNAL(clicked(bool)), this, SLOT(clear_all_data()));
    connect(ui->Update, SIGNAL(clicked(bool)), this, SLOT(update_data()));
    connect(ui->Login, SIGNAL(clicked(bool)), this, SLOT(login_click_button()));
    connect(ui->Delete, SIGNAL(clicked(bool)), this, SLOT(show_text()));
    connect(ui->Update, SIGNAL(clicked(bool)), this, SLOT(show_text()));
    connect(ui->Insert, SIGNAL(clicked(bool)), this, SLOT(show_text()));
    connect(ui->Login, SIGNAL(clicked(bool)), this, SLOT(show_text()));
    connect(ui->clear_data, SIGNAL(clicked(bool)), this, SLOT(show_text()));
    this->read_from_file();
}//-------------------------------------------------------界面的构造函数，初始化数据和绑定信号与槽

Widget::~Widget()
{
    delete ui;
    delete root;
}//-------------------------------------------------------析构

void Widget::paintEvent(QPaintEvent *)
{
    root = const_cast<avl_tree*>(root->get_root());
    if(root != nullptr)
        paint_tree(root);
}//-------------------------------------------------------事件函数，可以被update（）函数调用

void Widget::paint_tree(avl_tree* node)
{
    if(node->stu_info.id == "") return;
    QString str = QString::fromStdString(node->stu_info.id);
    QString str0 = QString::fromStdString(node->stu_info.passward);
    QPainter painter(this);
    painter.setPen(Qt::blue);
    if(str.length() == 1)
        painter.setFont(QFont("Arial", 30));
    else painter.setFont(QFont("Arial", 15));
    painter.drawEllipse(node->circle_pos.x, node->circle_pos.y,
                            node->circle_pos.d, node->circle_pos.d);
    //--------------------------------------------------------------画圈
    painter.drawText(node->circle_pos.x+10,node->circle_pos.y + 35, str + '+' + str0);
    //--------------------------------------------------------------画字
    if(node->left_child != nullptr)
    {
        QLine line(node->circle_pos.x+8, node->circle_pos.y+32,
                node->left_child->circle_pos.x+32, node->left_child->circle_pos.y+8);
        painter.drawLine(line);
        paint_tree(node->left_child);
    }
    if(node->right_child != nullptr)
    {
        QPainter tempP(this);
        tempP.setPen(Qt::blue);
        QLine line(node->circle_pos.x + 32, node->circle_pos.y + 32,
                   node->right_child->circle_pos.x+8, node->right_child->circle_pos.y+8);
        tempP.drawLine(line);
        paint_tree(node->right_child);
    }//------------------------------------------------------------画线
}//----------------------------------------------------------------画树函数

void Widget::insert_click_button()
{
    if(stu_info.id == "")
    {
        ui->name->setText("");
        ui->passward->setText("");
        status = PLEASE_INPUT_ACCOUNT;
        return;
    }
    if(!root->fin_stu_info_without_passward(stu_info))
        status = REGISTER_SUCCEED;
    else
    {

        ui->name->setText("");
        ui->passward->setText("");
        status = ACCOUNT_EXIST_ALREADY;
        return;
    }
    root->insert_data(*(new my_data(stu_info.id, stu_info.passward)));
    root = const_cast<avl_tree*>(root->get_root());
    ui->name->setText("");
    ui->passward->setText("");
    root->update_pos();
    update();
}//--------------------------------------点击登陆按钮后插入数据

void Widget::login_click_button()
{
    if(stu_info.id == "")
    {
        status = PLEASE_INPUT_ACCOUNT;
        return;
    }
    if(!root->fin_stu_info(stu_info))
        status = PLEASE_CHECK_YOUR_ACCOUNT_OR_PASSWARD;
    else status = LOGIN_SUCCEED;
    ui->name->setText("");
    ui->passward->setText("");
}

void Widget::get_name()
{
    stu_info.id = ui->name->text().toStdString();
}//--------------------------------------得到单行文本框name输入的字符串作为数据的id

void Widget::get_passward()
{
    stu_info.passward = ui->passward->text().toStdString();
}//--------------------------------------得到单行文本框passward输入的字符串作为数据的passward

void Widget::delete_click_button()
{
    if(stu_info.id == "")
    {
        status = PLEASE_INPUT_ACCOUNT;
        return;
    }
    if(root->fin_stu_info(stu_info))
        status = DELETE_SUCCEED;
    else
    {
        status = PLEASE_CHECK_YOUR_ACCOUNT_OR_PASSWARD;
        return;
    }
    root->delete_data(stu_info.id);
    root = const_cast<avl_tree*>(root->get_root());
    ui->name->setText("");
    ui->passward->setText("");
    update();
}//--------------------------------------单击删除按钮进行数据删除的操作

void Widget::clear_all_data()
{
    status = CLEAR_ALL_SUCCEED;
    root = new avl_tree();
    update();
}//-------------------------------------清除所有数据

void Widget::update_data()
{
    if(stu_info.id == "")
    {
        ui->name->setText("");
        ui->passward->setText("");
        return;
    }
    if(!root->fin_stu_info_without_passward(stu_info))
    {
        ui->name->setText("");
        ui->passward->setText("");
        status = IS_NOT_REGISTER;
        return;
    }
    status = CHANGE_PASSWARD_SUCCEED;
    my_data my_stu_info;
    my_stu_info.id = stu_info.id;
    my_stu_info.passward = stu_info.passward;
    root->update_data(my_stu_info);
    root = const_cast<avl_tree*>(root->get_root());
    ui->name->setText("");
    ui->passward->setText("");
    root->update_pos();
    update();
}//------------------------------------更新数据

void Widget::show_text()
{
    QFile file(root->path);
    file.resize(0);
    this->print_to_file();
    switch (status)
    {
    case LOGIN_SUCCEED:
        ui->ShowLabel->setText("登陆成功");
        break;
    case IS_NOT_REGISTER:
        ui->ShowLabel->setText("用户未注册");
        break;
    case REGISTER_SUCCEED:
        ui->ShowLabel->setText("注册成功");
        break;
    case PLEASE_CHECK_YOUR_ACCOUNT_OR_PASSWARD:
        ui->ShowLabel->setText("请检查及你的帐号或者密码是否正确");
        break;
    case DELETE_SUCCEED:
        ui->ShowLabel->setText("帐号删除成功");
        break;
    case CHANGE_PASSWARD_SUCCEED:
        ui->ShowLabel->setText("密码更改成功");
        break;
    case CLEAR_ALL_SUCCEED:
        ui->ShowLabel->setText("所有数据已经删除");
        break;
    case PLEASE_INPUT_ACCOUNT:
        ui->ShowLabel->setText("请输入帐号");
        break;
    case ACCOUNT_EXIST_ALREADY:
        ui->ShowLabel->setText("帐号已存在");
        break;
    default:
        ui->ShowLabel->setText("null");
        break;
    }
}//-------------------------------------在标签展示登陆状态

void Widget::print_to_file() const
{
    root->print_to_file();
}

void Widget::read_from_file() const
{
    root->read_from_file();
}
