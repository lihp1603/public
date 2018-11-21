#ifndef __CTASKTHREADPOOL_H_
#define __CTASKTHREADPOOL_H_

#include "stdint.h"
#include "ctask.h"
#include "ctaskthread.h"

namespace CTaskThreadPool {

    //Because task threads share a global queue of tasks to execute,
    //there can only be one pool of task threads. That is why this object
    //is static.
    class CTaskThreadPool {
    public:

        //Adds some threads to the pool
        static bool   AddThreads(uint32_t numToAdd); // creates the threads: takes NumShortTaskThreads + NumBLockingThreads,  sets num short task threads.
        static void     SwitchPersonality( char *user = NULL, char *group = NULL);
        static void     RemoveThreads();
        static CTaskThread* GetThread(uint32_t index);
        static uint32_t  GetNumThreads() { return m_unNumTaskThreads; }
        static void SetNumShortTaskThreads(uint32_t numToAdd) { m_unNumShortTaskThreads = numToAdd; }
        static void SetNumBlockingTaskThreads(uint32_t numToAdd) { m_unNumBlockingTaskThreads = numToAdd; }

    private:

        static CTaskThread**     m_pTaskThreadArray;
        static uint32_t          m_unNumTaskThreads;
        static uint32_t          m_unNumShortTaskThreads;
        static uint32_t          m_unNumBlockingTaskThreads;

//        static OSMutexRW        m_sMutexRW;// __attribute__((visibility("hidden")));

        friend class CTask;
        friend class CTaskThread;
    };
}

#endif // CTASKTHREADPOOL_H
