#ifndef __CTASKTHREAD_H_
#define __CTASKTHREAD_H_

#include "ctask.h"
#include "../coscond.h"
#include "../cosmutex.h"
#include "../costhread.h"



namespace PublicTool {

    class CTask;
    class COSMutex;
    class COSCond;

    class CTaskThread:public COSThread{
    public:
        CTaskThread(CTask *task);
        virtual ~CTaskThread();
        //线程任务函数
        virtual void Entry();
    protected:
        //为taskthread从新指定新的task
        void  SetTaskThread(CTask *task);

    private:
        CTask* m_pUserTask;

        //定义一个条件变量
        COSCond m_condQuit;
        COSMutex m_mutexQuit;
        bool m_bQuit;
        //使用coscond和mutex来做task和thread之间的信号通知
        COSCond m_condTask;
        COSMutex m_mutexTask;
    };


}

#endif // CTASKTHREAD_H
