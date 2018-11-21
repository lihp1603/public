/*
 * 本文主要是提供产生随机数一类的算法函数
 * author:lihaiping1603@aliyun.com
 * date:2017-11-09 create
*/

#ifndef __CRANDOM_H_
#define __CRANDOM_H_

#include <set>
using namespace std;

namespace Algorithm {
    class CRandom
    {
    public:
        CRandom();
        //[low,up]产生这个区间内的随机数算法
        static int GetRandom(int low,int up);
        //生成随机数组成的数组,用set防止重复随机数,同时可以对产生的随机数进行排序
        static void GetRandomArray(set<int>& setArray,int nArraySize,int low,int up);
    };
}


#endif // CRANDOM_H
