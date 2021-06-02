#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <QMutex>
#include <wiringPi.h>
#include <QThread>
#include <QCoreApplication>
#include <iostream>

class Encoder : public QObject
{
    Q_OBJECT
public:
    explicit Encoder(QObject *parent = nullptr);

    Encoder(int pinEncoderA, int pinEncoderB);
    ~Encoder();
    bool m_bIsRunning;
    int getEncoderValue();

signals:
    void sgn_EncoderValues();

public slots:
    void slot_readEncoder();
    void slot_stopEncoder();
    void slot_setCounts();



private:
    QMutex m_mutexRunning;
    QMutex m_mutexCounts;
    QThread m_threadEncoder;

    int m_nPinA;
    int m_nPinB;

    int m_nValueA;
    int m_nValueB;
    int m_nCounts;
    int m_nOld_valueA;
    int m_nOld_valueB;

};

#endif // ENCODER_H
