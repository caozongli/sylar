#ifndef __SYLAR_THREAD_H_
#define __SYLAR_THREAD_H_

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

namespace sylar{

    class Semaphore{
    public:
        Semaphore(uint32_t count = 0);
        ~Semaphore();

        void wait();
        void notity();

    private:
        Semaphore(const Semaphore &) = delete;
        Semaphore(const Semaphore &&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

    private:
        sem_t m_semaphore;
    };

    template<class T>
    struct ScopeLockImp1{
    public:
        ScopeLockImp1(T& mutex)
        :m_mutex(mutex){
            m_mutex.lock();
            m_locked = true;
        }
        ~ScopeLockImp1(){
            unlock();
        }

        void lock(){
           if(!m_locked) {
               m_mutex.lock();
               m_locked = true;
           }
        }

        void unlock(){
            if(m_locked){
                m_mutex.unlock();
                m_locked = false;
            }
        }
    private:
        T& m_mutex;
        bool m_locked;
    };


    template<class T>
    struct ReadScopeLockImp1{
    public:
        ReadScopeLockImp1(T& mutex)
                :m_mutex(mutex){
            m_mutex.rdlock();
            m_locked = true;
        }
        ~ReadScopeLockImp1(){
            unlock();
        }

        void lock(){
            if(!m_locked) {
                m_mutex.rdlock();
                m_locked = true;
            }
        }

        void unlock(){
            if(m_locked){
                m_mutex.unlock();
                m_locked = false;
            }
        }
    private:
        T& m_mutex;
        bool m_locked;
    };

    class Mutex{
    public:
        typedef  ScopeLockImp1<Mutex> Lock;
        Mutex(){
            pthread_mutex_init(&m_mutex, nullptr);
        }

        ~Mutex(){
            pthread_mutex_destroy(&m_mutex);
        }

        void lock() {
            pthread_mutex_lock(&m_mutex);
        }

        void unlock(){
            pthread_mutex_unlock(&m_mutex);
        }
    private:
        pthread_mutex_t m_mutex;
    };

    class NullMutex{
    public:
        typedef ScopeLockImp1<NullMutex> Lock;
        NullMutex(){}
        ~NullMutex(){}
        void lock(){}
        void unlock(){}
    private:

    };

    template<class T>
    struct WriteScopeLockImp1{
    public:
        WriteScopeLockImp1(T& mutex)
        :m_mutex(mutex){
            m_mutex.wrlock();
            m_locked = true;
        }
        ~WriteScopeLockImp1(){
            unlock();
        }

        void lock(){
            if(!m_locked) {
                m_mutex.wrlock();
                m_locked = true;
            }
        }

        void unlock(){
            if(m_locked){
                m_mutex.unlock();
                m_locked = false;
            }
        }
    private:
        T& m_mutex;
        bool m_locked;
    };


    class RWMutex{
    public:
        typedef ReadScopeLockImp1<RWMutex> ReadLock;
        typedef WriteScopeLockImp1<RWMutex> WriteLock;
        RWMutex(){
            pthread_rwlock_init(&m_lock, nullptr);
        }

        ~RWMutex(){
            pthread_rwlock_destroy(&m_lock);
        }

        void rdlock(){
            pthread_rwlock_rdlock(&m_lock);
        }

        void wrlock() {
            pthread_rwlock_wrlock(&m_lock);
        }

        void unlock() {
            pthread_rwlock_unlock(&m_lock);
        }
    private:
        pthread_rwlock_t m_lock;
    };

    class NullRWMutex{
    public:
        typedef ReadScopeLockImp1<NullRWMutex> ReadLock;
        typedef WriteScopeLockImp1<NullRWMutex> WriteLock;
        NullRWMutex(){}
        ~NullRWMutex(){}

        void rdlock(){}
        void wrlock(){}
        void unlock(){}
    };

    class Spinlock{
    public:
        typedef ScopeLockImp1<Spinlock> Lock;
        Spinlock() {
            pthread_spin_init(&m_mutex, 0);
        }
        ~Spinlock() {
            pthread_spin_destroy(&m_mutex);
        }

        void lock(){
            pthread_spin_lock(&m_mutex);
        }

        void unlock(){
            pthread_spin_unlock(&m_mutex);
        }
    private:
        pthread_spinlock_t m_mutex;
    };

    class Thread{
    public:
        typedef std::shared_ptr<Thread> ptr;
        Thread(std::function<void()> cb, const std::string &name);
        ~Thread();

        pid_t getId() const;
        const std::string& getName() const;

        void join();

        static Thread* GetThis();
        static const std::string& GetName();
        static void SetName(const std::string& name);

    private:
        Thread(const Thread&) = delete;
        Thread(const Thread&&) = delete;
        Thread& operator=(const Thread&) = delete;

        static void* run(void* arg);

    private:
        pid_t m_id;
        pthread_t m_thread = 0;
        std::function<void()> m_cb;
        std::string m_name;

        Semaphore m_semaphore;

    };

};

//pthread_xxx
//std::thread包装了pthread


#endif
