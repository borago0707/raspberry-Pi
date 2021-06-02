#ifndef MOBILEPLATFORM_H
#define MOBILEPLATFORM_H

#include <QObject>
#include <iostream>
#include <qtimer.h>
#include <math.h>

#include <dcmotor.h>
#include <linesensor.h>
#include <encoder.h>
#include <distance_sensor.h>
#include <color_sensor.h>
/**
 * Diese Klasse stellt alle Funktionalitäten zur Steuerung der Mobilen Plattform bereit.
 * Sie enthält den Chassis und eine Motoreinheit bestehend aus zwei Motoren.
 * @brief The MobilePlattform class
 */

class MobilePlatform : public QObject
{
    Q_OBJECT
public:
    MobilePlatform();
    ~MobilePlatform();

public slots:
    void slot_forwardMotion();
    void slot_backwardMotion();
    void slot_leftMotion();
    void slot_rightMotion();
    void slot_stopMotion();

    void slot_followLine();
    void slot_readEncoder();
    void slot_driveCircle();

    void slot_updatePID();

    void slot_activateSensors();
    void slot_followCorridor();
    void slot_startFollowCorridor();

    void slot_getSpeedGUI(int speed);
    void slot_getRadiusGUI(double radius);

signals:
    void sgn_followLine();
    void sgn_readEncoder();

    void sgn_setCountsLeft();
    void sgn_setCountsRight();

    void sgn_stopEncoder();
    void sgn_activateSensors();
    void sgn_endOfCorridor();

    void sgn_test();

private:
    QTimer *m_pQTimer;

    DCmotor *m_pLeft;
    DCmotor *m_pRight;
    LineSensor *m_pLineSensor;
        int m_nLinePosition;

    Encoder *m_pLeftEncoder;
    Encoder *m_pRightEncoder;
        int m_nCountsLeftMotor;
        int m_nCountsRightMotor;

    distance_sensor *m_pDistanceSensorLeft;
    distance_sensor *m_pDistanceSensorRight;
    distance_sensor *m_pDistanceSensorCenter;
        int nW; //----------------------- m_....?
        int nLimitDistance;
        float fDistanceRight;
        float fDistanceCenter;
        float fDistanceLeft;

    color_sensor *m_pColorSensor;
        int nColor;

    double m_dSpeedGUI;
    double m_dSpeed;
    double m_dSpeedOld;
    double m_dSpeedLeftMotor;
    double m_dSpeedRightMotor;
    int m_nTimerInterval;


    //drive circle
    double m_dSpeedRatioRL;
    double m_dWheelBase;
    double m_dDistancePenToAxle;
    double m_dWheelDiameter;
    double m_dRadius;

    //corridor
    float m_fFactorSpeedRatio;
    int m_nDistance1;
    int m_nDistance2;
    float m_fSpeedRatioCurve;
    int m_nCaseNumber;

    //PID
    double m_dPwmPIDLeft;
    double m_dPwmPIDRight;
    double m_deLeft;
    double m_deRight;
    double m_de_sumLeft;
    double m_de_sumRight;
    double m_de_oldLeft;
    double m_de_oldRight;
    double m_dKpLeft;
    double m_dKpRight;

    double m_dMaxSpeed;
    int m_nTurningDirection; // 0=nothing ; 1=turn left ; 2=turn right

};

#endif // MOBILEPLATFORM_H
