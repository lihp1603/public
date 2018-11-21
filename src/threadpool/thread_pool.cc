#include "thread_pool.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <unistd.h>

/*
 * singleton
 */
ThreadPool* ThreadPool::m_instance = NULL;
 
ThreadPool* ThreadPool::create_instance()
{
    if (m_instance == NULL) {
        m_instance = new ThreadPool();
    }
    if (!m_instance->m_work_queue.lock_flag())  // if work queue has no locks, it will be useless,
        return NULL;                            // so the threadpoll is also useless
    return m_instance;
}

/*
 * constructor
 */
ThreadPool::ThreadPool() {
    m_total_number    = 0;
    m_idle_number     = 0;
    m_busy_number     = 0;
    m_initial_number  = DEFAULT_NUMBER;
    m_min_number      = DEFAULT_NUMBER;
    m_max_number      = MAX_THREAD_NUMBER;
    
    m_during_seconds  = DURING_SECONDS;
    m_heavy_work_size = HEAVY_WORK_SIZE;
    m_each_inc_number = EACH_INC_NUMBER;
    
    m_initialed       = false;
    m_decrease_flag   = false;
    m_manager_flag    = false;
    
    m_idle_header     = NULL;
    m_idle_end        = NULL;
    m_busy_header     = NULL;
    m_busy_end        = NULL;
    
    pthread_mutex_init(&m_counter_lock, NULL);
    pthread_mutex_init(&m_list_lock, NULL);
}

/*
 * creat threads, initial locks,
 * return true on success, false on failed
 */
bool ThreadPool::initial()
{
    if (m_initialed)
        return true;
    
    /* limit threads number */
    if (m_initial_number > MAX_THREAD_NUMBER)
        m_initial_number = MAX_THREAD_NUMBER;    
    
    /* create m_initial_number threads */
    if (!create_thread(m_initial_number))
        return false;
    
    /* should we crteate a manager? */
    if (m_manager_flag)
        if (!create_manager())
            cout << "create manager failed" << endl;
    
    m_initialed = true;
    return true;
}

/*
 * create new threads in the pool
 */
bool ThreadPool::create_thread(unsigned int number)
{
    if (0 != pthread_mutex_lock(&m_counter_lock))
        return false;
    
    if (m_total_number == MAX_THREAD_NUMBER) {
        pthread_mutex_unlock(&m_counter_lock);
        return false;   
    }
    
    Thread* tmp = NULL;
    for (unsigned int i = 0; i < number; ++i) {
        tmp = new Thread();
        if (0 == pthread_create(&tmp->m_id, NULL, ThreadPool::process, static_cast<void*> (tmp))) {
            if (++m_total_number == MAX_THREAD_NUMBER)
                break;
        }
        else {
            delete tmp;
            tmp = NULL;
        }
    }
    
    pthread_mutex_unlock(&m_counter_lock);
    return true;
}

void ThreadPool::to_idle(Thread* p_thread)
{
    /* if can't get lock, then we mustn't change the lists */
    if (0 != pthread_mutex_lock(&m_list_lock))
        return;
    
    /* first, remove from busy list, or not */
    if (p_thread->m_state == in_busy) {
        if (m_busy_number == 1) {
            m_busy_header = NULL;
            m_busy_end    = NULL;
        }
        else if (m_busy_header == p_thread) {
            m_busy_header = m_busy_header->m_next;
            if(m_busy_header)
                m_busy_header->m_prev = NULL;
        }
        else {
            if(p_thread->m_prev)
                p_thread->m_prev->m_next = p_thread->m_next;
            //这里应该还要更改一下,因为是双向链表
            if(p_thread->m_next){
                p_thread->m_next->m_prev=p_thread->m_prev;
            }
        }
        m_busy_number--;
    }
    
    /* then insert to idle list */
    if (m_idle_header == NULL) {
        m_idle_header = p_thread;
        m_idle_end    = p_thread;
    }
    else {
        m_idle_end->m_next = p_thread;
        p_thread->m_prev = m_idle_end;
        m_idle_end = p_thread;
    }
    p_thread->m_state = in_idle;
    m_idle_number++;
    
    pthread_mutex_unlock(&m_list_lock);
}

