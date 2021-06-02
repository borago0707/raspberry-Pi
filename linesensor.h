#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <QObject>
#include <wiringPi.h>
#include <QThread>
#include <iostream>
#include <QMutex>
#include <QCoreApplication>

class LineSensor : public QObject
{
    Q_OBJECT
public:
    LineSensor();
    ~LineSensor();

public slots:
    void slot_followLine();
    void slot_stopFollow();

signals:
    void sgn_goStraight();
    void sgn_goLeft();
    void sgn_goRight();
    void sgn_stopFollowLine();

private:
    bool m_bIsRunning;
    QMutex m_mutex;
    QThread m_threadLine;

    int m_nValueC; //Center
    int m_nValueR; //Right side
    int m_nPosition;
};

#endif // LINESENSOR_H
