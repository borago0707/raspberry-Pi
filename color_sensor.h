#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <QObject>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <QThread>
#include <QMutex>
#include <QCoreApplication>

class color_sensor : public QObject
{
    Q_OBJECT
public:
    color_sensor();
    ~color_sensor();
    int getColor();

public slots:
    void slot_detectColor();
    void slot_stopDetectingColor();

private:
    QMutex m_mutexColorSet;
    QThread m_threadColorSensor;
    int m_nDeviceHandleId;
    int m_nColor;
    bool m_bIsRunning;

};

#endif // COLOR_SENSOR_H
