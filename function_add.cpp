#include"head.h"

std::string Upper(std::string input)
{
    //std::string x;//作为最后返回的大写模式

    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
            input[i] = input[i] + 'A' - 'a';
    }

    return input;
}

void MyCorrect(std::string input)
{
    //当已经确定输入有这个judge点错误的时候
    std::string standard[STANDARD] = {"CREATE","DROP","TABLE","INSERT","DELETE",
                                    "UPDATE","SELECT","QUIT","FROM","WHERE",
                                    "ORDER","BY","SET","ASC","DESC",
                                    "DISTINCT","INTO","VALUES"};
    
    for (int which_standard = 0; which_standard < STANDARD; which_standard++)//将所有的标准进行遍历
    {
        int weight = 0;
        for (int i = 0; i < input.length() && i < standard[which_standard].length(); i++)//将这个作为input遍历的点
        {
            if (input[i] == standard[which_standard][i])
                weight++;
        }
        //这时候要对权数进行计算
        if ((double)weight >= (double)(standard[which_standard].length() * 0.5))//这时候有超过50%的相似度
        {
            std::cout<<"您输入的“"<<input<<"”可能是想输入：“"<<standard[which_standard]<<"”"<<std::endl;
            return;
        }
    }

    return;//未完成
}

void Data_Table::compute(char *input)//计算功能的实现
//所希望的格式：COMPUTE 属性 FROM name TO 模式
{
    //这个时候的第一个单词“COMPUTE”已经进入了之前的判定
    //所以现在进入的是第二个单词

    //所希望的格式：COMPUTE 属性 FROM name TO 模式

    char *order1, *order2;
    std::string judge;
    std::string name;
    std::string which_get;//作为计算哪一个的依据
    std::vector<std::string> mode;
    Table *p = HEAD;

    order1 = OrderDivide(input, ' ', 1);
    which_get = input;//将属性写入
    order2 = OrderDivide(order1, ' ', 1);
    judge = order1;
    judge = Upper(judge);

    if (judge != "FROM")
    {
        std::cout<<"语法错误"<<std::endl;
        MyCorrect(judge);
        return;
    }

    order1 = OrderDivide(order2, ' ', 1);
    name = order2;

    order2 = OrderDivide(order1, ' ', 1);
    judge = order1;
    judge = Upper(judge);

    if (judge != "TO")
    {
        std::cout<<"语法错误"<<std::endl;
        MyCorrect(judge);
        return;
    }

    mode = ListDivide(order2, '|');

    //这时候所有的语法点是正确的

    for (; p != NULL; p = p->next)
        if (p->table_name == name)
            break;
    if (p == NULL)
    {
        std::cout<<"表“"<<name<<"”不存在"<<std::endl;
        return;
    }

    //这时候已经找到了这一个表

    int which = -1;//这里是标注出哪一个属性的数字

    for (int i = 0; i < p->table_head.size(); i++)
    {
        if (p->table_head[i] == which_get)
        {
            which = i;
            break;
        }
    }
    if (which == -1)//还是初始化的值
    {
        std::cout<<"属性“"<<which_get<<"”不存在"<<std::endl;
        return;
    }

    //这里就找到了哪一列的数据进行计算

    std::vector<double> data;//作为储存数据的容器
    double mid;//作为中间的变量进行操作

    for (int i = 0; i < p->all_data.size(); i++)
    {
        int point_count = 0;
        int j = 0;
        for (; j < p->all_data[i][which].length(); j++)
        {
            if (p->all_data[i][which][j] >= '0' && p->all_data[i][which][j] <= '9')
                continue;
            else if (p->all_data[i][which][j] == '.')
                    point_count++;
            else
                break;
        }
        if (point_count > 1 || j < p->all_data[i][which].length())
        {
            std::cout<<"该属性下的数据不全为数字！"<<std::endl;
            return;
        }
        else
        {
            mid = atof(p->all_data[i][which].c_str());//将这一个数据变成double
            data.push_back(mid);    
        }
    }

    //下面进行模式的判断


    for (int t = 0; t < mode.size(); t++)
    {
        if (mode[t] == "MAX")
        {
            double max = -10000000;
            for (int i = 0; i < data.size(); i++)
            {
                if (data[i] > max)
                    max = data[i];
            }
            std::cout<<which_get<<" 最大值："<<max<<std::endl;
        }

        else if (mode[t] == "MIN")
        {
            double min = 10000000;
            for (int i = 0; i < data.size(); i++)
            {
                if (data[i] < min)
                    min = data[i];
            }
            std::cout<<which_get<<" 最小值："<<min<<std::endl;
        }

        else if (mode[t] == "AVERAGE")
        {
            double sum = 0;
            double average = 0;
            for (int i = 0; i < data.size(); i++)
                sum += data[i];
            average = sum / data.size();
            std::cout<<which_get<<" 平均值："<<average<<std::endl;
        }

        else if (mode[t] == "VARIANCE")
        {
            double sum = 0;
            double average = 0;
            for (int i = 0; i < data.size(); i++)
                sum += data[i];
            average = sum / data.size();
            //先计算出这一组数据的平均值

            double variance = 0;//作为方差的储存值
            for (int i = 0; i < data.size(); i++)
                variance += (data[i] - average) * (data[i] - average);//算出没有进行除法之前的值
            variance = variance / data.size();

            std::cout<<which_get<<" 方差："<<variance<<std::endl;
        }

        else if (mode[t] == "RANGE")//极差的计算
        {
            double max = -10000000;
            for (int i = 0; i < data.size(); i++)
            {
                if (data[i] > max)
                    max = data[i];
            }

            double min = 10000000;
            for (int i = 0; i < data.size(); i++)
            {
                if (data[i] < min)
                    min = data[i];
            }

            double range = max - min;
            std::cout<<which_get<<" 极差："<<std::endl;
        }

        else
        {
            std::cout<<"没有“"<<mode[t]<<"”这一种模式"<<std::endl;
        }
    }

    return;
}

