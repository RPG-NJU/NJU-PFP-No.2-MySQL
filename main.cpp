#include<iostream>
#include<string>

#include<stdio.h>
#include<cstring>
#include"head.h"


using namespace std;


int main()
{
    char input[MAXLEN];
    char *secondinput;//作为二级输入的指针进行储存
    string judge;//作为判断第一个关键词的词储存

    char first[MAXLEN];
    string firstjudge;//决定是否进去

    cout<<"~$ ";
    cin>>first;//这是作为进入的判定

    firstjudge = first;

    firstjudge = Upper(firstjudge);

    if (firstjudge != "MYSQL")
    {
        cout<<"无法进入"<<endl;
        return 0;
    }
    
    getchar();

    
    cout<<"Welcome mySQL by GRP"<<endl;
    Data_Table data_table;
    cout<<"(mySQL)==> 初始化"<<std::endl;
    data_table.ini();
    do
    {
        
        cout<<"(mySQL)==>";
        cin.getline(input, MAXLEN, '\n');//默许的输入语句长度为1000;
        //cout<<input<<endl;//作为调试的语句

        secondinput = OrderDivide(input,' ',1);
        judge = input;
        judge = Upper(judge);
        //cout<<judge<<endl;

        if (judge == "CREATE")
            data_table.create(secondinput);
        else if (judge == "DROP")
            data_table.drop(secondinput);
        else if (judge == "TABLE")
            data_table.table_list(secondinput);
        else if (judge == "INSERT")
            data_table.insert(secondinput);
        else if (judge == "DELETE")
            data_table.my_delete(secondinput);
        else if (judge == "UPDATE")
            data_table.update(secondinput);
        else if (judge == "SELECT")
            data_table.select(secondinput);
        else if (judge == "COMPUTE")
            data_table.compute(secondinput);
        else if (judge == "SEARCH")
            data_table.search(secondinput);
        

        else if (judge != "QUIT")
            cout<<"语法错误!"<<endl;


    }while (strcmp(input,"quit") != 0);

    cout<<"~$ quit"<<endl;
    //getchar();getchar();
    return 0;
}
