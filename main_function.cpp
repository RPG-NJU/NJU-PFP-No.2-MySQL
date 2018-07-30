#include"head.h"

//这个文件是跨cpp使用的函数的构造文件

char* OrderDivide(char *p,char key,int num)//改进之后加了一个num，这样子可以选择在第num个标识符进行切割
{
    int i = 0;
    //这是一个自定义的分割函数，相较于strtok会有一些自定义而带来的好处
    for (;*p!='\0';p++)
    {
        if (*p==key)
        {
            i++;
            if (i == num)
            {
               *p='\0';
               p++;
               break;
            }
        }
    }
    
    return p;//当切割失败的时候，会返回一个结束符，已经达到了判定的效果
}

std::vector<std::string> ListDivide(char *p, char key)
{
    std::vector<std::string> list;
    std::string mid;//作为中间的转换
    char *order = p;

    /*
    for (; *(pp+1) != '\0'; pp++);
    if (*pp == '\n')
    {
        *pp = '\0';
        std::cout<<"?";
    }
    */


    while(*order != '\0')
    {
        order = OrderDivide(order, key, 1);
        mid = p;
        for (int i = 0; i < mid.length(); i++)
        {
            if (mid[i] == '\n')
                mid[i] = ' ';
        }
        
        list.push_back(mid);//将这个属性压进容器
        p = order;
    }
    
    if (list[list.size()-1] == " ")
        list.pop_back();
    
    //if (list[list.size()-1][list[(list.size()-1)].length()-1] == '\n')
        //list[list.size()-1][list[(list.size()-1)].length()-1] = ' ';


    return list;
}

std::vector<std::string> ListDividePro(char *p, char key)
{
    std::vector<std::string> list;
    std::string mid;//作为中间的转换
    char *order = p;

    /*
    for (; *(pp+1) != '\0'; pp++);
    if (*pp == '\n')
    {
        *pp = '\0';
        std::cout<<"?";
    }
    */
    
    bool flag = true;

    while(*order != '\0')
    {
        order = OrderDivide(order, key, 1);
        p = p + 1;
        if (flag)
        {
            flag = false;
            p = p - 1;
        }
        mid = p;
        
        list.push_back(mid);//将这个属性压进容器
        p = order;
    }


    return list;
}