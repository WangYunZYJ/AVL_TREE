#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "data.h"
#include <string>
#include "position.h"
#include <QString>
#include <fstream>

class avl_tree
{
public:
    avl_tree();
    avl_tree(const my_data& stu, avl_tree* temp_ptr);
    ~avl_tree();
    void insert_data(const my_data& stu); //插入节点
    void delete_data(std::string str); //删除节点
    void update_data(const my_data& stu); // 更新节点
    void print_test() const;
    void update_pos(int height = 0, int flag = 0);
    void update_height();
    const avl_tree* get_root() const;
    int fin_stu_info(my_data& stu) const;
    int fin_stu_info_without_passward(my_data& stu) const;
    void print_to_file() const; //打印到文件
    void read_from_file(); //从文件读取
    position circle_pos;
    avl_tree *left_child;
    avl_tree *right_child;
    my_data stu_info;
    QString path;
private:
    const avl_tree* search_left_child() const;
    const avl_tree* search_right_child() const;
    int find_height() const;
    void rotate_LL();
    void rotate_RR();
    void rorate_LR();
    void rotate_RL();
    void rotate();
    int find(avl_tree* tag) const;
    int balance;
    avl_tree *father;
};

#endif// AVL_TREE_H
