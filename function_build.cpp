#include"head.h"


Data_Table::Data_Table()
{
    HEAD = NULL;//将头指针设定为0，已满足之后的操作
    filename_head = "./DATA/";
    condition_filename = filename_head + "condition.txt";
    
}

void Data_Table::ini()//这是初始化数据库的操作
{
    Table *TAIL = HEAD;
    if (TAIL)
        for (; TAIL->next != NULL; TAIL = TAIL->next);
    FILE *file = fopen(condition_filename.c_str(), "r");
    if (file == NULL)
    {
        std::cout<<"维护文件打开失败"<<std::endl;
        return;
    }
    char str[MAXLEN];
    std::vector<std::string> mid;
    //作为中转储存

    fgets(str, MAXLEN, file);
    while (!feof(file))
    {
        mid = ListDivide(str, '|');
        condition.push_back(mid);//压入其中
        if (mid.size() < 2)
        {
            std::cout<<"维护文件损坏"<<std::endl;
            return;
        }
        mid.clear();
        fgets(str, MAXLEN, file);
    }

    fclose(file);

    //已经将文件读取进入

    for (int i = 0; i < condition.size(); i++)
    {
        //std::string name = condition[i][0];
        file = fopen(condition[i][1].c_str(), "r");

        //打开对应的文件

        if (file == NULL)
        {
            std::cout<<"文件打开失败"<<std::endl;
            return;
        }


        //这里是打开成功之后的操作

        Table *p = new Table;
        p->table_file_name = condition[i][1];
        p->table_name = condition[i][0];
        p->next = NULL;

        char file_get[MAXLEN];//将作为所有的文件读取的操作
        fgets(file_get, MAXLEN, file);
        //p->table_head.assign(ListDivide(file_get, ' ').begin(), ListDivide(file_get, ' ').end());//将第一行的表头写入节点
        p->table_head = ListDivide(file_get, ' ');       
        p->head_num = p->table_head.size();

        std::vector<std::string> data_get;
        fgets(file_get, MAXLEN, file);
        while (!feof(file))
        {

            //data_get.assign(ListDivide(file_get, ' ').begin(),ListDivide(file_get, ' ').end());
            data_get = ListDivide(file_get, ' ');

            p->all_data.push_back(data_get);//将新得到的数据压入容器

            data_get.clear();//清楚捕获区

            fgets(file_get, MAXLEN, file);


        }


        fclose(file);

        
        if (!HEAD)
            HEAD = p;
        else
            TAIL->next = p;
        TAIL = p;
        //将节点进行挂载
        this->print_head(p);
        //std::cout<<p->table_head[5];
    }
    return;
}

void Data_Table::condition_save()
{
    FILE *file = fopen(condition_filename.c_str(), "w");

    if (file == NULL)
    {
        std::cout<<"维护文件损坏，请检查"<<std::endl;
        return;
    }

    for (int i = 0; i < condition.size(); i++)
    {
        fprintf(file, "%s|%s|%s", condition[i][0].c_str(), condition[i][1].c_str(), "\n");
    }

    fclose(file);


    return;
}

void Data_Table::print_head(const Table *p) const
{
    std::vector<int> every_max;//作为每一列的最大长度的储存

    for (int i = 0; i < p->head_num; i++)
    {
        every_max.push_back(p->table_head[i].length());//作为初始长度
    }

    /*
    for (int i = 0; i < p->all_data.size(); i++)
    {
        for (int j = 0; j < p->head_num; j++)
        {
            if (p->all_data[i][j].length() > every_max[j])
                every_max[j] = p->all_data[i][j].length();
        }
    }
    */

    //已经知道了每一列的最大长度

    //下面打算先生成表的第一行边框

    std::cout<<"+--";
    std::cout<<"+";
    for (int i = 0; i < p->head_num; i++)
    {
        for (int j = 0; j < every_max[i]; j++)
            std::cout<<"-";
        std::cout<<"+";
    }
    std::cout<<std::endl;

    //这是上下边框的生成函数


    std::cout<<"|ID";
    std::cout<<"|";
    std::cout.setf(std::ios::left);//设定为左对齐
    for (int i = 0; i < p->head_num; i++)
    {
        std::cout<<std::setw(every_max[i])<<p->table_head[i];
        std::cout<<"|";
    }
    std::cout<<std::endl;

    //这是内容输出的函数

    std::cout<<"+--";
    std::cout<<"+";
    for (int i = 0; i < p->head_num; i++)
    {
        for (int j = 0; j < every_max[i]; j++)
            std::cout<<"-";
        std::cout<<"+";
    }
    std::cout<<std::endl;




    return;
}

void Data_Table::save(const Table *p) const
{
    FILE *file;//作为文件的指针操作


    file = fopen(p->table_file_name.c_str(), "w");
    if (!file)
    {
        std::cout<<"文件打开失败！"<<std::endl;
        return;
    }

    for (int i = 0; i < p->head_num; i++)
    {
        fprintf(file, "%s ", p->table_head[i].c_str());
    }
    fprintf(file, "%s", "\n");//加入回车符


    for (int i = 0; i < p->all_data.size(); i++)
    {
        for (int j = 0; j < p->head_num; j++)
        {
            fprintf(file, "%s ", p->all_data[i][j].c_str());
        }
        fprintf(file, "%s", "\n");//补充回车符
    }

    fclose(file);


    return;
}