void Data_Table::search(char *input)
//所希望的格式：SEARCH 关键词 FROM name 属性 个数
{
    char *order1, *order2;
    std::string judge, name;
    std::string key;//作为搜索的关键词
    std::vector<std::string> which_to_search;//捕获最后的所有属性
    Table *p = HEAD;

    order1 = OrderDivide(input, ' ', 1);
    key = input;
    key = Upper(key);//强制转换为大写

    order2 = OrderDivide(order1, ' ', 1);
    judge = order1;
    judge = Upper(judge);
    if (judge != "FROM")
    {
        std::cout<<"语法错误"<<std::endl;
        MyCorrect(judge);
        return;
    }

    order1 = OrderDivide(order2, ' ', 1);
    name = order2;//抓取到表的名字
    order2 = OrderDivide(order1, ' ', 1);

    //order2 = OrderDivide(order1, ' ', 1);
    //std::cout<<order1<<std::endl;
    which_to_search = ListDivide(order1, '|');
    //得到所有属性的名字
    int num = atoi(order2);//作为一共有多少个搜索结果的标记
    std::cout<<num<<std::endl;

    for (; p != NULL; p = p->next)
        if (p->table_name == name)
            break;
    if (!p)
    {
        std::cout<<"表“"<<name<<"”不存在"<<std::endl;
        return;
    }
    //这就已经找到了这个表

    std::vector<int> which;//作为查找哪些属性的标记，int型
    for (int i = 0; i < which_to_search.size(); i++)
    {
        for (int j = 0; j < p->table_head.size(); j++)
        {
            if (p->table_head[j] == which_to_search[i])
                which.push_back(j);//将这个属性的位置压入
        }
    }
    if (which.size() < which_to_search.size())
    {
        std::cout<<"属性不对等"<<std::endl;
        return;
    }

    std::vector<int> which_to_print;//作为打印数据标记

    for (int condition = 10; which_to_print.size() < num && condition > 0; condition--)
    //这就是标记权的限度以及决定函数终止与否的函数
    {
        for (int i = 0; i < p->all_data.size(); i++)//对于原数据行的遍历
        {
            //std::cout<<condition<<" ";
            bool flag = true;
            for (int ii = 0; ii < which_to_print.size(); ii++)
            {
                if (which_to_print[ii] == i)//已经存在于这之中的一个数据
                {
                    flag = false;
                    break;
                }
            }
            if (!flag)
                continue;
            else
            //这时候就可以进行相似的比较
            {     
                //std::cout<<"?";           
                for (int j = 0; j < which.size(); j++)//作为每一列的遍历
                {
                    int like = 0;//标记一共有多少个相似的权
                    std::string mid = Upper(p->all_data[i][which[j]]);//将数据全部转化为大写
                    std::vector<int> record;//记录原来数据的哪些位次被记录了
                    for (int key_i = 0; key_i < key.length(); key_i++)//对关键词的每一个字符进行遍历
                    {
                        for (int data_i = 0; data_i < p->all_data[i][which[j]].length(); data_i++)//作为对原来的数据每一位进行的遍历
                        {
                            bool x = true;
                            for (int record_i = 0; record_i < record.size(); record_i++)
                            //观察这一位的字符是否经过了遍历
                            {
                                if (data_i == record[record_i])
                                {
                                    x = false;
                                    break;//这种情况就是说明已经重复
                                }
                            }
                            if (!x)
                                continue;
                            //下面的操作就是说明已经确定了这个字母没有被选中过
                            else
                            {
                                if (mid[data_i] == key[key_i])//如果这时候的这一位等于key中的某一位
                                {
                                    record.push_back(data_i);
                                    break;
                                }
                            }
                        }
                    }
                    like = record.size();
                    if ((double)like / (double)key.length() >= (double)condition / 10.0)//计算权比例是否超过预期
                    {
                        //std::cout<<(double)like / (double)key.length()<<"  "<<(double)condition / 10.0<<"|";
                        which_to_print.push_back(i);
                    }
                }
            }
        }
    }

    std::vector<std::vector<std::string> > data_to_print;//作为最后进行输出的数据表
    for (int i = 0; i < which_to_print.size(); i++)
    {
        data_to_print.push_back(p->all_data[which_to_print[i]]);
    }

    this->print_data(p->table_head, data_to_print);



    return;
}