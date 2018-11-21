/*
 * 主要用于使用动态规划的算法计算数据的组合数，即解决人民币找零这种类似的经典问题
 * author:lihaiping1603@aliyun.com
 * date:2017-11-08 create
*/

#ifndef __DYNAMIC_PLANNING_ALGORITHM_H_
#define __DYNAMIC_PLANNING_ALGORITHM_H_
#include <vector>
using namespace std;

namespace Algorithm {

    //使用动态规划算法计算组合数
    class DynamicPlanningAlgorithm{
    public:
        DynamicPlanningAlgorithm();
        ~DynamicPlanningAlgorithm();
        //只求组合数
        /// \brief countWays 计算x可以换算成changes中的数据的组合数有多少种方法
        /// \param changes 可换算的基本数据
        /// \param n   changes的大小
        /// \param x   待换算的数据
        /// \return 返回x换算的方法数目
        ///函数只求组合数
        int CountWays(vector<int> changes, int n, int x);

        /// \brief countWays 计算x可以换算成changes中的数据的组合数有多少种方法
        /// \param changes 可换算的基本数据
        /// \param n   changes的大小
        /// \param x   待换算的数据
        /// \param vecvecAimChanges   x换算成功以后的数据方法数
        /// \return 返回x换算的方法数目
        /// //此函数求组合数的时候，同时可以算出具体的组合有哪些
        int CountWaysEx(vector<int> changes, int n, int x, vector<vector<int> >& vecvecAimChanges);

    };


}

#endif // DYNAMICPLANNINGALGORITHM_H