void Data_Table::create(char *input)
{
    //std::cout<<"what"<<std::endl;//之前产生的测试语句

    Table *TAIL = HEAD; 
    if (TAIL)
        for (; TAIL->next != NULL; TAIL = TAIL->next);
    char *order1;
    char *order2;//使用两个指针进行交替的储存
    std::string judge;//用于一些关键词语句的判定   
    
    order1 = OrderDivide(input, ' ', 1);
    judge = input;
    judge = Upper(judge);
    //std::cout<<judge;
    if (judge != "TABLE")//不符合语法的终止
    {
        std::cout<<"语法错误"<<std::endl;
        MyCorrect(judge);
        return;
    }



    std::string name;//作为数据表的名字
    std::string filename;//作为打开的文件的名字
    std::vector<std::string> table_head_get;//从操作中得到的表头
    order2 = OrderDivide(order1, ' ', 1);
    name = order1;//将截取下来的片段进行记录

    //下面就要进行两种打开方式的分类

    if (*order2 == '(')//这就是新建数据表的形式
    {
        order2++;//将order2移过一个位置

        order1 = OrderDivide(order2, ')', 1);//将整个需要分析的语句截取出来
        if (*order1 != ' ')
        {
            std::cout<<"语法错误"<<std::endl;
            return;
        }
        else
            order1++;

        //至此，已经完全判定目前为止的语句为合乎语法的

        table_head_get = ListDivide(order2, ',');//从这里开始，order2解放

        order2 = OrderDivide(order1, ' ', 1);
        judge = order1;
        judge = Upper(judge);
        if (judge != "TO")
        {
            std::cout<<"语法错误"<<std::endl;
            MyCorrect(judge);
            return;
        }
        //对于TO语法的判定

        filename = order2;
        int len = filename.length();
        if (filename[len-4] != '.' || filename[len-3] != 't' || filename[len-2] != 'x' || filename[len-1] != 't' )
        {
            std::cout<<"文件名语法错误"<<std::endl;
            return;
        }

        //将文件名也捕获到
        
        //下面是链表节点的构造***

        Table *p = new Table;//建立一个新的数据模块
        p->table_file_name = filename_head + filename;
        p->table_name = name;
        p->table_head = table_head_get;
        p->head_num = table_head_get.size();
        p->next = NULL;
        //将前4个可以进行初始化的数据进行了初始化 

        //这就完成了一个节点的搭建

        //下面还需要写入一个文件

        FILE *file = fopen(p->table_file_name.c_str(), "w+");
        if (!file)
        {
            std::cout<<"文件打开失败！"<<std::endl;
            return;
        }

        //文件打开成功

        for (int i = 0; i < p->table_head.size(); i++)
            fprintf(file, "%s ", p->table_head[i].c_str());
        fprintf(file, "%c", '\n');

        fclose(file);

        if (!HEAD)
            HEAD = p;
        else
            TAIL->next = p;
        TAIL = p;

        this->print_head(p);

    }

    else//已有的数据表的挂载，这时候需要对name进行抓取并且进行之后的操作
    {    
        //order1 = OrderDivide(order2, ' ', 1);
        name = order1;//将表的名字抓取成功

        order1 = OrderDivide(order2, ' ', 1);
        judge = order2;//判定是否为FROM
        judge = Upper(judge);
        //std::cout<<judge;
        if (judge != "FROM")
        {
            std::cout<<"语法错误"<<std::endl;
            MyCorrect(judge);
            return;
        }

        //order1 = OrderDivide(order2, ' ', 1);
        filename = order1;//将文件名读入



        FILE *file = fopen((filename_head + filename).c_str(), "r+");
        if (!file)
        {
            std::cout<<"数据库文件打开失败"<<std::endl;
            return;
        }

        //以下就是文件打开成功之后的操作

        Table *p = new Table;
        p->table_file_name = filename_head + filename;
        p->table_name = name;
        p->next = NULL;


        char file_get[MAXLEN];//将作为所有的文件读取的操作
        fgets(file_get, MAXLEN, file);
        //p->table_head.assign(ListDivide(file_get, ' ').begin(), ListDivide(file_get, ' ').end());//将第一行的表头写入节点
        p->table_head = ListDivide(file_get, ' ');

        
        p->head_num = p->table_head.size();


        std::vector<std::string> data_get;
        fgets(file_get, MAXLEN, file);
        while (!feof(file))
        {

            //data_get.assign(ListDivide(file_get, ' ').begin(),ListDivide(file_get, ' ').end());
            data_get = ListDivide(file_get, ' ');

            p->all_data.push_back(data_get);//将新得到的数据压入容器

            data_get.clear();//清楚捕获区

            fgets(file_get, MAXLEN, file);


        }

        fclose(file);

        //将该链表的构造完成，除了基本数据

        //std::cout<<p->table_head[2];

        if (!HEAD)
            HEAD = p;
        else
            TAIL->next = p;
        TAIL = p;
        //将节点进行挂载

        this->print_head(p);

        //std::cout<<p->table_head[5];
        
    }

    condition.clear();

    for (Table *a = HEAD; a != NULL; a = a->next)
    {
        std::vector<std::string> mid;
        mid.push_back(a->table_name);
        mid.push_back(a->table_file_name);
        condition.push_back(mid);
        mid.clear();
    }

    this->condition_save();

    return;
}

