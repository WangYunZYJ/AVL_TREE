#include "avl_tree.h"
#include <exception>
#include "widget.h"
#include <iostream>
#include <string>
#include "position.h"
#include <ios>
#include "data.h"
#include <fstream>
#include <QTextStream>
#include <QFile>
#define max(a, b) (a) > (b) ? (a) : (b)
#define abs(x) (x) > 0 ? (x) : (-x)
avl_tree::avl_tree():
circle_pos(*(new position())),left_child(nullptr), right_child(nullptr), path("/home/cloud/文档/data.txt")
{
    father = nullptr;
    stu_info.id = "";
    stu_info.passward = "";
}//path路径设置为文件路径

avl_tree::avl_tree(const my_data &stu, avl_tree* temp_ptr):
    circle_pos(*(new position())),left_child(nullptr), right_child(nullptr), path("/home/cloud/文档/data.txt")
{
    father = temp_ptr;
    stu_info.id = stu.id;
    stu_info.passward = stu.passward;
}//path路径设置为文件路径

avl_tree::~avl_tree(){}

const avl_tree* avl_tree::search_left_child() const
{
    avl_tree* m_ptr = this->right_child;
    if(m_ptr == nullptr) return m_ptr;
    while(true)
    {
        if(m_ptr->left_child == nullptr)
            return m_ptr;
        m_ptr = m_ptr->left_child;
    }
}//往右子树找最左节点

const avl_tree* avl_tree::search_right_child() const
{
    avl_tree* m_ptr = this->left_child;
    if(m_ptr == nullptr) return m_ptr;
    while(true)
    {
        if(m_ptr->right_child == nullptr)
            return m_ptr;
        m_ptr = m_ptr->right_child;
    }
}//往左子树找最右节点

//删除操作
void avl_tree::delete_data(std::string str)
{
    avl_tree *m_ptr = this;//找操作节点
    while(true)
    {
        if(m_ptr == nullptr)
        {
            break;
        }
        if(m_ptr->stu_info.id == str)
        {
            if(m_ptr->left_child == nullptr && m_ptr->right_child != nullptr)//左空右不空，
            {
                m_ptr->stu_info = m_ptr->right_child->stu_info;
                m_ptr->right_child = nullptr;
            }
            else if(m_ptr->left_child != nullptr && m_ptr->right_child == nullptr)
            {
                m_ptr->stu_info = m_ptr->left_child->stu_info;
                m_ptr->left_child = nullptr;
            }//左不空右空
            else if(m_ptr->left_child == nullptr && m_ptr->right_child == nullptr)
            {
                if(m_ptr->father == nullptr)
                {
                    m_ptr->stu_info.id = "";
                    break;
                }
                if(m_ptr->father->left_child == m_ptr)
                    m_ptr->father->left_child = nullptr;
                else
                    m_ptr->father->right_child = nullptr;
                delete m_ptr;//删除叶子节点。
            }//左右皆空
            else
            {
                avl_tree* temp_ptr = const_cast<avl_tree*>(m_ptr->search_left_child());
                if(temp_ptr!=nullptr)//在右侧找到替代点
                {
                    m_ptr->stu_info = temp_ptr->stu_info;
                    if(temp_ptr->father == m_ptr)
                    {
                        m_ptr->right_child = temp_ptr->right_child;
                        if(m_ptr->right_child != nullptr)
                            m_ptr->right_child->father = m_ptr;
                    }
                    else if(temp_ptr->right_child!=nullptr)
                    {
                        temp_ptr->right_child->father = temp_ptr->father;
                        temp_ptr->father->left_child = temp_ptr->right_child;
                        m_ptr = temp_ptr->father;
                    }
                    else
                    {
                        temp_ptr->father->left_child = nullptr;
                        m_ptr = temp_ptr->father;
                    }
                }
                else
                {
                    temp_ptr = const_cast<avl_tree*>(m_ptr->search_right_child());
                    if(temp_ptr == nullptr)
                    {
                        this->stu_info.id = "";
                        return;
                    }
                    m_ptr->stu_info = temp_ptr->stu_info;
                    if(temp_ptr->father == m_ptr)
                    {
                        m_ptr->left_child = temp_ptr->left_child;
                        m_ptr->left_child->father = m_ptr;
                    }
                    else if(temp_ptr->left_child != nullptr)
                    {
                        temp_ptr->left_child->father = temp_ptr->father;
                        temp_ptr->father->right_child = temp_ptr->left_child;
                        m_ptr = temp_ptr->father;
                    }
                    else
                    {
                        temp_ptr->father->right_child = nullptr;
                        m_ptr = temp_ptr->father;
                    }
                }
                delete temp_ptr;
            }//左右都不空
            break;
        }
        if(my_data(str) < (m_ptr->stu_info))
           m_ptr = m_ptr->left_child;
        else m_ptr = m_ptr->right_child;//搜索操作
    }


    avl_tree *p = const_cast<avl_tree*>(this->get_root());
    p->update_height();
    while(m_ptr != nullptr)//while找到操作节点
    {
        if((abs(m_ptr->balance)) >= 2)
        {
            if(m_ptr->left_child!=nullptr)
            {
                if(m_ptr->left_child->left_child != nullptr)
                    m_ptr->rotate_RR();
            }
            if(m_ptr->right_child!=nullptr)
            {
                if(m_ptr->right_child->right_child != nullptr)
                    m_ptr->rotate_LL();
            }
            if(m_ptr->left_child != nullptr)
            {
                if(m_ptr->left_child->right_child != nullptr)
                    m_ptr->rorate_LR();
            }
            if(m_ptr->right_child != nullptr)
            {
                if(m_ptr->right_child->left_child != nullptr)
                    m_ptr->rotate_RL();
            }
        }//四种平衡操作。
        p = const_cast<avl_tree*>(this->get_root());
        p->update_height();
        m_ptr = m_ptr->father;
    }
    m_ptr = const_cast<avl_tree*>(this->get_root());
    m_ptr->update_height(); //更新树的高度
    m_ptr->update_pos(); //更新树的坐标
}

