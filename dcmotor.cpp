#include "dcmotor.h"
#include <QDebug>
/**
 * To create an instance the constructur is set all the necessary options.
 * The connected GPIO pins must be defined as well as a default speed must be set.
 * @brief DCmotor::DCmotor
 * @param pinMotorA
 * @param pinMotorB
 * @param pinMotorE
 * @param speed
 */

DCmotor::DCmotor(int pinMotorA, int pinMotorB, int pinMotorE, int pwm) : QObject(NULL)
{

    // initialisiert die Pins und Variablen
    m_nPinA = pinMotorA;
    m_nPinB = pinMotorB;
    m_nPinE = pinMotorE;
    m_nPwm = pwm;
    // Setze die GPIO Pins als output/pwm_output
    pinMode(m_nPinA,OUTPUT);
    pinMode(m_nPinB,OUTPUT);
    pinMode(m_nPinE,PWM_OUTPUT);
    softPwmCreate(m_nPinE,0,255);

}

DCmotor::~DCmotor()
{

}

/**
 * Setzt die GPIO auf "Low" zum Stoppen des Motors
 * @brief DCmotor::stopMotor
 */
void DCmotor::stopMotor()
{
    digitalWrite(m_nPinA,0);
    digitalWrite(m_nPinB,0);
    softPwmWrite(m_nPinE,0);
}


/**
 * durch Setzen des GPIO Pins B auf "High" fängt
 * der Motor an zu drehen (je nach Anschluss ist die Drehrichtung)
 * @brief DCmotor::forward
 */
void DCmotor::forwardMotor()
{
    digitalWrite(m_nPinA,0);
    digitalWrite(m_nPinB,1);
    softPwmWrite(m_nPinE,m_nPwm);
}

/**
 * durch Setzen des GPIO Pins A auf "High" fängt
 * der Motor an zu drehen (je nach Anschluss ist die Drehrichtung)
 * @brief DCmotor::backward
 */
void DCmotor::backwardMotor()
{
    digitalWrite(m_nPinA,1);
    digitalWrite(m_nPinB,0);
    softPwmWrite(m_nPinE,m_nPwm);
}

/**
 * setzt einen neuen PWM Wert
 * @brief DCmotor::setPWM
 * @param pwm
 */
void DCmotor::setPWM(int pwm)
{
    m_nPwm = pwm;
    softPwmWrite(m_nPinE,m_nPwm);
}

/**
 * gibt den aktuellen PWM Wert zurück
 * @brief DCmotor::getPWM
 * @return
 */
int DCmotor::getPWM()
{
    return m_nPwm;
}






