void Data_Table::drop(char *input)
{
    //在原来的数据中已经有了定义了的HEAD和TAIL两个指针分别标记头尾

    std::string name, judge;
    char *order = OrderDivide(input, ' ', 1);
    judge = input;
    judge = Upper(judge);
    name = order;
    //分别判定语法和将表名进行抓取

    if (judge != "TABLE")
    {
        std::cout<<"语法错误"<<std::endl;
        MyCorrect(judge);
        return;
    }

    //对语句进行判定

    Table *current = HEAD, *previous = HEAD;

    if (current->table_name == name)//如果是头节点，进行一种特别的操作
    {
        HEAD = current->next;
        delete current;//对节点进行必要的释放
    }

    //下面就是非头结点的操作

    else
    {
    current = current->next; //previous = previous->next;//将头结点过掉
    for (;current != NULL;current = current->next, previous = previous->next)
    {
        if (current->table_name == name)
        {
            previous->next = current->next;
            delete current;
            break;
        }
    }
    }

    if (current == NULL)
    {
        std::cout<<"表“"<<name<<"”不存在"<<std::endl;
        return;
    }

    /*
    condition.clear();
    std::vector<std::string> mid;
    mid.clear();
    Table *p = HEAD;
    for (; p != NULL; p = p->next)
    {
        //std::vector<std::string> mid;
        mid.push_back(p->table_name);
        //std::cout<<a->table_file_name<<std::endl;
        //getchar();
        mid.push_back(p->table_file_name);
        //mid.push_back("./DATA/what.txt");
        condition.push_back(mid);
        mid.clear();
    }
    */

    for (int i = 0; i < condition.size(); i++)
    {
        if (condition[i][0] == name)
        {
            if (i == condition.size() - 1)
            {
                condition.pop_back();
                break;
            }
            else
            {
                for (int ii = i; ii < condition.size() - 1; ii++)
                {
                    condition[i] = condition[i + 1];
                }
                condition.pop_back();
                break;
            }
        }
    }
    this->condition_save();

    

    return;
}

void Data_Table::table_list(char *input)
{
    std::string judge;

    judge = input;//这里应该只有一个参数
    judge = Upper(judge);

    if (judge != "LIST")
    {
        std::cout<<"语法错误"<<std::endl;
        MyCorrect(judge);
        return;
    }

    //以下就是正确的操作

    int total_num = 0;
    Table *p = HEAD;

    for (; p != NULL; p = p->next)
        total_num++;
    //这里就找到了一共有多少个表的问题

    p = HEAD;//再将指针调回头指针

    std::cout<<"  total:"<<total_num<<std::endl;
    //system("pause");

    for (; p != NULL; p = p->next)
    {
        std::cout<<"    ";
        std::cout<<p->table_name<<":";
        std::cout<<"("<<p->head_num<<","<<p->all_data.size()<<")";
        std::cout<<"[";
        for (int i = 0; i < p->head_num - 1; i++)
        {
            std::cout<<p->table_head[i]<<",";
        }
        std::cout<<p->table_head[p->head_num - 1]<<"]"<<std::endl;
    }


    return;
}

//2018.5.5晚上20.21以上测试正确

void Data_Table::insert(char *input)
{
    char *order1, *order2;//作为两个标志指针
    std::string judge, name;//作为两个判定需求
    Table *p = HEAD;//表的指针

    order1 = OrderDivide(input, ' ', 1);
    judge = input;
    judge = Upper(judge);

    if (judge != "INTO")
    {
        std::cout<<"语法错误"<<std::endl;
        MyCorrect(judge);
        return;
    }

    order2 = OrderDivide(order1, ' ',1);
    name = order1;//将表的名字进行抓取


    for (; p != NULL; p = p->next)
    {
        if (p->table_name == name)
            break;
    }

    if (p == NULL)
    {
        std::cout<<"表“"<<name<<"”不存在"<<std::endl;
        return;
    }

    //这一段代码可以趁早止损

    if (*order2 == '(')//如果是复杂的选择属性加入法
    {
        std::vector<std::string> add;//这里就是表示添加的信息流

        order2++;
        order1 = OrderDivide(order2, ')', 1);//将整个需要分析的语句提取出来
        if (*order1 != ' ')
        {
            std::cout<<"语法错误"<<std::endl;
            return;
        }
        

        std::vector<std::string> add_head = ListDivide(order2, ',');
        //已经将需要的添加的表头进行了抓取

        order1++;
        order2 = OrderDivide(order1, ' ', 1);
        judge = order1;
        judge = Upper(judge);
        if (judge != "VALUES" || *order2 != '(')
        {
            std::cout<<"语法错误"<<std::endl;
            MyCorrect(judge);
            return;
        }
        else
            order2++;//移过括号的位置

        order1 = OrderDivide(order2, ')', 1);
        //这里没有考虑在这之后还有文字的情况××××××××××××××××××××××××××××××××××
        
        std::vector<std::string> add_data = ListDivide(order2, ',');

        //已经将需要的数据全部抓取完毕！

        for (int i = 0; i < p->head_num; i++)
        {
            add.push_back("NULL");//将每一个位置进行初始化
        }


        int i = 0;//作为标记总遍历的标记
        int j = 0;//作为抓取数据的遍历
        int data_max = add_data.size();
        int head_max = add_head.size();

        if (data_max != head_max)
        {
            std::cout<<"属性与数据不对应"<<std::endl;
            return;
        }

        for (; i < p->head_num; i++)
        {
            if (p->table_head[i] == add_head[j])
            {
                add[i] = add_data[j];
                j++;//将指针指向下一位
            }
        }

        if (j != head_max)
        {
            std::cout<<"添加表的属性与原属性不相符"<<std::endl;
            return;
        }

        p->all_data.push_back(add);//将这一项压入表


        add.clear();

        this->save(p);

        this->print_data(p->table_head, p->all_data);

    }

    else//如果是简单的全插入
    {
        order1 = OrderDivide(order2, ' ', 1);
        judge = order2;//抓取判定
        judge = Upper(judge);

        if (judge != "VALUES" || *order1 != '(')
        {
            std::cout<<"语法错误"<<std::endl;
            MyCorrect(judge);
            return;
        }
        order1++;

        order2 = OrderDivide(order1, ')', 1);

        std::vector<std::string> add = ListDivide(order1, ',');

        if (add.size() != p->head_num)
        {
            std::cout<<"数据参数过少"<<std::endl;
            return;
        }

        //已经确认过眼神，是对的人

        p->all_data.push_back(add);//进行压入

        this->save(p);

        add.clear();
        this->print_data(p->table_head, p->all_data);

        return;
    }


    return;
}

