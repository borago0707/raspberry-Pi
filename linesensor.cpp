#include "linesensor.h"

LineSensor::LineSensor()
{
    m_bIsRunning = false;
    moveToThread(&m_threadLine);
    m_threadLine.start();
}

LineSensor::~LineSensor()
{
    m_bIsRunning = false;
    if (m_threadLine.isRunning())
    {
        m_threadLine.terminate();
        m_threadLine.wait();
    }
}

//read sensors, calculate position and correct moving direction to follow the line
void LineSensor::slot_followLine()
{
    m_bIsRunning = true;
    std::cout << "following line......................." << std::endl;

    while(m_bIsRunning)
    {
        /// read value for left and right line sensor
        m_nValueC = digitalRead(0); // 1=black, 0=white
        m_nValueR = digitalRead(2);

        /// calculate "position"
        /// mobile platform is on the line
        if (m_nValueC == 0 && m_nValueR == 1)
        {
            m_nPosition = 1;
        }

        /// mobile platform is leaving the line on the right side
        else if ( (m_nValueC == 1 && m_nValueR == 1))
        {
            m_nPosition = 2;
        }

        /// mobile platform is leaving the line on the left side
        else if ((m_nValueC == 0 && m_nValueR == 0) || (m_nValueC == 1 && m_nValueR == 0))
        {
            m_nPosition = 3;
        }

        ///corrects the moving direction of the mobile platform
        /// if platform is left off the line it goes right etc.
        switch (m_nPosition)
        {
        case 1:
            //Mittig auf der Linie
            emit sgn_goStraight();
            break;

        case 2:
            //Rechts daneben
            emit sgn_goLeft();
            break;

        case 3:
            //Links daneben;
            emit sgn_goRight();
            break;
        }

        // sleep for x msec
        m_threadLine.msleep(1);
        QCoreApplication::processEvents();
    }
}

//stop following the line
void LineSensor::slot_stopFollow()
{
    if (m_bIsRunning)
    {
        //stops the follow-line-loop
        std::cout << "....................stopped following line" << std::endl;
        m_bIsRunning = false;
    }
}


