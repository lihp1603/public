
#include "DynamicPlanningAlgorithm.h"
#include <vector>
#include <map>
#include <string>
#include <cstring>

#include "../../Public/loger/simpleLoger.h"

using namespace std;


namespace Algorithm {
    //
    DynamicPlanningAlgorithm::DynamicPlanningAlgorithm()
    {

    }

    DynamicPlanningAlgorithm::~DynamicPlanningAlgorithm()
    {

    }
    /// \brief countWays 计算x可以换算成changes中的数据的组合数有多少种方法
    /// \param changes 可换算的基本数据
    /// \param n   changes的大小
    /// \param x   待换算的数据
    /// \return 返回x换算的方法数目
    int DynamicPlanningAlgorithm::CountWays(vector<int> changes, int n, int x) {
        // 待兑换的金额是i，可用的范围是0-j-1
        // i = x,j = 0-n-1
        //int dp[x+1][n];

        //判断
        if(changes.empty()||n<=0||x<=0){
            ELogPrint("the param is Invalid");
            return 0;
        }
        //使用动态开辟二维数组
        int **dp = new int*[x+1];
        for(int i=0;i<(x+1);i++)
        {
            dp[i] = new int[n];
            memset(dp[i],0,n*sizeof(int));
        }
        // 当待兑换的金额是0的时候，都是只有一种,就是空集
        for(int i=0;i<n;i++){//
            dp[0][i] = 1;
        }

        // 第1列，待兑换的钱是i，因为只能用第一种零钱，所以只有当是第一种零钱的整数倍的时候，才有一种兑换方法
        for (int i = 1; i <= x; i++) {
            if (i % changes[0] == 0) {
                dp[i][0] = 1;
            }
        }
        for (int i = 1; i <= x; i++)
        {
            for (int j = 1; j < n; j++)
            {
                if (i - changes[j] >= 0)
                {
                    dp[i][j] += (dp[i][j-1] + dp[i - changes[j]][j]);
                }
                else{
                    dp[i][j] += dp[i][j-1];
                }
            }
        }
        int count=dp[x][n-1];


        for(int k=0;k<(x+1);k++)
            delete []dp[k];

        delete []dp;

        return count;
    }

