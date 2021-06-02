#include "mobileplatform.h"
#include <QDebug>
/// Pinbelegung!!
#define	MOTOR_1A	4
#define	MOTOR_1B	3  //left motor
#define MOTOR_1E    2  //PWM
#define	MOTOR_2A	15
#define	MOTOR_2B	16 //right motor
#define MOTOR_2E    1  //PWM

#define ENCODER_2A  21 //left encoder
#define ENCODER_2B  22
#define ENCODER_1A  10 //right encoder
#define ENCODER_1B  11

#define DISTANCE_LEFT   14
#define DISTANCE_RIGHT  12
#define DISTANCE_CENTER 13
//wiringPi pin 8+9 are SDA+SCL (I2C) for color sensor

MobilePlatform::MobilePlatform() : QObject(NULL)
{
    m_pQTimer = new QTimer();
        m_nTimerInterval = 10;
    m_pQTimer->setInterval(m_nTimerInterval);
    m_pQTimer->setSingleShot(false);

// DC Motor
    m_pLeft  = new DCmotor("Left", MOTOR_1A,MOTOR_1B,MOTOR_1E,25);
    m_pRight = new DCmotor("Recht", MOTOR_2A,MOTOR_2B,MOTOR_2E,25);

// LinieSensor
    m_pLineSensor = new LineSensor();
    m_nLinePosition = 0;
    connect(this, SIGNAL(sgn_followLine()), m_pLineSensor, SLOT(slot_followLine()));
    connect(m_pLineSensor, SIGNAL(sgn_goStraight()), this, SLOT(slot_forwardMotion()));
    connect(m_pLineSensor, SIGNAL(sgn_goRight()), this, SLOT(slot_rightMotion()));
    connect(m_pLineSensor, SIGNAL(sgn_goLeft()), this, SLOT(slot_leftMotion()));

// Encode
    m_pLeftEncoder  = new Encoder(ENCODER_1A, ENCODER_1B);
    m_pRightEncoder = new Encoder(ENCODER_2A, ENCODER_2B);
    connect(this, SIGNAL(sgn_readEncoder()), m_pLeftEncoder, SLOT(slot_readEncoder()));
    connect(this, SIGNAL(sgn_readEncoder()), m_pRightEncoder, SLOT(slot_readEncoder()));
    connect(this, SIGNAL(sgn_setCountsLeft()), m_pLeftEncoder, SLOT(slot_setCounts()));
    connect(this, SIGNAL(sgn_setCountsRight()), m_pRightEncoder, SLOT(slot_setCounts()));
    connect(this, SIGNAL(sgn_stopEncoder()), m_pLeftEncoder, SLOT(slot_stopEncoder()));
    connect(this, SIGNAL(sgn_stopEncoder()), m_pRightEncoder, SLOT(slot_stopEncoder()));

// DistanzSensor
    connect(this, SIGNAL(sgn_activateSensors()), m_pDistanceSensorRight, SLOT(slot_readSensor()));
    connect(this, SIGNAL(sgn_activateSensors()), m_pDistanceSensorCenter, SLOT(slot_readSensor()));
    connect(this, SIGNAL(sgn_activateSensors()), m_pDistanceSensorLeft, SLOT(slot_readSensor()));


// ColorSensor
   m_pColorSensor = new color_sensor;
   connect(this, SIGNAL(sgn_activateSensors()), m_pColorSensor, SLOT(slot_detectColor()));
   nColor = 0;

// PID
connect(m_pQTimer, SIGNAL(timeout()), m_pColorSensor, SLOT(slot_updatePID()));

//Korridor
connect(m_pQTimer, SIGNAL(timeout()), this, SLOT(slot_followCorridor()));
connect(this, SIGNAL(sgn_endOfCorridor()), this, SLOT(slot_stopMotion()));


//measured at the mobilePlatform [cm]
    m_dWheelBase = 11.0;
    m_dDistancePenToAxle = 11.4;
    m_dSpeedRatioRL  = 1.0;
    m_dWheelDiameter = 6.4;

//corridor
    m_fFactorSpeedRatio = 0.1;
    m_nDistance1 = 50; //distance to the wall ahead; where to beginn to slow down
    m_nDistance2 = 5;  //safety buffer / min. distance to the wall ahead
    m_fSpeedRatioCurve = 0.45; //speed ratio for driving trough curve
    m_nCaseNumber = 0;
    nW = 35; //max width that is possible in the corridor +tolerance
    nLimitDistance = 30;  //stop turning when center distance exeeds limit

    fDistanceRight  = 0;
    fDistanceCenter = 0;
    fDistanceLeft   = 0;



//PID parameters
    m_dKpLeft  = 1.0;
    m_dKpRight = 1.0;

    m_dMaxSpeed = 2*3.1416 * 2500/1440; //2pi * maxCountsPerSec / countsPerRev ; (original max=2500)
    m_de_sumLeft = 0.0;
    m_de_sumRight = 0.0;

    m_dSpeedGUI = 35/100.0 * m_dMaxSpeed;
    m_dRadius = 15.0;
    m_dSpeed = 0.0;
    m_dSpeedOld = 0.0;
    m_nTurningDirection = 0;

}