void Data_Table::my_delete(char *input)
{
    char *order1, *order2;//作为命令的记录
    std::string name, judge;

    order1 = OrderDivide(input, ' ', 1);
    judge = input;
    judge = Upper(judge);

    if (judge == "*")
    {
        order2 = OrderDivide(order1, ' ', 1);
        judge = order1;
        name = order2;
        judge = Upper(judge);

        if (judge != "FROM")
        {
            std::cout<<"语法错误"<<std::endl;
            MyCorrect(judge);
            return;
        }

        Table *p = HEAD;
        for (; p != NULL; p = p->next)
        {
            if (p->table_name == name)
                break;
        }

        if (!p)
        {
            std::cout<<"表“"<<name<<"”不存在"<<std::endl;
            return;
        }

        //下面就是确认过眼神

        p->all_data.clear();

        this->save(p);
        this->print_data(p->table_head, p->all_data);

        return;
    }

    else if (judge == "FROM")
    {
        order2 = OrderDivide(order1, ' ', 1);
        name = order1;


        Table *p = HEAD;
        for (; p != NULL; p = p->next)
        {
            if (p->table_name == name)
                break;
        }

        if (!p)
        {
            std::cout<<"表“"<<name<<"”不存在"<<std::endl;
            return;
        }

        //这样子就确定找到了这个表的指针p


        order1 = OrderDivide(order2, ' ', 1);
        judge = order2;
        judge = Upper(judge);

        if (judge != "WHERE")
        {
            std::cout<<"语法错误"<<std::endl;
            MyCorrect(judge);
            return;
        }

        std::string which, what;//两个需要的参数

        order2 = OrderDivide(order1, ' ', 1);
        which = order1;
        order1 = OrderDivide(order2, ' ', 1);
        judge = order2;
        what = order1;
        if (judge != "=")
        {
            std::cout<<"语法错误，缺少关键词“=”"<<std::endl;
            return;
        }

        //这时候的两个参数都已经浮出水面

        int j = 0;
        for (; j < p->head_num; j++)
        {
            if (which == p->table_head[j])
                break;
        }
        if (j == p->head_num)
        {
            std::cout<<"没有这一个属性"<<std::endl;
            return;
        }

        //找到了属性的位置

        for (int i = 0; i < p->all_data.size(); i++)//对于数据进行遍历
        {
            if (p->all_data[i][j] == what)
            {
                if (i == p->all_data.size() - 1)
                {
                    p->all_data.pop_back();//这是如果是最后一个数据的话，就进行这种操作
                }

                else
                {
                    for (int ii = i; ii < p->all_data.size() - 1; ii++)
                    {
                        p->all_data[ii] = p->all_data[ii+1];
                    }
                    p->all_data.pop_back();//将最后一个节点进行删除
                }
            }
        }

        this->save(p);

        this->print_data(p->table_head, p->all_data);

        return;
    }

    else
    {
        std::cout<<"语法错误"<<std::endl;
        return;
    }

    return;
}

