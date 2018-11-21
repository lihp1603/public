/*
 * 主要记录一些常用的算法
 * author:lihaiping1603@aliyun.com
 * date:2017-11-23 create
*/


#ifndef __CMISC_ALGORITHM_H_
#define __CMISC_ALGORITHM_H_

#include <string>
#include <vector>

using namespace std;

namespace Algorithm{
    class CMiscAlgorithm
    {
    public:
        CMiscAlgorithm();
        //求最大公约数
        static int LargestCommonDivisor(int a,int b);
        //将字符串转化为16进制,并用字符串
        static string string_to_hex(const string& str); //transfer string to hex-string
        //transfer hex-string to bit
        static long long string_to_bit(const string& str);//transfer hex-string to bit
        /****************************************************************************
        函数名称: hex_to_str
        函数功能: 十六进制转字符串
        输入参数: ptr 字符串 buf 十六进制 len 十六进制字符串的长度。
        输出参数: 无
        *****************************************************************************/
        static void hex_to_str(char *ptr,unsigned char *buf,int len);

        //删除字符串首尾空格,\r,\t,\n等字符
        static string EraseSomeString(string strSrc);
        //洗牌算法
        static void ShuffleArray(int array[],int length);
        static void ShuffleVector(vector<int> &vecArray);
    };
}

#endif // CMISCALGORITHM_H