void ThreadPool::to_busy(Thread* p_thread)
{
    if (0 != pthread_mutex_lock(&m_list_lock))
        return;
     
    /* first, remove it from idle list */   
    if (p_thread->m_state == in_idle) {
        if (m_idle_number == 1) {
            m_idle_header = NULL;
            m_idle_end    = NULL;
        }
        else if (m_idle_header == p_thread) {
            m_idle_header = m_idle_header->m_next;
            if(m_idle_header)
                m_idle_header->m_prev = NULL;
        }
        else {
            if(p_thread->m_prev)
                p_thread->m_prev->m_next = p_thread->m_next;
            //这里应该还要更改一下,因为是双向链表
            if(p_thread->m_next){
                p_thread->m_next->m_prev=p_thread->m_prev;
            }
        }
        m_idle_number--;
    }
    
    /* then, insert to busy list */
    if (m_busy_header == NULL) {
        m_busy_header = p_thread;
        m_busy_end    = p_thread;
    }
    else {
        m_busy_end->m_next = p_thread;
        p_thread->m_prev = m_busy_end;
        m_busy_end = p_thread;
    }
    p_thread->m_state = in_busy;
    m_busy_number++;    
        
    pthread_mutex_unlock(&m_list_lock);
}

/* remove a thread from pool, always in busy list */
void ThreadPool::remove_thread(Thread* p_thread)
{
    pthread_mutex_lock(&m_counter_lock);
    m_total_number--;
    pthread_mutex_unlock(&m_counter_lock);

    if (0 != pthread_mutex_lock(&m_list_lock))
        return;
    
    if (m_busy_number == 1) {
        m_busy_header = NULL;
        m_busy_end    = NULL;
        m_busy_number = 0;
    }    
        
    if (p_thread == m_busy_header) {
        m_busy_header = m_busy_header->m_next;
        m_busy_header->m_prev = NULL;
    }
    else {
        p_thread->m_prev->m_next = p_thread->m_next;
    }    
    m_busy_number--;
    
    delete p_thread;
    p_thread = NULL;
    
    pthread_mutex_unlock(&m_list_lock);
}
 
 
 /*
  * decrease the number of current threads
  * this function will only cancel threads 
  * which are sleeping, waiting for work
  */
 bool ThreadPool::decrease()
 {
     /* no need to cancel any one */
     if (m_total_number <= m_min_number)
         return false;
     
     m_decrease_flag = true;
     
     if (!m_work_queue.wake_up_all_worker())
         return false;
         
     return true;
 }
 
 bool ThreadPool::decrease_to_none()
 {
     m_min_number = 0;
     return decrease();
 }
 
 /*
  * check m_decrease_flag and m_total_number,
  * tell thread wether it should exit
  */
bool ThreadPool::check_decrease()
{
    bool flag = true;
    if (!m_decrease_flag)
        return false;
    
    if (0 != pthread_mutex_lock(&m_counter_lock))
        return false;
    if (m_total_number > m_min_number)
        m_total_number--;
    else 
        flag = false;
        
    pthread_mutex_unlock(&m_counter_lock);
    return flag;
}


/*
 * create a new thread in the pool, this thread  work
 * as a manager, it will check the threadpool every hour,
 * and add threads or cancel threads according to current
 * situation.
 */
bool ThreadPool::create_manager()
{
    m_manager_flag = true;
    int ret;
    ret = pthread_create(&m_manager, NULL, ThreadPool::manage, static_cast<void*>(this));
    if (0 != ret) {
        cout << "creat thread error : " << ret << endl;
        return false;
    }
    return true;
}

void ThreadPool::manage_inc(ofstream& os)
{
    /* if work is too much, first try to wake up sleeping thread */
    if (m_work_queue.size() > m_heavy_work_size) {
        m_work_queue.wake_up_all_worker();
    }
    /* wait for a while */
    sleep(10);
    
    /* if work is still very much, create new threads */
    if (m_work_queue.size() > m_heavy_work_size) {
        os << "manager going to create some worker thread" << endl;
        create_thread(m_each_inc_number);    
    }

}

void ThreadPool::manage_dec(ofstream& os)
{
   /* if work queue is empty, and current threads number is bigger */
   /* than initial number, then cancel extra threads */
   if (m_work_queue.size() == 0 && m_total_number > m_min_number) {
       os << "manager going to cancel some worker thread" << endl;
       decrease();
   }
}