void Data_Table::print_data(const std::vector<std::string> head, const std::vector<std::vector<std::string> > data) const
{
    int ID_len = 5;
    int ID = 1;
    std::cout.setf(std::ios::left);//设定为左对齐
    std::vector<int> every_max;//作为每一列的最大长度的储存

    for (int i = 0; i < head.size(); i++)
    {
        every_max.push_back(head[i].length());//作为初始长度
    }

    for (int i = 0; i <data.size(); i++)
    {
        for (int j = 0; j < head.size(); j++)
        {
            if (data[i][j].length() > every_max[j])
                every_max[j] = data[i][j].length();
        }
    }

    //这样子就知道了每一列的最大长度

    std::cout<<"+";
    for (int i = 0; i < ID_len; i++)
        std::cout<<"-";
    std::cout<<"+";
    for (int i = 0; i < head.size(); i++)
    {
        for (int j = 0; j < every_max[i]; j++)
            std::cout<<"-";
        std::cout<<"+";
    }
    std::cout<<std::endl;//这就是一个完整的边框输出****************************

    std::cout<<"|";
    std::cout<<std::setw(ID_len)<<"ID";
    std::cout<<"|";
    for (int i = 0; i < head.size(); i++)
    {
        std::cout<<std::setw(every_max[i])<<head[i];
        std::cout<<"|";
    }
    std::cout<<std::endl;
    //这是表头的输出********************************

    std::cout<<"+";
    for (int i = 0; i < ID_len; i++)
        std::cout<<"-";
    std::cout<<"+";
    for (int i = 0; i < head.size(); i++)
    {
        for (int j = 0; j < every_max[i]; j++)
            std::cout<<"-";
        std::cout<<"+";
    }
    std::cout<<std::endl;//这就是一个完整的边框输出****************************

    for (int i = 0; i < data.size(); i++)
    {
        std::cout<<"|";
        std::cout<<std::setw(ID_len)<<ID;
        std::cout<<"|";

        for (int j = 0; j < head.size(); j++)
        {
            if (data[i][j] == "NULL")
                std::cout<<std::setw(every_max[j])<<" ";
            else
                std::cout<<std::setw(every_max[j])<<data[i][j];
            std::cout<<"|";
        }
        ID++;
        std::cout<<std::endl;
    }

    std::cout<<"+";
    for (int i = 0; i < ID_len; i++)
        std::cout<<"-";
    std::cout<<"+";
    for (int i = 0; i < head.size(); i++)
    {
        for (int j = 0; j < every_max[i]; j++)
            std::cout<<"-";
        std::cout<<"+";
    }
    std::cout<<std::endl;//这就是一个完整的边框输出****************************


    return;
}

void Data_Table::update(char *input)
{
    char *order1, *order2;
    std::string name, judge;
    Table *p = HEAD;

    //更新表的操作

    order1 = OrderDivide(input, ' ', 1);
    order2 = OrderDivide(order1, ' ', 1);//order2指向数据抓取的开始

    judge = order1;
    name = input;
    judge = Upper(judge);

    if (judge != "SET")
    {
        std::cout<<"语法错误，缺少关键词“SET”"<<std::endl;
        MyCorrect(judge);
        return;
    }

    for (; p != NULL; p = p->next)
    {
        if (p->table_name == name)
            break;
    }
    if (!p)
    {
        std::cout<<"表“"<<name<<"”不存在"<<std::endl;
        return;
    }

    //找到了这个表的情况


    //order2 = OrderDivide(order1, ' ', 1);

    //这里需要一个自己定义的新函数去分割这个字符串看是否存在WHERE关键词*****************************
    bool flag = false;//作为是否是复杂情况的标志
    for (order1 = order2; *(order1+4) != '\0'; order1++)
    {
        if (*order1 == 'W' && *(order1 + 1) == 'H' && *(order1 + 2) == 'E' && *(order1 + 3) == 'R' && *(order1 + 4) == 'E')
        {
            *(order1 - 1) = '\0';
            //这时候order1指向W，可以作为下一次的判定
            flag = true;
            break;
        }
    }

    std::vector<std::string> column_and_data = ListDividePro(order2, ',');//将每一个属性和数据的赋值数据抓取进来
    int num = column_and_data.size();//知道一共有多少个数据组

    std::vector<std::string> column;
    std::vector<std::string> data;
    std::vector<std::string> mid;//作为一个中间的储存

    for (int i = 0; i < num; i++)
    {
        char *p = &column_and_data[i][0];//将第一个字符给p进行赋值
        //std::cout<<p<<std::endl;
        mid = ListDivide(p, ' ');
        if (mid.size() != 3)
        {
            std::cout<<"属性与相关数据不对等"<<std::endl;
            return;
        }
        if (mid[1] != "=")
        {
            std::cout<<"缺少语法符“=”"<<std::endl;
            return;
        }

        //下面的操作就是确认过眼神，是对的人

        column.push_back(mid[0]);
        data.push_back(mid[2]);

        //就已经将所需要的这一组数据进行了压入

        mid.clear();

    }

    std::vector<int> which_to_update;//用这个变量和这之后的操作来进行找出哪几个属性需要替换
    for (int i = 0; i < column.size(); i++)
    {
        for (int j = 0; j < p->head_num; j++)
        {
            if (p->table_head[j] == column[i])
            {
                which_to_update.push_back(j);
                break;
            }
        }
    }

    if (flag)//如果是指定位置的update
    {
        order2 = OrderDivide(order1, ' ', 1);
        judge = order1;
        judge = Upper(judge);
        //std::cout<<order2;
        if (judge != "WHERE")
        {
            std::cout<<"语法错误，缺少WHERE"<<std::endl;
            MyCorrect(judge);
            return;
        }

        //下面就是对于条件限定的指派

        std::vector<std::string> which_get = ListDivide(order2, ' ');

        if (which_get.size() != 3)
        {
            std::cout<<"参数不足"<<std::endl;
            return;
        }

        if (which_get[1] != "=")
        {
            std::cout<<"语法错误，缺少关键词“=”"<<std::endl;
            return;
        }

        //这里的所有的判定都已经结束，没有了什么问题

        std::string which_head = which_get[0];
        std::string which_data = which_get[2];

        int which_if;//作为判定条件是哪一个位置的记录

        int which_int = 0;
        for (int which_int = 0; which_int < p->head_num; which_int++)
        {
            if (p->table_head[which_int] == which_head)
            {
                which_if = which_int;
                break;//一定要跳出
            }
        }
        if (which_int == p->head_num)
        {
            std::cout<<"WHERE所指条件不存在"<<std::endl;
            return;
        }

        for (int i = 0; i < p->all_data.size(); i++)
        {
            if (p->all_data[i][which_if] == which_data)//如果是符合WHERE的条件的数据条
            {
                for (int j = 0; j < which_to_update.size(); j++)
                {
                    //std::cout<<"?";
                    p->all_data[i][which_to_update[j]] = data[j];
                }
            }
        }
        //以上所期许的应该是将符合的数据都进行了更新
    }

    else
    {
        for (int i = 0; i < p->all_data.size(); i++)
        {
            for (int j = 0; j < which_to_update.size(); j++)
            {
                p->all_data[i][which_to_update[j]] = data[j];
            }
        }
    }

    this->save(p);

    this->print_data(p->table_head, p->all_data);

    std::cout<<"    UPDATE成功"<<std::endl;

    return;
}