void avl_tree::insert_data(const my_data& stu)
{
    avl_tree *m_ptr = this;
    bool is_found = false;
    if(this->stu_info.id == "")
        stu_info = stu;
    else
    {
        is_found = false;
        while(!is_found)
        {
            if(stu == (m_ptr->stu_info))
            {
                break;
            }
            if(stu < (m_ptr->stu_info))
            {
                if(m_ptr->left_child == nullptr)
                {
                    is_found = true;
                    m_ptr->left_child = new avl_tree(stu, m_ptr);
                    m_ptr = m_ptr->left_child;
                }
                else
                {
                    m_ptr = m_ptr->left_child;
                }
            }
            else
            {
                if(m_ptr->right_child == nullptr)
                {
                    is_found = true;
                    m_ptr->right_child = new avl_tree(stu, m_ptr);
                    m_ptr = m_ptr->right_child;
                }
                else
                {
                    m_ptr = m_ptr->right_child;
                }
            }
        }
    }//找到插入点并插入
    this->update_height();

    //Balance Deal!!!
    avl_tree *c_ptr = m_ptr;

    while( (abs(m_ptr->balance))!=2)
    {
        //if(m_ptr == nullptr) break;
        m_ptr = m_ptr->father;
        if(m_ptr == nullptr) break;
    }//m_ptr为当前操作点
    if(m_ptr != nullptr)
    {
        if(m_ptr->left_child!=nullptr)
            if(m_ptr->left_child->left_child != nullptr)
                if(m_ptr->left_child->left_child->find(c_ptr))
                    m_ptr->rotate_RR();
        if(m_ptr->right_child!=nullptr)
            if(m_ptr->right_child->right_child != nullptr)
                if(m_ptr->right_child->right_child->find(c_ptr))
                    m_ptr->rotate_LL();
        if(m_ptr->left_child != nullptr)
            if(m_ptr->left_child->right_child != nullptr)
                if(m_ptr->left_child->right_child->find(c_ptr))
                    m_ptr->rorate_LR();
        if(m_ptr->right_child != nullptr)
            if(m_ptr->right_child->left_child != nullptr)
                if(m_ptr->right_child->left_child->find(c_ptr))
                    m_ptr->rotate_RL();
    }//四种平衡操作
    this->update_height();
    this->update_pos();
}

void avl_tree::update_data(const my_data &stu)
{
    avl_tree *m_ptr = this;
    while(true)
    {
        if(m_ptr == nullptr)
        {
            this->insert_data(stu);
            break;
        }
        if(stu.id == m_ptr->stu_info.id)
        {
            m_ptr->stu_info.passward = stu.passward;
            break;
        }
        if(stu < (m_ptr->stu_info))
            m_ptr = m_ptr->left_child;
        else m_ptr = m_ptr->right_child;
    }
}//更新数据操作

void avl_tree::update_height()
{
    int left_height = 0;
    int right_height = 0;
    if(this->left_child != nullptr)
        left_height = this->left_child->find_height();
    if(this->right_child != nullptr)
        right_height = this->right_child->find_height();
    this->balance = left_height - right_height;
    if(this->left_child!=nullptr)
        this->left_child->update_height();
    if(this->right_child != nullptr)
    this->right_child->update_height();
}//更新树的高度

int avl_tree::find_height() const
{
    if(this->left_child == nullptr && this->right_child == nullptr)
        return 1;
    if(this->left_child!=nullptr && this->right_child!=nullptr)
        return max(this->left_child->find_height() + 1, this->right_child->find_height() + 1);
    else if(this->left_child!=nullptr&&this->right_child ==nullptr)
        return this->left_child->find_height() + 1;
    else
        return this->right_child->find_height() + 1;
}//找到树高

void avl_tree::rotate_RR()//简单右旋
{
    avl_tree* temp;
    if(this->left_child!=nullptr)
    {\
        temp = this->left_child->right_child;
        this->left_child->father = this->father;
    }
    if(this->father != nullptr)
    {
        if(this->father->left_child == this)
            this->father->left_child = this->left_child;
        else
            this->father->right_child = this->left_child;
    }
    if(this->left_child->right_child != nullptr)
        this->left_child->right_child->father = this;
    this->left_child->right_child = this;
    this->father = this->left_child;
    this->left_child = temp;
}


