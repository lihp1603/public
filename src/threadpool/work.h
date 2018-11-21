#ifndef _WORK_H_
#define _WORK_H_

#include <string>
#include <fstream>
using namespace std;


class WorkBase {
    friend class WorkQueue;
public:
    WorkBase(): has_work(false) {}
    WorkBase(string in): in_arg(in), has_work(true) {}
    
    virtual void do_work(ofstream& os);
    
//    virtual WorkBase* clone() {
//        return new WorkBase(*this);
//    }
    
    void set_path(string& arg) {
        in_arg = arg;
    }
    
    bool empty() {
        return has_work;
    }
    
private:

    string in_arg;
    string out_arg;
    bool   has_work;

};



#endif /* _WORK_H_ */
