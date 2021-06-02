#include "distance_sensor.h"

distance_sensor::distance_sensor(int PinSensor)
{
    m_bIsRunning = false;
    moveToThread(&m_threadDistance);
    m_threadDistance.start();
    //precise time measurement is important in this thread
    m_threadDistance.setPriority(QThread::HighPriority);

    m_nPin = PinSensor;
    digitalWrite(m_nPin, false);

    m_fDistance = 0.0;
    m_fMeasurement1 = 0.0;
    m_fMeasurement2 = 0.0;
    m_fMeasurement3 = 0.0;
    m_fMeasurement4 = 0.0;
    m_fMeasurement5 = 0.0;
    QList_measurements  <<5<<5<<5<<5<<5;

}

distance_sensor::~distance_sensor()
{
    m_bIsRunning = false;

    if (m_threadDistance.isRunning())
    {
        m_threadDistance.terminate();
        m_threadDistance.wait();
    }
}

//reads sensor and calculates distance to object
void distance_sensor::slot_readSensor()
{
    m_bIsRunning = true;
    std::cout << ("---Abstandssensor aktiviert---") << std::endl;

    m_mutexDistance.lock();
    m_fDistance = 0.0;
    m_mutexDistance.unlock();


    float fNewValue = 0.0;
    float fLastValue = 0.0;
    bool bValuePin;
    bool bValuePin_old;
    double time = 0.0;


    while (m_bIsRunning)
    {
        //trigger sensor
        pinMode(m_nPin,OUTPUT);
        digitalWrite(m_nPin, true);
        delayMicroseconds(10);
        digitalWrite(m_nPin, false);

        //read sensor
        pinMode(m_nPin,INPUT);
        fLastValue = fNewValue;
        bValuePin = false;
        bValuePin_old = false;

        time = 0.0;
        QElapsedTimer m_ElapsedTimer;
        m_ElapsedTimer.start();

        //get pulse from sensor
        //distances > 2m are ignored! for 2 meters: 200.0*29.0*2.0 = 11600
        while (time < 11600.0)
        {
            bValuePin_old = bValuePin;
            bValuePin = digitalRead(m_nPin);
            //elapsed time in mikroseconds
            time = m_ElapsedTimer.nsecsElapsed() / 1000.0;

            //start of pulse
            if ((bValuePin_old == false) && (bValuePin == true )) m_ElapsedTimer.start();

            //end of pulse
            if ((bValuePin_old == true ) && (bValuePin == false))
            {
                //calculate distance [cm]
                fNewValue = time / 29.0 / 2.0;
                if (fNewValue > 200.0) fNewValue = 200.0;
                //if value > 0.5cm --> finish
                if (fNewValue > 0.5) break;
                //restart if value is not plausible
                else m_ElapsedTimer.start();
            }
            //sound speed: 0.1cm*2 need 5.8us --> 5.8us = possible resolution of about 0.1cm
            //(multiply by 2 because sound pulse goes: sensor --> object --> sensor)
            m_threadDistance.usleep(5);
        }

        //save last values
        if (fLastValue != fNewValue)
        {
            //store values
            m_fMeasurement5 = m_fMeasurement4;
            m_fMeasurement4 = m_fMeasurement3;
            m_fMeasurement3 = m_fMeasurement2;
            m_fMeasurement2 = m_fMeasurement1;
            m_fMeasurement1 = fNewValue;

            //sort values
            QList_measurements[0] = m_fMeasurement1;
            QList_measurements[1] = m_fMeasurement2;
            QList_measurements[2] = m_fMeasurement3;
            QList_measurements[3] = m_fMeasurement4;
            QList_measurements[4] = m_fMeasurement5;
            qSort (QList_measurements.begin(), QList_measurements.end());

            //distance = middle value
            m_mutexDistance.lock();
            m_fDistance = QList_measurements[2];
            m_mutexDistance.unlock();
        }

        m_threadDistance.msleep(10);
        QCoreApplication::processEvents();
    }
}


//returns distance
float distance_sensor::getSensorValue()
{
    m_mutexDistance.lock();
    float fTmp = m_fDistance;
    m_mutexDistance.unlock();
    return fTmp;
}


//stops reading the distance sensor
void distance_sensor::slot_stopSensor()
{
    m_mutexDistance.lock();
    m_fDistance = 0.0;
    m_mutexDistance.unlock();
    m_bIsRunning = false;
    std::cout << ("---Abstandssensor deaktiviert---") << std::endl;
}






















