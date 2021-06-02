#include "color_sensor.h"

color_sensor::color_sensor()
{
    m_bIsRunning = false;

    moveToThread(&m_threadColorSensor);
    m_threadColorSensor.start();

    m_nDeviceHandleId = wiringPiI2CSetup(0x29);
    wiringPiI2CWriteReg8(m_nDeviceHandleId, 0x80|0x00, 0x01|0x02);
    wiringPiI2CWriteReg8(m_nDeviceHandleId, 0x80|0x01, 0xc0);
    wiringPiI2CWriteReg8(m_nDeviceHandleId, 0x80|0x03, 0xff);

    m_nColor = 0;

}

color_sensor::~color_sensor()
{
    m_bIsRunning = false;
    if (m_threadColorSensor.isRunning())
    {
        m_threadColorSensor.terminate();
        m_threadColorSensor.wait();
    }
}

//return color
int color_sensor::getColor()
{
    m_mutexColorSet.lock();
    int nTmp=m_nColor; //red=1; green=2; blue=3; else=0
    m_mutexColorSet.unlock();
    return nTmp;
}

//detect color
void color_sensor::slot_detectColor()
{
    m_bIsRunning = true;
    std::cout << ("---Farbsensor aktiviert---") << std::endl;

    int nRed = 0;
    int nRed2 = 0;
    int nGreen = 0;
    int nGreen2 = 0;
    int nBlue = 0;
    int nBlue2 = 0;

    while(m_bIsRunning == true)
    {
        //read registers / color channels
        nRed = wiringPiI2CReadReg16(m_nDeviceHandleId, 0x80|0x16);
        nGreen = wiringPiI2CReadReg16(m_nDeviceHandleId, 0x80|0x18);
        nBlue = wiringPiI2CReadReg16(m_nDeviceHandleId, 0x80|0x1A);

        //normalize values to make measurement more immune to changing lighting conditions
        nRed2 = nRed*1000 / (nRed+nGreen+nBlue);
        nGreen2 = nGreen*1000 / (nRed+nGreen+nBlue);
        nBlue2 = nBlue*1000 / (nRed+nGreen+nBlue);

        //resulting color
        m_mutexColorSet.lock();
        if      ((nRed2>580) && (nGreen2<250) && (nBlue2<190)) m_nColor = 1; //red
        else if ((nRed2<450) && (nGreen2>350) && (nBlue2<250)) m_nColor = 2; //green
        else if ((nRed2<400) && (nGreen2<400) && (nBlue2>280)) m_nColor = 3; //blue
        else m_nColor = 0; //standard
        m_mutexColorSet.unlock();

        m_threadColorSensor.msleep(10);
        QCoreApplication::processEvents();
    }
}



//stops the slot "slot_detectColor()"
void color_sensor::slot_stopDetectingColor()
{
    m_bIsRunning=false;
    std::cout << ("---Farbsensor deaktiviert---") << std::endl;
}
