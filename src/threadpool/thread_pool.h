#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <pthread.h>
#include <vector>
#include <queue>
#include <fstream>
#include "work.h"
#include "work_queue.h"
using namespace std;

/* default config for threadpool */
const unsigned int MAX_THREAD_NUMBER = 10;
const unsigned int DEFAULT_NUMBER    = 3;   // initial number, min number

/* parameters for manage stuff */
const unsigned int DURING_SECONDS    = 120;
const unsigned int HEAVY_WORK_SIZE   = 100;
const unsigned int EACH_INC_NUMBER   = 3;

enum ThreadState {
    in_busy = 0,
    in_idle,
    new_created
};

class Thread {
    friend class ThreadPool;
    
    Thread(): m_id(0), m_prev(NULL), m_next(NULL), m_state(new_created) {}
    
    pthread_t    m_id;
    Thread*      m_prev;
    Thread*      m_next;
    ThreadState  m_state;
};

class ThreadPool {

public:
    
    static ThreadPool* create_instance();
    
    ~ThreadPool() {
       decrease_to_none();
       set_manager_flag(false);
       pthread_mutex_destroy(&m_counter_lock);
       pthread_mutex_destroy(&m_list_lock);
    }
    
    bool initial();
    bool create_thread(unsigned int number);    
    bool decrease();
    bool decrease_to_none();
    bool check_decrease();
    bool create_manager();
    void manage_inc(ofstream& os);
    void manage_dec(ofstream& os);

    unsigned int get_idle_number();
    unsigned int get_busy_number();
    unsigned int get_total_number();

    bool add_work(WorkBase* work) {
        return m_work_queue.add_work(work);
    }
    bool add_work(WorkBase& work) {
        return m_work_queue.add_work(work);
    }
    //添加队列删除操作-lihaiping1603@aliyun.com
    void remove_work(WorkBase* work){
        return m_work_queue.remove_work(work);
    }

    /* function to config thread pool */
    ThreadPool* set_initial_number(unsigned int number) {
        m_initial_number = number;
        m_min_number     = number;
        return this;
    }
    ThreadPool* set_max_number(unsigned int number) {
        m_max_number = number;
        return this;
    }
    ThreadPool* set_min_number(int number) {
        m_min_number = number;
        return this;
    }
    
    /* functions to config manager */
    ThreadPool* set_manager_flag(bool flag) {
        m_manager_flag = flag;
        return this;
    }   
    ThreadPool* set_during_seconds(unsigned int sec) {
        m_during_seconds = sec;
        return this;
    }
    ThreadPool* set_heavy_work_size(unsigned int size) {
        m_heavy_work_size = size;
        return this;
    }
    ThreadPool* set_each_add_number(unsigned int number) {
        m_each_inc_number = number;
        return this;
    }
    
    void clear_manager_id() {
        m_manager = 0;
    }
    
    /* get some info from thread pool */        
    bool get_manager_flag() const {
        return m_manager_flag;
    }
    unsigned int get_during_seconds() const {
        return m_during_seconds;
    }
    unsigned int get_queue_size() {
        return m_work_queue.size();
    }
       
private:
    /* forbiden construct, assignment */
    ThreadPool();
    ThreadPool(const ThreadPool&);
    ThreadPool& operator=(const ThreadPool&);

    unsigned int          m_idle_number;     // idle threads number
    unsigned int          m_busy_number;     // busy threads number
    unsigned int          m_total_number;    // total threads number, this counter is nessary, it make sync more easy
    
    unsigned int          m_initial_number;  // number of thread should be created when initial thread pool
    unsigned int          m_max_number;      // threads in the pool mustn't be more than this
    unsigned int          m_min_number;
    
    unsigned int          m_during_seconds;
    unsigned int          m_heavy_work_size;
    unsigned int          m_each_inc_number;
    
    bool                  m_initialed;
    bool                  m_decrease_flag;   // flag to tell thread it's time to exit
    bool                  m_manager_flag;    // turn this flag on, means set a manager for threads in the pool
    
    Thread*               m_idle_header;       // idle list header
    Thread*               m_idle_end;
    Thread*               m_busy_header;       // busy list header
    Thread*               m_busy_end;
    
    WorkQueue             m_work_queue;      // work queue, threads get work here
    pthread_t             m_manager;
    
    static ThreadPool*    m_instance;        // only one instance

    pthread_mutex_t       m_counter_lock;    // lock for all counters
    pthread_mutex_t       m_list_lock;       // lock for idle list and busy list
    
    static void* process(void* arg);         // rountine for thread
    static void* manage(void* arg);          // rountine for manager
    
    void                  to_idle(Thread*);
    void                  to_busy(Thread*);
    void                  remove_thread(Thread*);
};

#endif /* _THREAD_POOL_ */
