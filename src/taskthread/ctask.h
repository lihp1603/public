#ifndef __CTASK_H_
#define __CTASK_H_

#include "../coscond.h"
#include "../cosmutex.h"
#include "ctaskthread.h"

namespace PublicTool {

    class CTaskThread;
    //task基类
    class CTaskBase{
    public:
         CTaskBase();
         virtual ~CTaskBase();

         //获取任务的开始时间
         int64_t GetTaskStartTimer();
         //获取这个任务流逝的时间：now-starttime
         int64_t GetTaskElapseTimer();
         //声明一个任务运行函数
         virtual void Run()=0;
    protected:
         //做任务函数
         void do_task();
         //等待任务退出
         void WaitExitRun();
         //是否需要退出
         bool IsNeedExitRun();
         //设置退出
         bool SetExitRun();
    private:
         //表示run开始运行
         void StartRunning();
         //判断run是否在运行过程中
         bool IsRunning();
         //发送退出信号，表示run已经退出
         void SignalExitOK();

         //用于task退出
         COSMutex m_mutexExit;
         COSCond  m_condExit;
         bool m_bExit;
         bool m_bRunning;
         //定义一个task的开始时间
         int64_t m_llStartTimer;
    };

    //任务类
    class CTask:public CTaskBase{
    public:
        typedef unsigned int EventFlags;

         //EVENTS
         //here are all the events that can be sent to a task
         enum
         {
             kKillEvent =    0x1 << 0x0, //these are all of type "EventFlags"
             kIdleEvent =    0x1 << 0x1,
             kStartEvent =   0x1 << 0x2,
             kTimeoutEvent = 0x1 << 0x3,

           //socket events
             kReadEvent =        0x1 << 0x4, //All of type "EventFlags"
             kWriteEvent =       0x1 << 0x5,

            //update event
             kUpdateEvent =      0x1 << 0x6
         };
        CTask();
        virtual ~CTask();

    private:
        //Send an event to this task.
        void  Signal(EventFlags eventFlags);
        //
        void  SetTaskThread(CTaskThread *thread);
        EventFlags      m_fEvents;
        CTaskThread*     m_fUseThisThread;
        friend class CTaskThread;
    };
}


#endif // CTASK_H
