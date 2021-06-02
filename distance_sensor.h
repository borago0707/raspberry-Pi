#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <QObject>
#include <wiringPi.h>
#include <QThread>
#include <QCoreApplication>
#include <iostream>
#include <QMutex>
#include <QList>
#include <QElapsedTimer>


class distance_sensor : public QObject
{
    Q_OBJECT
public:
    distance_sensor(int PinSensor);
    ~distance_sensor();
    float getSensorValue();

private:
    bool m_bIsRunning;
    QThread m_threadDistance;
    QMutex  m_mutexDistance;


    float m_fDistance;
    int   m_nPin;
    float m_fMeasurement1;
    float m_fMeasurement2;
    float m_fMeasurement3;
    float m_fMeasurement4;
    float m_fMeasurement5;
    QList <float> QList_measurements;


public slots:
    void slot_readSensor();
    void slot_stopSensor();


signals:




signals:

};

#endif // DISTANCE_SENSOR_H
