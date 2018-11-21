#include "ctaskthreadpool.h"
#include "stdint.h"
#include "ctask.h"
#include "ctaskthread.h"
#include <assert.h>

namespace CTaskThreadPool {

    CTaskThread** CTaskThreadPool::m_pTaskThreadArray = NULL;
    uint32_t       CTaskThreadPool::m_unNumTaskThreads = 0;
    uint32_t       CTaskThreadPool::m_unNumShortTaskThreads = 0;
    uint32_t       CTaskThreadPool::m_unNumBlockingTaskThreads = 0;

    Bool16 CTaskThreadPool::AddThreads(uint32_t numToAdd)
    {
        assert(m_pTaskThreadArray == NULL);
        m_pTaskThreadArray = new CTaskThread*[numToAdd];

        for (uint32_t x = 0; x < numToAdd; x++)
        {
            m_pTaskThreadArray[x] = new CTaskThread();
//            m_pTaskThreadArray[x]->Start();
            if (TASK_DEBUG)  qtss_printf("TaskThreadPool::AddThreads m_pTaskThreadArray[%"_U32BITARG_"]=%p\n",x, m_pTaskThreadArray[x]);
        }
        m_unNumTaskThreads = numToAdd;

        if (0 == m_unNumShortTaskThreads)
            m_unNumShortTaskThreads = numToAdd;

        return true;
    }



    CTaskThread* CTaskThreadPool::GetThread(uint32_t index)
    {

      assert(m_pTaskThreadArray != NULL);
      if (index >= m_unNumTaskThreads)
        return NULL;

       return m_pTaskThreadArray[index];

    }




    void CTaskThreadPool::RemoveThreads()
    {
        //Tell all the threads to stop
        for (uint32_t x = 0; x < m_unNumTaskThreads; x++)
            m_pTaskThreadArray[x]->SendStopRequest();

        //Because any (or all) threads may be blocked on the queue, cycle through
        //all the threads, signalling each one
        for (uint32_t y = 0; y < m_unNumTaskThreads; y++)
            m_pTaskThreadArray[y]->fTaskQueue.GetCond()->Signal();

        //Ok, now wait for the selected threads to terminate, deleting them and removing
        //them from the queue.
        for (uint32_t z = 0; z < m_unNumTaskThreads; z++)
            delete m_pTaskThreadArray[z];

        m_unNumTaskThreads = 0;
    }

}
