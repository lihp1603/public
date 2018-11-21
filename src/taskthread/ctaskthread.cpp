#include "ctaskthread.h"
#include "../coscond.h"
#include "../cosmutex.h"

namespace PublicTool {
    /* ==========================================================================*
    * ================================CTaskThread=====================================*
    * ==========================================================================*/

    CTaskThread::CTaskThread(CTask *task)
        :COSThread()
        ,m_pUserTask(task)
        ,m_bQuit(false)
    {
        this->Start();
    }

    CTaskThread::~CTaskThread()
    {
       //应该先让任务退出
       if (IsRunning())
       {
           //先设置线程退出标志
           m_mutexQuit.Lock();
           m_bQuit=true;
           m_mutexQuit.Unlock();
           //万一线程阻塞在task的锁地方，所以这个地方发送一次信号唤醒
           m_mutexTask.Lock();
           m_pUserTask=NULL;
           m_condTask.Signal();
           m_mutexTask.Unlock();
           //等待线程退出
           m_mutexQuit.Lock();
           while(m_bQuit)
           {
               m_condQuit.Wait(&m_mutexQuit);//等待
           }
           m_mutexQuit.Unlock();
           //销毁条件变量
       }
    }

    void CTaskThread::Entry()
    {
        while(!m_bQuit){
            m_mutexTask.Lock();
            if(m_pUserTask==NULL){
                m_condTask.Wait(&m_mutexTask);
            }
            if(m_pUserTask){
                m_pUserTask->do_task();//仅允许一次运行
                m_pUserTask=NULL;
            }
            m_mutexTask.Unlock();
        }

        m_mutexQuit.Lock();
        m_bQuit=false;
        m_condQuit.Signal();//发信息唤醒上面的等待
        m_mutexQuit.Unlock();

        return;
    }



//    void CTaskThread::SetTaskThread(CTask *task)
//    {
//        m_mutexTask.Lock();
//        m_pUserTask=task;
//        m_condTask.Signal();
//        m_mutexTask.Unlock();
//    }


}
