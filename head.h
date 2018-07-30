#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<cstdio>
#include<stdlib.h>
#include<iomanip>//作为setw设定对齐的头文件
#include<cstring>


#define MAXLEN 1000
#define STANDARD 18


char* OrderDivide(char *p, char key, int num);

std::vector<std::string> ListDivide(char *p, char key);

std::vector<std::string> ListDividePro(char *p, char key);

std::string Upper(std::string input);

void MyCorrect(std::string input);

struct Table
{
    std::string table_file_name;
    std::string table_name;
    std::vector<std::string> table_head;
    int head_num;//作为属性的数目
    std::vector<std::vector<std::string> > all_data;
    Table *next;
    
};

class Data_Table
{
private:
    std::vector<std::vector<std::string> > condition;
    std::string condition_filename;
    //前两个是对于数据库的维护文件的相关操作
    std::string filename_head;//用于文件名的头
    Table *HEAD;
public:
    Data_Table();
    void ini();
    void condition_save();
    void save(const Table *p) const;
    void save_pro(const std::vector<std::string> head, const std::vector<std::vector<std::string> > data,const std::string filename) const;
    void print_head(const Table *p) const;
    void create(char *input);
    void drop(char *input);
    void table_list(char *input);
    void insert(char *input);
    void my_delete(char *input);
    void print_data(const std::vector<std::string> head, const std::vector<std::vector<std::string> > data) const;
    void update(char *input);
    void select(char *input);
    int compare(std::vector<std::string> a, std::vector<std::string> b, int which);
    void swap(std::vector<std::string> &a, std::vector<std::string> &b);
    void compute(char *inupt);
    void search(char *input);
};