    //map<待兑换金额,map<所用找零的面值,找零具体方案vector<vector<int> > >>
    //map<int,map<int,vector<vector<int> > > > mapAimChanges;
    /// \brief countWays 计算x可以换算成changes中的数据的组合数有多少种方法
    /// \param changes 可换算的基本数据
    /// \param n   changes的大小
    /// \param x   待换算的数据
    /// \param vecvecAimChanges   x换算成功以后的数据方法数
    /// \return 返回x换算的方法数目
    int DynamicPlanningAlgorithm::CountWaysEx(vector<int> changes, int n, int x, vector<vector<int> >& vecvecAimChanges) {
        //存储换算过程中的具体数据方法
        map<int,map<int,vector<vector<int> > > > mapAimChanges;
        // 待兑换的金额是i，可用的范围是0-j-1
        // i = x,j = 0-n-1
        //int dp[x+1][n];

        //判断
        if(changes.empty()||n<=0||x<=0){
            ELogPrint("the param is Invalid");
            return 0;
        }
        //使用动态开辟二维数组
        int **dp = new int*[x+1];
        for(int i=0;i<(x+1);i++)
        {
            dp[i] = new int[n];
            memset(dp[i],0,n*sizeof(int));
        }

        // 当待兑换的金额是0的时候，都是只有一种,就是空集
        for(int i=0;i<n;i++){//
            dp[0][i] = 1;
        }

        // 第1列，待兑换的钱是i，因为只能用第一种零钱，所以只有当是第一种零钱的整数倍的时候，才有一种兑换方法
         for (int i = 1; i <= x; i++) {
            if (i % changes[0] == 0) {
                //一维数组,具体找零方案
                vector<int> vecTemp(i/changes[0],changes[0]);
                //找零方案的组合，形成一个二维数组
                vector<vector<int> > vecvecTemp(1,vecTemp);
                //存储所用零钱
                map<int,vector<vector<int> > > mapVecVecTemp;
                mapVecVecTemp.insert(pair<int,vector<vector<int> > >(0,vecvecTemp));

                mapAimChanges.insert(pair<int,map<int,vector<vector<int> > > >(i,mapVecVecTemp));

                dp[i][0] = 1;
            }
        }
        for (int i = 1; i <= x; i++)
        {
            for (int j = 1; j < n; j++)
            {
                if (i - changes[j] >= 0)
                {
                    //找零方案的组合，二维数组
                    vector<vector<int> > vecvecTempGoup;
                    //存储所用零钱索引j的找零方案组合
                    map<int,vector<vector<int> > > mapVecVecTemp;

                    map<int,map<int,vector<vector<int> > > >::iterator iter1=mapAimChanges.find(i);
                    if (iter1!=mapAimChanges.end())
                    {
                        mapVecVecTemp=iter1->second;//进行一次拷贝，防止后续在earse的时候出现其他内容的丢失
                        //查找上一个索引j-1
                        map<int,vector<vector<int> > >::iterator iter2=iter1->second.find(j-1);
                        if (iter2!=iter1->second.end())
                        {//如果找到
                            //先把j-1的内容拷贝
                            vecvecTempGoup=iter2->second;
                        }
                    }

                    //添加新的方案
                    vector<vector<int> > vecvecTemp2;//新的找零二维数组方案
                    iter1=mapAimChanges.find(i - changes[j]);//查找之前对i-change[j]的找零方案
                    if (iter1!=mapAimChanges.end())
                    {
                        //查找上一个索引j
                        map<int,vector<vector<int> > >::iterator iter2=iter1->second.find(j);
                        if (iter2!=iter1->second.end())
                        {//如果找到
                            //对于i-changes[j]的零钱在j这个索引上所用的所有找零方案，
                            //我们需要在之前的基础上插入一个新的元素changes[i]，形成一个新的找零方案
                            vector<vector<int> > vecvecTemp;
                            vecvecTemp=iter2->second;

                            vector<vector<int> >::iterator vecvecIter3=vecvecTemp.begin();
                            for (;vecvecIter3!=vecvecTemp.end();vecvecIter3++)
                            {
                                vector<int> vecTemp=*vecvecIter3;
                                vecTemp.push_back(changes[j]);
                                //将生成的新找零方案，放入二维数组
                                vecvecTemp2.push_back(vecTemp);
                            }
                        }
                    }
                    else if(i - changes[j]==0)//说明找到的是为空集
                    {//说明i - changes[j]==0，才会为空集
                        vector<int> vecTemp_0(1,changes[j]);
                        vecvecTemp2.push_back(vecTemp_0);
                    }

                    if (!vecvecTemp2.empty())
                    {
                        vecvecTempGoup.insert(vecvecTempGoup.end(),vecvecTemp2.begin(),vecvecTemp2.end());
                    }
                    if (!vecvecTempGoup.empty())
                    {
                        mapVecVecTemp.erase(j);
                        mapVecVecTemp.insert(pair<int,vector<vector<int> > >(j,vecvecTempGoup));
                        mapAimChanges.erase(i);
                        mapAimChanges.insert(pair<int,map<int,vector<vector<int> > > >(i,mapVecVecTemp));
                    }


                    dp[i][j] += (dp[i][j-1] + dp[i - changes[j]][j]);
                }
                else{
                    //找零方案的组合，二维数组
                    vector<vector<int> > vecvecTemp;
                    map<int,map<int,vector<vector<int> > > >::iterator iter1=mapAimChanges.find(i);
                    if (iter1!=mapAimChanges.end())
                    {
                        //存储所用零钱索引j的找零方案组合
                        map<int,vector<vector<int> > > mapVecVecTemp=iter1->second;
                        //查找上一个索引
                        map<int,vector<vector<int> > >::iterator iter2=iter1->second.find(j-1);
                        if (iter2!=iter1->second.end())
                        {//如果找到
                            vecvecTemp=iter2->second;
                            mapVecVecTemp.insert(pair<int,vector<vector<int> > >(j,vecvecTemp));

                            //map操作的时候，相同的key的时候，会出现insert失败，而不是覆盖
                            //所以这里需要先进行删除
                            mapAimChanges.erase(i);
                            //mapAimChanges.erase(iter1);
                            mapAimChanges.insert(pair<int,map<int,vector<vector<int> > > >(i,mapVecVecTemp));
                        }
                    }
                    dp[i][j] += dp[i][j-1];
                }
            }
        }
        int count=dp[x][n-1];


        for(int k=0;k<(x+1);k++)
            delete []dp[k];

        delete []dp;

        //将具体的数据方法拷贝给调用者
        map<int,map<int,vector<vector<int> > > >::iterator map_iter=mapAimChanges.find(x);
        if (map_iter!=mapAimChanges.end())
        {
            map<int,vector<vector<int> > >::iterator map_iter2=map_iter->second.find(changes.size()-1);
            if (map_iter2!=map_iter->second.end())
            {
                //数据拷贝
                 vecvecAimChanges=map_iter2->second;
                //调试
    //                vector<vector<int> >::iterator vec_vec_iter3=map_iter2->second.begin();
    //                //printf("size:%d\n",map_iter2->second.size());
    //                for (;vec_vec_iter3!=map_iter2->second.end();vec_vec_iter3++)
    //                {
    //                    vector<int> vecTemp=*vec_vec_iter3;
    //                    //打印调试
    ////                    for (int i=0;i<vecTemp.size();i++)
    ////                    {
    ////                        printf("[");
    ////                        printf(" %d ",vecTemp.at(i));
    ////                        printf("]");
    ////                    }
    ////                    printf("\n");
    //                }
            }
        }


        return count;
    }


}
