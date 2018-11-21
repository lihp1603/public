#include "cmiscalgorithm.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

namespace Algorithm{
    CMiscAlgorithm::CMiscAlgorithm()
    {

    }

    int CMiscAlgorithm::LargestCommonDivisor(int m, int n)
    {
        /*辗转相除法求最大公约数 */
        int a=m;
        int b=n;
        int c=0;
        while(b!=0)  /* 余数不为0，继续相除，直到余数为0 */
         {
           c=a%b;
           a=b;
           b=c;
        }
        return a;
    }

    string CMiscAlgorithm::string_to_hex(const string& str) //transfer string to hex-string
    {
        //string result="0x";
        string result;
        string tmp;
        stringstream ss;
        for(int i=0;i<str.size();i++)
        {
            ss<<hex<<int(str[i])<<endl;
            ss>>tmp;
            result+=tmp;
        }
        return result;
    }


    long long CMiscAlgorithm::string_to_bit(const string& str)//transfer hex-string to bit
    {

        long long result=strtol(str.c_str(), NULL, 16);//第三个参数base为合法字符范围，base=2,为0、1，base=16，合法字符则为0-F，开头的0x自动忽略
        return result;
    }

    /****************************************************************************
    函数名称: hex_to_str
    函数功能: 十六进制转字符串
    输入参数: ptr 字符串 buf 十六进制 len 十六进制字符串的长度。
    输出参数: 无
    *****************************************************************************/
    void CMiscAlgorithm::hex_to_str(char *ptr,unsigned char *buf,int len)
    {
        for(int i = 0; i < len; i++)
        {
            sprintf(ptr, "%02x",buf[i]);
            ptr += 2;
        }
    }

    //删除首尾为
    string CMiscAlgorithm::EraseSomeString(string strSrc){
        string strDest=strSrc;
        size_t n = strDest.find_last_not_of( " \r\n\t" );
        if( n != string::npos )
        {
            strDest.erase( n + 1 , strDest.size() - n );
        }

        n = strDest.find_first_not_of ( " \r\n\t" );
        if( n != string::npos )
        {
            strDest.erase( 0 , n );
        }
        return strDest;
    }

    void CMiscAlgorithm::ShuffleArray(int array[], int length)
    {
        int index;
        int value;
        int median;

        if(NULL == array || 0 == length)
            return ;
        //设在随机数种子
        srand((unsigned)time(NULL));
        /* 发牌的时候对于已经分配的数据不再修改 */
        for(index = 0; index < length; index ++){
            value = index + rand() % (length - index);

            median = array[index];
            array[index] = array[value];
            array[value] = median;
        }
    }

    void CMiscAlgorithm::ShuffleVector(vector<int> &vecArray)
    {
        int index;
        int value;
        int median;
        int length=vecArray.size();
        if(length<=0)
            return ;
        //设在随机数种子
        srand((unsigned)time(NULL));
        /* 发牌的时候对于已经分配的数据不再修改 */
        for(index = 0; index < length; index ++){
            value = index + rand() % (length - index);

            median = vecArray[index];
            vecArray[index] = vecArray[value];
            vecArray[value] = median;
        }
    }
}
