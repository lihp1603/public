/*
 * 本文主要是提供产生随机数一类的算法函数
 * author:lihaiping1603@aliyun.com
 * date:2017-11-09 create
*/


#include "crandom.h"
#include <stdlib.h>
#include <time.h>

#include "../../Public/time/timeutils.h"
using namespace PublicTool;

namespace Algorithm{

    CRandom::CRandom()
    {

    }

    int CRandom::GetRandom(int low, int up)
    {
        //        //[low,up)产生这个区间内的随机数
        //要取得[a,b)的随机整数，使用(rand() % (b-a))+ a
        //通用公式:a + rand() % n；其中的a是起始值，n是整数的范围
        //设置随机数种子
        if(low==up)
            return low;
        //time只能精确到s
        //srand((unsigned)time(NULL));
        srand(TimeMicros());
        int rnum=(rand()%(up-low)) + low;
        return rnum;
    }

    void CRandom::GetRandomArray(set<int> &setArray, int nArraySize, int low, int up)
    {
        for(;setArray.size()<nArraySize;){
             int rnum=GetRandom(low,up);
             if(setArray.end()!=setArray.find(rnum))//说明已经存在这个随机数了
                  continue;
             setArray.insert(rnum);
        }
        return ;
    }

}

