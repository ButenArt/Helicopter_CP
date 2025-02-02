#ifndef QUEUETIMER_H
#define QUEUETIMER_H

#include <Windows.h>


static void CALLBACK TimerProc(void*, BOOLEAN);

class QueueTimer
{
public:

    QueueTimer()
    {
        m_hTimer = NULL;
        m_mutexCount = 0;
    }

    virtual ~QueueTimer()
    {
        Stop();
    }

    bool Start(unsigned int interval,   // interval in ms
               bool immediately = false,// true to call first event immediately
               bool once = false)       // true to call timed event only once
    {
        if( m_hTimer )
        {
            return false;
        }

        SetCount(0);

        BOOL success = CreateTimerQueueTimer( &m_hTimer,
                                              NULL,
                                              TimerProc,
                                              this,
                                              immediately ? 0 : interval,
                                              once ? 0 : interval,
                                              WT_EXECUTEINTIMERTHREAD);

        return( success != 0 );
    }

    void Stop()
    {
        DeleteTimerQueueTimer(NULL, m_hTimer, NULL);
        m_hTimer = NULL ;
    }

    virtual void OnTimedEvent()
    {
        // Override in derived class
    }

    void SetCount(int value)
    {
        InterlockedExchange( &m_mutexCount, value );
    }

    int GetCount()
    {
        return InterlockedExchangeAdd( &m_mutexCount, 0 );
    }

private:
    HANDLE m_hTimer;
    long m_mutexCount;
};


void CALLBACK TimerProc(void* param, BOOLEAN /*timerCalled*/)
{
    QueueTimer* timer = static_cast<QueueTimer*>(param);
    timer->SetCount( timer->GetCount()+1 );
    timer->OnTimedEvent();
};

template <class T> class TQueueTimer : public QueueTimer
{
public:
    typedef private void (T::*TimedFunction)(void);

    TQueueTimer()
    {
        m_pTimedFunction = NULL;
        m_pClass = NULL;
    }

    void SetTimedEvent(T *pClass, TimedFunction pFunc)
    {
        m_pClass         = pClass;
        m_pTimedFunction = pFunc;
    }

protected:
    void OnTimedEvent()
    {
        if (m_pTimedFunction && m_pClass)
        {
            (m_pClass->*m_pTimedFunction)();
        }
    }

private:
    T *m_pClass;
    TimedFunction m_pTimedFunction;
};

#endif // QUEUETIMER_H