/*
 * get info from pool
 */
 unsigned int ThreadPool::get_idle_number()
 {
     unsigned int ret = 0;
     pthread_mutex_lock(&m_list_lock);
     ret = m_idle_number;
     pthread_mutex_unlock(&m_list_lock);
     
     return ret;
 }
     
unsigned int ThreadPool::get_busy_number()
{
     unsigned int ret = 0;
     pthread_mutex_lock(&m_list_lock);
     ret = m_busy_number;
     pthread_mutex_unlock(&m_list_lock);
     
     return ret;
}

unsigned int ThreadPool::get_total_number()
{
     unsigned int ret = 0;
     pthread_mutex_lock(&m_counter_lock);
     ret = m_total_number;
     pthread_mutex_unlock(&m_counter_lock);
     
     return ret;
}

/*
 * main rountine for each thread
 */
void* ThreadPool::process(void* arg)
{   
    pthread_t    self_id = pthread_self();

    /* get the thread pool, self, work queue */
    ThreadPool*  p_thread_pool = ThreadPool::create_instance();
    Thread*      p_self = static_cast<Thread*> (arg);
    WorkQueue&   work_queue = p_thread_pool->m_work_queue;


	/* create the thread's log file */
	ofstream out_stream;
	char file_name[20] = {0};
	sprintf(file_name, "%u_log", static_cast<unsigned int>(self_id));
	out_stream.open(file_name);
	if (!out_stream) {
		pthread_exit((void*)0);		
	}
	out_stream << "thread " << self_id << " start......" << endl;    
    out_stream << "pid " << getpid() << endl;
    
    /* begin to do the work */
    while (true) {            
        /* put self to idle list */
        p_thread_pool->to_idle(p_self);
        WorkBase* work = work_queue.get_work();       
        if (work != NULL) {
            /* put self to busy list */
            /* this means every time worker thread finish a work, it will put self to  */
            /* idle list , then go to work queue to see wether there is  more work to do, */
            /* if got one, thread must put self to busy list again. this looks not very */
            /* good, but no other choice */          
            p_thread_pool->to_busy(p_self);
            work->do_work(out_stream);
            //修改：因为修改了任务队列的存储方式，使用了非clone的方式，而是选择保存work对象本身，
            //所以这个地方不再使用delete，否则会出现崩溃-lihaiping1603@aliyun.com
//            delete work;
//            work = NULL;
            
            /* check decrease flag, should I exit now? */
            if (p_thread_pool->check_decrease()) 
                break;
        }

    }
    
    /* normal exit */
    pthread_detach(self_id);
    p_thread_pool->remove_thread(p_self);
    out_stream << "thread " << self_id << " exit......" << endl;       
    out_stream.close();
    return static_cast<void*> (0);
}

/* 
 * rountine for manager thread
 */
void* ThreadPool::manage(void* arg)
{
    pthread_t    self_id = pthread_self();

    /* get the thread pool handle */
    ThreadPool*  p_thread_pool = static_cast<ThreadPool*> (arg);
//    WorkQueue&   work_queue = p_thread_pool->m_work_queue;

	/* create the thread's log file */
	ofstream out_stream;
	out_stream.open("manage_log", ofstream::app);
	if (!out_stream) {
		pthread_exit((void*)0);		
	}
	out_stream << "manager thread start......" << endl;    
    out_stream << "pid " << getpid() << endl;
    
    /* manager begin to do work */
    while (true) {
        /* increase some thread ? */
        p_thread_pool->manage_inc(out_stream);
        /* decrease some thread ? */
        p_thread_pool->manage_dec(out_stream);

        if (!p_thread_pool->get_manager_flag())
            break;

        out_stream << "manager go to sleep now------------>" << endl;
        sleep(p_thread_pool->get_during_seconds());
        out_stream << "manager wake up to work------------>" << endl;
        
        out_stream << "idle number: " << p_thread_pool->get_idle_number() << endl;
        out_stream << "busy number: " << p_thread_pool->get_busy_number() << endl;
        out_stream << "total number: " << p_thread_pool->get_total_number() << endl;
        out_stream << "queue size: " << p_thread_pool->get_queue_size() << endl;        

    }

    /* normal exit */
    pthread_detach(self_id);
    p_thread_pool->clear_manager_id();
    out_stream << "manager thread exit......" << endl;       
    out_stream.close();
    return static_cast<void*> (0);    
    
}










 
 
 
 
 
 
 


