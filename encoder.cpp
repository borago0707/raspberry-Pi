#include "encoder.h"

Encoder::Encoder(int pinEncoderA, int pinEncoderB)
{
    m_bIsRunning = false;
    moveToThread(&m_threadEncoder);
    m_threadEncoder.start();

    m_nPinA = pinEncoderA;
    m_nPinB = pinEncoderB;
    pinMode(m_nPinA,INPUT);
    pinMode(m_nPinB,INPUT);

    m_nCounts=0;
    m_nValueA=0;
    m_nValueB=0;
    m_nOld_valueA=0;
    m_nOld_valueB=0;
}

Encoder::~Encoder()
{
    m_bIsRunning = false;

    if (m_threadEncoder.isRunning())
    {
        m_threadEncoder.terminate();
        m_threadEncoder.wait();
    }
}

//count motor ticks
void Encoder::slot_readEncoder()
{
    m_bIsRunning = true;
    std::cout << ("---Encoder aktiviert---") << std::endl;

    while (m_bIsRunning == true)
    {
        //read both pins
        m_nValueA = digitalRead(m_nPinA);
        m_nValueB = digitalRead(m_nPinB);

        //count
        m_mutexCounts.lock();
        if((m_nValueA != m_nOld_valueA) || (m_nValueB != m_nOld_valueB)) m_nCounts++;
        m_mutexCounts.unlock();

        m_nOld_valueA = m_nValueA;
        m_nOld_valueB = m_nValueB;

        m_threadEncoder.usleep(20);
        QCoreApplication::processEvents();
    }
}


//get counts method
int Encoder::getEncoderValue()
{
    m_mutexCounts.lock();
    int tmp = m_nCounts;
    m_mutexCounts.unlock();

    return tmp;
}


//reset counters method
void Encoder::slot_setCounts()
{
    m_mutexCounts.lock();
    m_nCounts = 0;
    m_mutexCounts.unlock();
}

//stops reading the encoder
void Encoder::slot_stopEncoder()
{
    std::cout << ("---Encoder deaktiviert---") << std::endl;
    m_bIsRunning=false;

    m_mutexCounts.lock();
    m_nCounts=0;
    m_mutexCounts.unlock();
}






























