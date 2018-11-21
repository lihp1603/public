#include "work_queue.h"

WorkQueue::WorkQueue()
{
    m_lock_flag  = true;
    m_queue_size = 0;
    
    if (!initial_locks())
        m_lock_flag = false;
}

WorkQueue::~WorkQueue()
{
    if (!destroy_locks())
        ;// do something
}

bool WorkQueue::initial_locks()
{
    if (0 != pthread_cond_init(&m_worker_cond, NULL) || 
        0 != pthread_cond_init(&m_producer_cond, NULL) ||
        0 != pthread_mutex_init(&m_queue_lock, NULL))        
        return false;
        
    return true;
}

bool WorkQueue::destroy_locks()
{
    if (0 != pthread_cond_destroy(&m_worker_cond) || 
        0 != pthread_cond_destroy(&m_producer_cond) ||
        0 != pthread_mutex_destroy(&m_queue_lock))        
        return false;
        
    return true;
}

bool WorkQueue::add_work(WorkBase* work)
{
//    WorkBase* tmp = static_cast<WorkBase*> (work->clone());
//    return do_add_work(tmp);
    //修改：不使用对work进行clone，而选择保存work本身-lihaiping1603@aliyun.com
    return do_add_work(work);
}

bool WorkQueue::add_work(WorkBase& work)
{
//    WorkBase* tmp = static_cast<WorkBase*> (work.clone());
//    return do_add_work(tmp);

    //修改：不使用对work进行clone，而选择保存work本身-lihaiping1603@aliyun.com
    return do_add_work(&work);
}

//add by lihaiping1603@aliyun.com
//删除队列中的work
void WorkQueue::remove_work(WorkBase *work)
{
    if (0 != pthread_mutex_lock(&m_queue_lock)) {
        return ;
    }
    //遍历
    list<WorkBase*>::iterator iter=m_work_queue.begin();
    for(;iter!=m_work_queue.end();){
        WorkBase*pW=*iter;
        if(pW==work){//如果相等
            m_work_queue.erase(iter++);
            m_queue_size--;
        }
        else{
            iter++;
        }
    }
    pthread_mutex_unlock(&m_queue_lock);
    return ;
}

bool WorkQueue::do_add_work(WorkBase* work)
{
    if (0 != pthread_mutex_lock(&m_queue_lock)) {
//        delete work;
//        work = NULL;
        return false;
    }
    
    while (m_queue_size >= MAX_WORK_NUMBER)    
        pthread_cond_wait(&m_producer_cond, &m_queue_lock); // when thread wake up, the mutext is locked already
    
//    m_work_queue.push(work);
    m_work_queue.push_back(work);
    m_queue_size++;
    
    pthread_mutex_unlock(&m_queue_lock);
 
    /* wake up a worker thread */
    pthread_cond_signal(&m_worker_cond);
 
    return true;    
}

WorkBase* WorkQueue::get_work()
{
    WorkBase* work = NULL;
    if (0 != pthread_mutex_lock(&m_queue_lock))
        return work;
    
    while (m_queue_size == 0)       
        pthread_cond_wait(&m_worker_cond, &m_queue_lock);   // when thread wake up, the mutext is locked already
    
//    work = m_work_queue.front();
//    m_work_queue.pop();
    //取第一个元素
    work = m_work_queue.front();
    m_work_queue.pop_front();
    m_queue_size--;    
    pthread_mutex_unlock(&m_queue_lock);
    
    /* wake up a sleeping producer, maybe there is one */
    pthread_cond_signal(&m_producer_cond);
    
    return work;
}

unsigned int WorkQueue::size()
{
    unsigned int ret = 0;
    pthread_mutex_lock(&m_queue_lock);
    ret = m_queue_size;
    pthread_mutex_unlock(&m_queue_lock);
    
    return ret;
}