void avl_tree::rotate_LL()//简单左旋
{
    avl_tree* temp;
    if(this->right_child != nullptr)
    {
        temp = this->right_child->left_child;
        this->right_child->father = this->father;
    }
    if(this->father!=nullptr)
    {
        if(this->father->left_child == this)
            this->father->left_child = this->right_child;
        else
            this->father->right_child = this->right_child;
    }
    if(this->right_child->left_child != nullptr)
        this->right_child->left_child->father = this;
    this->right_child->left_child = this;
    this->father = this->right_child;
    this->right_child = temp;
}

void avl_tree::rorate_LR()//左右旋
{
    if(this->left_child!=nullptr)
        this->left_child->rotate_LL();
    this->rotate_RR();
}

void avl_tree::rotate_RL()//右左旋转
{
    if(this->right_child!=nullptr)
        this->right_child->rotate_RR();
    this->rotate_LL();
}

int avl_tree::find(avl_tree *tag) const
{
    if(this == tag) return 1;
    if(this->left_child != nullptr && this->right_child !=nullptr)
        return this->left_child->find(tag) + this->right_child->find(tag);
    else if(this->left_child == nullptr && this->right_child != nullptr)
        return this->right_child->find(tag);
    else if(this->left_child != nullptr)
        return this->left_child->find(tag);
    else return 0;
}//找数据所在的节点指针。

void avl_tree::print_test() const
{
    if(this->left_child != nullptr)
        this->left_child->print_test();
    if(this->right_child != nullptr)
        this->right_child->print_test();
    std::cout << this->stu_info.id;
    std::cout << "sb\n";
}//输出测试函数

const avl_tree* avl_tree::get_root() const
{
    const avl_tree *ans = this;
    while(ans->father != nullptr)
    {
        ans = ans->father;
    }
    return ans;
}

void avl_tree::update_pos(int height, int flag)
{
    if(height == 0)
        this->circle_pos = *(new position(600, 100, 20));
    else if(height == 1)
        this->circle_pos = *(new position(this->father->circle_pos.x + 20
                             + flag*(this->father->circle_pos.x + 20)/2,
                             this->father->circle_pos.y + 100, 20));
    else
    {
        int de_x = this->father->father->circle_pos.x
                - this->father->circle_pos.x;
        if(de_x < 0) de_x = -de_x;
        this->circle_pos = *(new position((this->father->circle_pos.x + 20)+
                                          flag*(de_x/2),
                                          this->father->circle_pos.y+100, 20));
    }
    if(this->left_child != nullptr)
        this->left_child->update_pos(height + 1, -1);
    if(this->right_child != nullptr)
        this->right_child->update_pos(height + 1, 1);
}//更新树节点和线的坐标

int avl_tree::fin_stu_info(my_data& stu) const
{
    if(this->stu_info.id == stu.id && this->stu_info.passward == stu.passward) return 1;
    if(this->left_child == nullptr && this->right_child == nullptr)
        return 0;
    if(this->left_child != nullptr && this->right_child != nullptr)
        return this->left_child->fin_stu_info(stu) + this->right_child->fin_stu_info(stu);
    else if(this->left_child != nullptr)
        return this->left_child->fin_stu_info(stu);
    else return this->right_child->fin_stu_info(stu);
}//是否找到学生信息

int avl_tree::fin_stu_info_without_passward(my_data& stu) const
{
    if(stu_info == stu) return 1;
    if(this->left_child == nullptr && this->right_child == nullptr)
        return 0;
    if(this->left_child != nullptr && this->right_child != nullptr)
        return this->left_child->fin_stu_info_without_passward(stu)
                + this->right_child->fin_stu_info_without_passward(stu);
    else if(this->left_child != nullptr)
        return this->left_child->fin_stu_info_without_passward(stu);
    else return this->right_child->fin_stu_info_without_passward(stu);
}//是否找到学生信息忽略密码

void avl_tree::read_from_file()
{
    QFile file(path);
    avl_tree *root = this;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString id, passward;
        while(!in.atEnd())
        {
            id = in.readLine();
            passward = in.readLine();
            root->insert_data(*(new my_data(id.toStdString(), passward.toStdString())));
            root = const_cast<avl_tree*>(get_root());
            root->update_pos();
        }
    }
}//文件读取操作

void avl_tree::print_to_file() const
{

    QFile file(path);
    if(file.open(QFile::Append))
    {
        QTextStream out(&file);
        if(this->stu_info.id != "")
            out << QString::fromStdString(stu_info.id) << '\n'
                << QString::fromStdString(stu_info.passward) << '\n';
        if(this->left_child != nullptr)
            this->left_child->print_to_file();
        if(this->right_child != nullptr)
            this->right_child->print_to_file();
    }
    file.close();
}//输出到文件