MobilePlatform::~MobilePlatform()
{
    delete m_pQTimer;

    delete m_pLeft;
    delete m_pRight;

    delete m_pLineSensor;
    delete m_pLeftEncoder;
    delete m_pRightEncoder;

    delete m_pDistanceSensorRight;
    delete m_pDistanceSensorCenter;
    delete m_pDistanceSensorLeft;

    delete m_pColorSensor;
}

//saves speed which is set by the user in the GUI
//dSpeedGUI is then used in the PID? controller below
void MobilePlatform::slot_getSpeedGUI(int nSpeed)
{
    //speed (convert from 0-100% to 0-maxSpeed in rad/sec)
    m_dSpeedGUI = nSpeed/100.0 * m_dMaxSpeed;
    qDebug()<<"Neue Geschwindigkeit:"<<endl;
    std::cout << "Neue Geschwindigkeit:  " << m_dSpeedGUI << std::endl;
}


//saves the radius, set in the GUI
void MobilePlatform::slot_getRadiusGUI(double dRadius)
{
    //radius in cm
    m_dRadius = dRadius;

    std::cout << "Neuer Radius[cm]:  " << m_dRadius << std::endl;
}


//start measuring distance
void MobilePlatform::slot_activateSensors()
{
    emit sgn_activateSensors();
}

/**
 * stoppt Bewegung der mobilen Plattform
 * @brief MobilePlatform::slot_stopMotion
 */
void MobilePlatform::slot_stopMotion()
{
    std::cout << ("-------------------------STOP---------------------------") << std::endl;
    /// stoppe beide Motoren
    m_pLeft->stopMotor();
    m_pRight->stopMotor();
    m_pLineSensor->slot_stopFollow();
    emit sgn_stopEncoder();
    m_pQTimer->stop();

    m_pDistanceSensorRight->slot_stopSensor();
    m_pDistanceSensorCenter->slot_stopSensor();
    m_pDistanceSensorLeft->slot_stopSensor();
    m_pColorSensor->slot_stopDetectingColor();

    m_dSpeedRatioRL = 1.0;
    m_dSpeed = m_dSpeedGUI;
    m_de_sumLeft = 0.0;
    m_de_sumRight = 0.0;
    m_nTurningDirection = 0;
    m_nCaseNumber = 0;
    m_dSpeedOld = 0;
}

/**
 * lässt die mobile Plattform vorwärts fahren
 * @brief MobilePlatform::slot_forwardMotion
 */

void MobilePlatform::slot_forwardMotion()
{
    /// drehen vorwärts beide Motoren an
    m_pLeft->forwardMotor();
    m_pRight->forwardMotor();
    m_pQTimer->start();
    emit sgn_test();
}
/**
 * lässt die mobile Plattform rückwärts fahren
 * @brief MobilePlatform::slot_backwardMotion
 */
void MobilePlatform::slot_backwardMotion()
{
    /// drehen rückbackwärts beide Motoren an
    m_pLeft->backwardMotor();
    m_pRight->backwardMotor();
    m_pQTimer->start();
    qDebug()<<"backwardMotion"<<endl;
}

/**
 * lässt die mobile Plattform links herum drehen
 * @brief MobilePlatform::slot_leftMotion
 */
void MobilePlatform::slot_leftMotion()
{
    /// drehen rückbackwärts beide Motoren an
    m_pLeft->stopMotor();
    m_pRight->forwardMotor();
    m_pQTimer->start();
    qDebug()<<"leftMotion"<<endl;
}
/**
 * lässt die mobile Plattform rechts herum drehen
 * @brief MobilePlatform::slot_rightMotion
 */