int Data_Table::compare(std::vector<std::string> a, std::vector<std::string> b, int which)
{
    //bool flag;
    return (strcmp(a[which].c_str(), b[which].c_str()) > 0);
}

void Data_Table::swap(std::vector<std::string> &a, std::vector<std::string> &b)
{
    std::vector<std::string> mid = a;
    a = b;
    b = mid;
    return;
}


void Data_Table::select(char *input)
{
    char *order1, *order2;
    std::string name, judge;
    order1 = OrderDivide(input, ' ', 1);
    judge = input;
    judge = Upper(judge);
    Table *p = HEAD;
    bool flag = false;
    std::vector<std::vector<std::string> > data;
    std::vector<std::string> data_head;
    char *to_file;
    std::vector<std::vector<std::string> > mid_data;

    if (judge == "*")//这就是全部输出的模式
    {
        order2 = OrderDivide(order1, ' ', 1);
        judge = order1;
        judge = Upper(judge);
        if (judge != "FROM")
        {
            std::cout<<"语法错误，缺少关键词“FROM”"<<std::endl;
            MyCorrect(judge);
            return;
        }

        order1 = OrderDivide(order2, ' ',1);
        name = order2;

        for (; p != NULL; p = p->next)
        {
            if (p->table_name == name)
                break;
        }
        if (!p)
        {
            std::cout<<"表“"<<name<<"”不存在"<<std::endl;
            return;
        }

        //已经找到了正确的节点


        if (*order1 != 'O')
        {
        //*************************************************************
        if (*order1 == 'W')
        {
            //这里就是简单的部分展示
            order2 = OrderDivide(order1, ' ', 1);
            judge = order1;
            judge = Upper(judge);
            if (judge != "WHERE")
            {
                std::cout<<"语法错误，缺少关键词“WHERE”"<<std::endl;
                MyCorrect(judge);
                return;
            }

            order1 = order2;
            order2 = OrderDivide(order1, ' ', 3);
            std::vector<std::string> column = ListDivide(order1, ' ');
            //std::cout<<order1<<std::endl;
            if (column.size() != 3)
            {
                std::cout<<"参数不足"<<std::endl;
                return;
            }
            if (column[1] != "=")
            {
                std::cout<<"语法错误，缺少关键词“=”"<<std::endl;
                return;
            }

            int which = -1;
            for (int i = 0; i < p->head_num; i++)
            {
                if (column[0] == p->table_head[i])
                {
                    which = i;
                        break;
                }
            }
            if (which == -1)
            {
                std::cout<<"属性值有偏差"<<std::endl;
                return;
            }

            for (int i = 0; i < p->all_data.size(); i++)
            {
                if (p->all_data[i][which] == column[2])
                {
                    data.push_back(p->all_data[i]);
                    //std::cout<<data[0][0]<<std::endl;
                }
            }
            data_head = p->table_head;

                //已经转站完毕
            if (*order2 == 'T')
            {
                flag = true;
                to_file = order2;
            }            
        }

        //*************************************************************

        else//(*order1 == '\0' || *order1 == 'T')//如果名字之后是空，或者是tofile
        {
            data = p->all_data;
            data_head = p->table_head;
            if (*order1 == 'T')
            {
                to_file = order1;
                flag = true;
            }
        }

        this->print_data(data_head, data);
        }

        else//这时候是排序的操作！！！！！！！！！！！！！！！！！！！！！！！！！！
        {
            order2 = OrderDivide(order1, ' ', 2);
            judge = order1;
            judge = Upper(judge);
            if (judge != "ORDER BY")
            {
                std::cout<<"语法错误，缺少关键词ORDER BY"<<std::endl;
                MyCorrect(judge);
                return;
            }

            order1 = OrderDivide(order2, ' ', 1);
            std::vector<std::string> column_get = ListDivide(order2, ',');
            order2 = OrderDivide(order1, ' ', 1);
            std::vector<std::string> mode_get = ListDivide(order1, '|');
            data_head = p->table_head;
            std::vector<std::vector<std::string> > mid_data;

            //*****************************************************
            if (*order2 == 'W')
            {
                //这里就是简单的部分展示
                order1 = OrderDivide(order2, ' ', 1);
                judge = order2;
                judge = Upper(judge);
                if (judge != "WHERE")
                {
                    std::cout<<"语法错误，缺少关键词“WHERE”"<<std::endl;
                    MyCorrect(judge);
                   return;
                }

                order2 = OrderDivide(order1, ' ', 3);
                std::vector<std::string> column = ListDivide(order1, ' ');
                if (column.size() != 3)
                {
                    std::cout<<"参数不足"<<std::endl;
                    return;
                }
                if (column[1] != "=")
                {
                    std::cout<<"语法错误，缺少关键词“=”"<<std::endl;
                    return;
                }

                int which = -1;
                for (int i = 0; i < p->head_num; i++)
                {
                    if (column[0] == p->table_head[i])
                    {
                        which = i;
                            break;
                    }
                }
                if (which == -1)
                {
                    std::cout<<"属性值有偏差"<<std::endl;
                    return;
                }

                for (int i = 0; i < p->all_data.size(); i++)
                {
                    if (p->all_data[i][which] == column[2])
                        data.push_back(p->all_data[i]);
                }

                //已经转站完毕

                if (*order2 == 'T')
                {
                    flag = true;
                    to_file = order2;
                }            
            }

            //**********************************************************
            else
            {
                if (*order2 == 'T')
                {
                    flag = true;
                    to_file = order2;
                }
                data = p->all_data;
            }
            

            std::vector<int> which;
            std::vector<int> mode;
            //分别作为哪一列和哪一种模式的标志

            for (int i = 0; i < column_get.size(); i++)
            {
                for (int j = 0; j < p->head_num; j++)
                {
                    if (p->table_head[j] == column_get[i])
                    {
                        which.push_back(j);
                        break;
                    }
                }
            }
            if (which.size() != column_get.size())
            {
                std::cout<<"存在未知属性"<<std::endl;
                return;
            }

            for (int i = 0; i < mode_get.size(); i++)
            {
                if (mode_get[i] == "ASC")
                    mode.push_back(2);
                else if (mode_get[i] == "DESC")
                    mode.push_back(1);
                else
                {
                    std::cout<<"未知的模式"<<std::endl;
                    return;
                }
            }
            if (mode.size() != which.size())
            {
                std::cout<<"参数不足"<<std::endl;
                return;
            }
            
            //在这之上，which按顺序进行排列，true为升序

            for (int t = 0; t < mode.size(); t++)//总的排序次数
            {
                //std::cout<<"?";
                for (int secondtime = 0; secondtime < data.size(); secondtime++)//冒泡的次数
                {
                    for (int i = data.size() - 1; i > 0; i--)//这时候需要的是第i和第i-1
                    {
                        int check = 0;
                        for (; check < t; check++)
                        {
                            //std::cout<<"?";
                            if (this->compare(data[i], data[i-1], which[check]) != 0)
                            {
                                //std::cout<<"?";
                                break;
                            }
                        }
                        if (check == t)//前面的属性都一样，进行后一个属性的排序
                        {
                            if (mode[t] == 1)//升序
                            {
                                if (this->compare(data[i], data[i-1], which[t]) > 0)
                                    this->swap(data[i], data[i-1]);
                            }
                            else
                            {
                                if (this->compare(data[i], data[i-1], which[t]) < 0)
                                    this->swap(data[i], data[i-1]);
                            }
                        }
                    }
                }
            }

            this->print_data(data_head, data);

        }

    }
    //这之中有return；

    else if (Upper(judge) == "DISTINCT")
    {
        order2 = OrderDivide(order1, ' ', 1);
        std::vector<std::string> head_get = ListDivide(order1, ',');//将所有的属性读入
        order1 = OrderDivide(order2, ' ', 1);
        judge = order2;
        judge = Upper(judge);
        if (judge != "FROM")
        {
            std::cout<<"语法错误，缺少关键词“FROM”"<<std::endl;
            MyCorrect(judge);
            return;
        }

        order2 = OrderDivide(order1, ' ', 1);
        name = order1;
        order1 = OrderDivide(order2, ' ', 1);

        for (; p != NULL; p = p->next)
        {
            if (p->table_name == name)
                break;
        }
        if (!p)
        {
            std::cout<<"表“"<<name<<"”不存在"<<std::endl;
            return;
        }

        std::vector<int> head;
        for (int i = 0; i < head_get.size(); i++)
        {
            for (int j = 0; j < p->head_num; j++)
            {
                if (p->table_head[j] == head_get[i])
                {
                    head.push_back(j);//将位置进行记录
                    break;
                }
            }
        }
        if (head.size() != head_get.size())
        {
            std::cout<<"属性不对等"<<std::endl;
            return;
        }

        if (*order1 != '\0')
        {
            flag = true;
            to_file = order1;
        }

        data_head = head_get;//这个作为表头
        for (int i = 0; i < p->all_data.size(); i++)
        {
            bool x = true;
            for (int j = 0; j < data.size(); j++)//对于已有的data进行遍历
            {
                int which = 0;
                for (; which < head.size(); which++)
                {
                    if (data[j][which] != p->all_data[i][head[which]])
                        break;
                }
                if (which == head.size())
                {
                    x = false;
                    break;
                }
            }

            if (x)
            {
                std::vector<std::string> mid;
                for (int which = 0; which < head.size(); which++)
                {
                    mid.push_back(p->all_data[i][head[which]]);
                }
                data.push_back(mid);
                mid.clear();
            }
        }

        this->print_data(data_head,data);

    }

    else//这时候是input就是属性的开始，order1已经越过了所有的属性
    {
        std::vector<std::string> head_get = ListDivide(input, ',');
        //将属性领入其中
        order2 = OrderDivide(order1, ' ', 1);
        judge = order1;
        judge = Upper(judge);
        if (judge != "FROM")
        {
            std::cout<<"语法错误，缺少关键词“FROM”"<<std::endl;
            MyCorrect(judge);
            return;
        }

        order1 = OrderDivide(order2, ' ', 1);
        name = order2;//将名字进行捕获

        for (; p != NULL; p = p->next)
        {
            if (p->table_name == name)
                break;
        }
        if (!p)
        {
            std::cout<<"表“"<<name<<"”不存在"<<std::endl;
            return;
        }

        std::vector<int> head;

        for (int i = 0; i < head_get.size(); i++)
        {
            for (int j = 0; j < p->table_head.size(); j++)
            {
                if (p->table_head[j] == head_get[i])
                {
                    head.push_back(j);
                    break;
                }
            }
        }
        if (head.size() != head_get.size())
        {
            std::cout<<"属性不对等"<<std::endl;
            return;
        }

        data_head = head_get;

        if (*order1 == 'W')
        {
            //这里就是简单的部分展示
            order2 = OrderDivide(order1, ' ', 1);
            judge = order1;
            judge = Upper(judge);
            if (judge != "WHERE")
            {
                std::cout<<"语法错误，缺少关键词“WHERE”"<<std::endl;
                MyCorrect(judge);
                return;
            }

            order1 = OrderDivide(order2, ' ', 3);
            std::vector<std::string> column = ListDivide(order2, ' ');
            if (column.size() != 3)
            {
                std::cout<<"参数不足"<<std::endl;
                return;
            }
            if (column[1] != "=")
            {
                std::cout<<"语法错误，缺少关键词“=”"<<std::endl;
                return;
            }

            int which = -1;
            for (int i = 0; i < p->head_num; i++)
            {
                if (column[0] == p->table_head[i])
                {
                    which = i;
                    break;
                }
            }
            if (which == -1)
            {
                std::cout<<"属性值有偏差"<<std::endl;
                return;
            }

            for (int i = 0; i < p->all_data.size(); i++)
            {
                if (p->all_data[i][which] == column[2])
                    mid_data.push_back(p->all_data[i]);
            }

            //已经转站完毕

            if (*order1 == 'T')
            {
                flag = true;
                to_file = order1;
            }            
        }

        else
        {
            mid_data = p->all_data;
            
            if (*order1 == 'T')
            {
                flag = true;
                to_file = order1;
            }
        }

        for (int i = 0; i < mid_data.size(); i++)
        {
            std::vector<std::string> mid;            
            for (int j = 0; j < head.size(); j++)
            {
                mid.push_back(mid_data[i][head[j]]);
            }
            data.push_back(mid);
            mid.clear();
        }
        //将所有的东西进行压入

        this->print_data(data_head, data);
    }

    //下面有一个文件的写入这个小操作



    if (flag)
    {
        order1 = OrderDivide(to_file, ' ', 1);
        judge = to_file;
        judge = Upper(judge);
        if (judge != "TO")
        {
            std::cout<<"语法错误，缺少关键词“TO“"<<std::endl;
            MyCorrect(judge);
            return;
        }

        std::string filename = order1;

        this->save_pro(data_head, data, filename);
        std::cout<<"文件写入成功"<<std::endl;

    }



    return;
}

void Data_Table::save_pro(const std::vector<std::string> head, const std::vector<std::vector<std::string> > data,const std::string filename) const
{
    FILE *file;//作为文件的指针操作


    file = fopen((filename_head + filename).c_str(), "w");
    if (!file)
    {
        std::cout<<"文件打开失败！"<<std::endl;
        return;
    }

    for (int i = 0; i < head.size(); i++)
    {
        fprintf(file, "%s ", head[i].c_str());
    }
    fprintf(file, "%s", "\n");//加入回车符


    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < head.size(); j++)
        {
            fprintf(file, "%s ", data[i][j].c_str());
        }
        fprintf(file, "%s", "\n");//补充回车符
    }

    fclose(file);


    return;
    }

