#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_

#include <list>
#include <queue>
#include <pthread.h>
#include "work.h"

using namespace std;

const unsigned int MAX_WORK_NUMBER = 10000;

class WorkQueue {
public:
    WorkQueue();
    virtual ~WorkQueue();
    
    bool           add_work(WorkBase* work);
    bool           add_work(WorkBase& work);
    //add by lihaiping1603@aliyun.com
    //针对任务队列中的任务,提供一个删除操作
    void           remove_work(WorkBase* work);

    void           wake_up_worker();
    void           wake_up_producer();
    
    WorkBase*      get_work();
    
    bool  wake_up_all_worker() {
        if (0 != pthread_cond_broadcast(&m_worker_cond))
            return false;
        return true;
    }
    
    unsigned int size();
    
    bool lock_flag() const {
        return m_lock_flag;
    }
    
private:
    /* forbiden copy and assignment*/
    WorkQueue(const WorkQueue&);
    WorkQueue& operator=(const WorkQueue&);

//    queue<WorkBase*>  m_work_queue;
    //修改work存储的容器-lihaiping1603@aliyun.com
    list<WorkBase*>  m_work_queue;
    
    pthread_cond_t    m_worker_cond;
    pthread_cond_t    m_producer_cond;
    pthread_mutex_t   m_queue_lock;
    
    unsigned int      m_queue_size;
    
    bool              m_lock_flag;   
    
    bool              initial_locks();
    bool              destroy_locks();
    
    bool              do_add_work(WorkBase*);
};

#endif /* _WORK_QUEUE_H_ */