void MobilePlatform::slot_rightMotion()
{
    /// drehen rückbackwärts beide Motoren an
    m_pLeft->forwardMotor();
    m_pRight->stopMotor();
    m_pQTimer->start();
    qDebug()<<"rightMotion"<<endl;
}

/**
 * startet die Funktion "Linie folgen"
 * @brief MobilePlatform::~MobilePlatform
 */
void MobilePlatform::slot_followLine()
{
    std::cout << "Following line........" << std::endl;
    emit sgn_followLine();
}


void MobilePlatform::slot_readEncoder()
{
    emit sgn_readEncoder();
}

//PID controllers for the left and right wheel
//for driving curves, the speed ratio for the wheels and the direction are calculated other methods
//this slot is activated by m_pQTimer
void MobilePlatform::slot_updatePID()
{
    //calculate target speeds in rad/s
    double leftWheelTargetSpeed  = m_dSpeed; // in [rad/sec]
    double rightWheelTargetSpeed = m_dSpeed;
    //turn left or turn right. (0 = nothing)
    if (m_dSpeedRatioRL > 1)
    {
        m_dSpeedRatioRL = 0;
        std::cout << ("ERROR: dSpeedRatioRL > 1.0 !!!");
    }
    if (m_nTurningDirection == 1) leftWheelTargetSpeed  = m_dSpeed * m_dSpeedRatioRL;  //turn left
    if (m_nTurningDirection == 2) rightWheelTargetSpeed = m_dSpeed *m_dSpeedRatioRL;  //turn right

    //get speed/counts from the encoders
    m_nCountsLeftMotor  = m_pLeftEncoder->getEncoderValue();
    m_nCountsRightMotor = m_pRightEncoder->getEncoderValue();

    //convert counts to rad/sec
    //Bogengrad: [deg/360*2*pi=rad: ],dauraus folgt: nCountsLeftMotor/1440*360/360*2*pi=2*pi*nCountsLeftMotor/1440
    m_dSpeedLeftMotor  = 2 * 3.1416 * m_nCountsLeftMotor  / 1440.0 / m_nTimerInterval*1000.0;
    m_dSpeedRightMotor = 2 * 3.1416 * m_nCountsRightMotor / 1440.0 / m_nTimerInterval*1000.0;

    //PIDs
    m_deLeft  = leftWheelTargetSpeed - m_dSpeedLeftMotor;
    m_deRight = rightWheelTargetSpeed - m_dSpeedRightMotor;
    m_de_sumLeft  = m_de_sumLeft+m_deLeft;
    m_de_sumRight = m_de_sumRight+m_deRight;
    m_dPwmPIDLeft  =m_dKpLeft *m_deLeft;
    m_dPwmPIDRight = m_dKpRight*m_deRight;
    m_de_oldLeft  = m_deLeft;
    m_de_oldRight =m_deRight;

    //scaling to 0-255 (PWM)
    m_dPwmPIDLeft  = m_dPwmPIDLeft  * 255 / m_dMaxSpeed;
    m_dPwmPIDRight =m_dPwmPIDRight * 255 / m_dMaxSpeed;

    //limit signals to 0-255
    if (m_dPwmPIDLeft>255)  m_dPwmPIDLeft  = 255;
    if (m_dPwmPIDRight>255) m_dPwmPIDRight = 255;
    if (m_dPwmPIDLeft<0)  m_dPwmPIDLeft  = 0;
    if (m_dPwmPIDRight<0) m_dPwmPIDRight = 0;

    //write the corrected pwm signals
    m_pLeft->setPWM(m_dPwmPIDLeft);
    m_pRight->setPWM(m_dPwmPIDRight);

    //reset counters to 0
    emit sgn_setCountsLeft();
    emit sgn_setCountsRight();
}


//calculates the speed ratio for the left and the right wheel for given radius of the circle
//starts the controller for driving the circle
//radius can not be changed when mobilePlatform is driving the circle!
void MobilePlatform::slot_driveCircle()
{
    if (m_dRadius >= 13.0 && m_dRadius <= 100.0)
    {
        //calculate the ratio:
        m_dSpeedRatioRL = 1.0 + m_dWheelBase / (-0.5*m_dWheelBase - sqrt(m_dRadius*m_dRadius - m_dDistancePenToAxle));
        m_nTurningDirection = 1; //make left turn

        //start timer for PID controller
        m_pQTimer->start();

        //start motors
        m_pLeft->forwardMotor();
        m_pRight->forwardMotor();
        std::cout << "Driving circle........" << std::endl;
        std::cout << ("Speed ratio: ") << m_dSpeedRatioRL << std::endl;
    }
}


