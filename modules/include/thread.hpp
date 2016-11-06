#ifndef THREAD_HPP
#define THREAD_HPP

#include "stdfax.h"

class Thread;

typedef void* (*ThreadCallback)(Thread *, void*);

class Thread {
private:
    pthread_t _thread;
    bool _isPending;
    pthread_mutex_t _mutex;
    void * _parent;
    ThreadCallback _doneCallback;
    static void* _run(void *);
protected:
    bool _isRunning;
public:

    Thread ();

    virtual ~Thread ();
    virtual void run () = 0;

    void start();
    void join();
    void lock();
    void release();
    void exit();
    void setDoneCallback(ThreadCallback, void * parent = NULL);
    void setParent(void *parent);
    long getThreadId ();

    bool isRunning();
    bool isPending();
};

#endif //THREAD_HPP