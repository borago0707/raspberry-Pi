#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <QObject>
#include <QString>
#include <iostream>
#include <QTime>
#include <vector>
#include <wiringPi.h>
#include <softPwm.h>
/**
 * Diese Klasse besitzt sämtliche Funktionalitäten
 * zur Ansteuerung eines Motors
 * Ein Motor ist über drei GPIO Pins mit dem Pi verbunden (A,B,E(pwm)).
 * Die Pins werden im Konstruktor definiert.
 * @brief The DCmotor class
*/


class DCmotor : public QObject
{
    Q_OBJECT
public:
    DCmotor(int pinMotorA, int pinMotorB, int pinMotorE, int pwm);
    ~DCmotor();

    void forwardMotor();
    void backwardMotor();
    void stopMotor();

    void setPWM(int);
    int getPWM();

private:
    int m_nPwm;
    int m_nPinA;
    int m_nPinB;
    int m_nPinE;
};

#endif // DCMOTOR_H