void MobilePlatform::slot_startFollowCorridor()
{
    //start timer
    m_pQTimer->start();

    //follow corridor / stay in middle of corridor
    m_nCaseNumber = 1;

    //start motors
    m_pLeft->forwardMotor();
    m_pRight->forwardMotor();

}

// follow corridor
// slot is triggerd by m_pQTimer
void MobilePlatform::slot_followCorridor()
{
    //get measured distances
    fDistanceRight  = m_pDistanceSensorRight->getSensorValue();
    fDistanceCenter = m_pDistanceSensorCenter->getSensorValue();
    fDistanceLeft   = m_pDistanceSensorLeft->getSensorValue();

    nColor = m_pColorSensor->getColor();

    //deviation from center between the 2 walls
    float fDeviation = fDistanceLeft - fDistanceRight;

    //slow down when wall is approaching
    m_dSpeedOld = m_dSpeed;
    if (fDistanceCenter < m_nDistance1) m_dSpeed = m_dSpeedGUI * ((fDistanceCenter-m_nDistance2)*(fDistanceCenter-m_nDistance2)) / ((m_nDistance1-m_nDistance2)*(m_nDistance1-m_nDistance2));
    else            m_dSpeed = m_dSpeedGUI;

    //limit acceleration
    if (m_dSpeed > (m_dSpeedOld*1.03)) m_dSpeed = m_dSpeedOld*1.03;

    //min Speed = x%
    if (m_dSpeed < m_dMaxSpeed*0.35)  m_dSpeed = m_dMaxSpeed * 0.35;

    switch(m_nCaseNumber)
    {
        //follow corridor / stay in middle of corridor
        case 1:
            //calculate SpeedRatio for left+right wheel
            m_dSpeedRatioRL = 1.0 - (abs(fDeviation) * m_fFactorSpeedRatio);

            //limit dSpeedRatioRL to 0.0-1.0
            if (m_dSpeedRatioRL < 0.7) m_dSpeedRatioRL = 0.7;

            //choose if to turn left or turn right
            if (fDeviation > 0.0) m_nTurningDirection = 1; //turn left
            else                 m_nTurningDirection = 2; //turn right

            //stop if color is red
            if (nColor == 1) m_nCaseNumber = 4;
            //if left or right are bigger than "w" --> drive left/right curve
            else if (((fDistanceLeft > nW) && (fDistanceRight < (nW-10))) or ((fDistanceLeft < (nW-10)) && (fDistanceRight > nW))) m_nCaseNumber = 2;
            break;


        //left or right curve?
        case 2:
            m_nCaseNumber = 3;
            if (nColor == 1) m_nCaseNumber = 4;
            if (fDistanceLeft > fDistanceRight) m_nTurningDirection = 1; //left turn
            else              m_nTurningDirection = 2; //right turn
            //no break necessary here. going on with case 3


        //drive curve
        case 3:
            std::cout << ("Curve.... ") << std::endl;
            //drive curve
            m_dSpeedRatioRL =m_fSpeedRatioCurve;
            m_dSpeed = m_dSpeedGUI;
            //limit speed for driving through curve
            if (m_dSpeed > 40) m_dSpeed = 40;

            //stop if color is red
            if (nColor == 1) m_nCaseNumber = 4;
            //if center is bigger X cm and platform is between the 2 walls --> go on following the corridor!
            else if ((fDistanceCenter > nLimitDistance) && (m_nTurningDirection==1) && (fDistanceLeft<(nW))) m_nCaseNumber = 1;
            else if ((fDistanceCenter > nLimitDistance) && (m_nTurningDirection==2) && (fDistanceRight<(nW))) m_nCaseNumber = 1;
            break;

        //stop at end of corridor
        case 4:

            std::cout << std::endl << std::endl << ("--- End of corridor reached! --- Mobile platform stopped! ---") << std::endl << std::endl << std::endl;
            emit sgn_endOfCorridor();
            break;

    }



}
























