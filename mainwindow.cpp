#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (MobilePlatform *platform, QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // check ob nicht NULL
    if(platform)
    {
        m_pMobilePlatform = platform;
        //speed
        connect(ui->qSpeedSpinBox, SIGNAL(valueChanged(int)), m_pMobilePlatform, SLOT(slot_getSpeedGUI(int)));
        //stop button
        connect(ui->qStopButton,    &QPushButton::clicked,  m_pMobilePlatform, &MobilePlatform::slot_stopMotion);
        // manual control
        connect(ui->qForwardButton, &QPushButton::clicked,  m_pMobilePlatform, &MobilePlatform::slot_forwardMotion);
        connect(ui->qBackwardButton, &QPushButton::clicked, m_pMobilePlatform, &MobilePlatform::slot_backwardMotion);
        connect(ui->qLeftButton, &QPushButton::clicked,     m_pMobilePlatform, &MobilePlatform::slot_leftMotion);
        connect(ui->qRightButton, &QPushButton::clicked,    m_pMobilePlatform, &MobilePlatform::slot_rightMotion);

        connect(ui->qForwardButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_readEncoder()));
        connect(ui->qLeftButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_readEncoder()));
        connect(ui->qRightButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_readEncoder()));
        connect(ui->qBackwardButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_readEncoder()));

        connect(ui->qForwardButton, SIGNAL(clicked()), this, SLOT(slot_driveManual()));
        connect(ui->qBackwardButton, SIGNAL(clicked()), this, SLOT(slot_driveManual()));
        connect(ui->qLeftButton, SIGNAL(clicked()), this, SLOT(slot_driveManual()));
        connect(ui->qRightButton, SIGNAL(clicked()), this, SLOT(slot_driveManual()));





        //enable/disable buttons in GUI
        connect(ui->qStopButton, SIGNAL(clicked()), this, SLOT(slot_stop()));
        connect(m_pMobilePlatform, SIGNAL(sgn_endOfCorridor()), this, SLOT(slot_stop()));

        connect(ui->qLinieFolgenButton, SIGNAL(clicked()), this, SLOT(slot_driveAutomatic()));
        connect(ui->qDriveCircleButton, SIGNAL(clicked()), this, SLOT(slot_driveAutomatic()));
        connect(ui->qFollowCorridorButton, SIGNAL(clicked()), this, SLOT(slot_driveAutomatic()));
        connect(ui->qActivateSensorsButton, SIGNAL(clicked()), this, SLOT(slot_sensorsActive()));



        //follow line
        connect(ui->qLinieFolgenButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_followLine()));
        //drive circle
        connect(ui->qDriveCircleButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_driveCircle()));
        connect(ui->qRadiusSpinBox, SIGNAL(valueChanged(double)), m_pMobilePlatform, SLOT(slot_getRadiusGUI(double)));

        //sensors
        connect(ui->qActivateSensorsButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_activateSensors()));
        connect(ui->qActivateSensorsButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_readEncoder()));

        //follow corridor
        connect(ui->qFollowCorridorButton, SIGNAL(clicked()), m_pMobilePlatform, SLOT(slot_startFollowCorridor()));

    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

//disable buttons if platform is driving automatically
void MainWindow::slot_driveAutomatic()
{
    ui->qForwardButton->setEnabled(false);
    ui->qBackwardButton->setEnabled(false);
    ui->qLeftButton->setEnabled(false);
    ui->qRightButton->setEnabled(false);
    ui->qLinieFolgenButton->setEnabled(false);
    ui->qDriveCircleButton->setEnabled(false);
    ui->qActivateSensorsButton->setEnabled(false);
    ui->qFollowCorridorButton->setEnabled(false);
    ui->qRadiusSpinBox->setEnabled(false);
    ui->qSpeedSpinBox->setEnabled(false);

    ui->qStopButton->setEnabled(true);
}


//disable buttons when platform is driving manually
void MainWindow::slot_driveManual()
{
    ui->qLinieFolgenButton->setEnabled(false);
    ui->qDriveCircleButton->setEnabled(false);
    ui->qActivateSensorsButton->setEnabled(false);
    ui->qFollowCorridorButton->setEnabled(false);
    ui->qRadiusSpinBox->setEnabled(false);

    ui->qStopButton->setEnabled(true);
}


//follow corridor is only possible if sensors are activated
void MainWindow::slot_sensorsActive()
{
    ui->qFollowCorridorButton->setEnabled(true);
    ui->qStopButton->setEnabled(true);
    ui->qActivateSensorsButton->setEnabled(false);
}


//enable buttons if platform is stopped
void MainWindow::slot_stop()
{
    ui->qForwardButton->setEnabled(true);
    ui->qBackwardButton->setEnabled(true);
    ui->qLeftButton->setEnabled(true);
    ui->qRightButton->setEnabled(true);
    ui->qLinieFolgenButton->setEnabled(true);
    ui->qDriveCircleButton->setEnabled(true);
    ui->qActivateSensorsButton->setEnabled(true);
    ui->qRadiusSpinBox->setEnabled(true);
    ui->qSpeedSpinBox->setEnabled(true);

    ui->qFollowCorridorButton->setEnabled(false);
    ui->qStopButton->setEnabled(false);
}

