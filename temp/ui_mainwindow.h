/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *qForwardButton;
    QPushButton *qLeftButton;
    QPushButton *qBackwardButton;
    QPushButton *qRightButton;
    QPushButton *qStopButton;
    QPushButton *qLinieFolgenButton;
    QPushButton *qDriveCircleButton;
    QPushButton *qActivateSensorsButton;
    QPushButton *qFollowCorridorButton;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *qRadiusSpinBox;
    QDoubleSpinBox *qSpeedSpinBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        qForwardButton = new QPushButton(centralwidget);
        qForwardButton->setObjectName(QString::fromUtf8("qForwardButton"));
        qForwardButton->setGeometry(QRect(190, 10, 89, 25));
        qLeftButton = new QPushButton(centralwidget);
        qLeftButton->setObjectName(QString::fromUtf8("qLeftButton"));
        qLeftButton->setGeometry(QRect(90, 60, 89, 25));
        qBackwardButton = new QPushButton(centralwidget);
        qBackwardButton->setObjectName(QString::fromUtf8("qBackwardButton"));
        qBackwardButton->setGeometry(QRect(190, 110, 89, 25));
        qRightButton = new QPushButton(centralwidget);
        qRightButton->setObjectName(QString::fromUtf8("qRightButton"));
        qRightButton->setGeometry(QRect(290, 60, 89, 25));
        qStopButton = new QPushButton(centralwidget);
        qStopButton->setObjectName(QString::fromUtf8("qStopButton"));
        qStopButton->setGeometry(QRect(60, 160, 341, 101));
        qLinieFolgenButton = new QPushButton(centralwidget);
        qLinieFolgenButton->setObjectName(QString::fromUtf8("qLinieFolgenButton"));
        qLinieFolgenButton->setGeometry(QRect(80, 300, 141, 41));
        qDriveCircleButton = new QPushButton(centralwidget);
        qDriveCircleButton->setObjectName(QString::fromUtf8("qDriveCircleButton"));
        qDriveCircleButton->setGeometry(QRect(80, 360, 141, 41));
        qActivateSensorsButton = new QPushButton(centralwidget);
        qActivateSensorsButton->setObjectName(QString::fromUtf8("qActivateSensorsButton"));
        qActivateSensorsButton->setGeometry(QRect(80, 420, 141, 41));
        qFollowCorridorButton = new QPushButton(centralwidget);
        qFollowCorridorButton->setObjectName(QString::fromUtf8("qFollowCorridorButton"));
        qFollowCorridorButton->setGeometry(QRect(80, 470, 141, 41));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(310, 300, 121, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(260, 510, 171, 31));
        qRadiusSpinBox = new QDoubleSpinBox(centralwidget);
        qRadiusSpinBox->setObjectName(QString::fromUtf8("qRadiusSpinBox"));
        qRadiusSpinBox->setGeometry(QRect(440, 290, 121, 51));
        qSpeedSpinBox = new QDoubleSpinBox(centralwidget);
        qSpeedSpinBox->setObjectName(QString::fromUtf8("qSpeedSpinBox"));
        qSpeedSpinBox->setGeometry(QRect(450, 480, 121, 51));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        qForwardButton->setText(QCoreApplication::translate("MainWindow", "Vorw\303\244rts", nullptr));
        qLeftButton->setText(QCoreApplication::translate("MainWindow", "Links", nullptr));
        qBackwardButton->setText(QCoreApplication::translate("MainWindow", "R\303\274ckw\303\244rts", nullptr));
        qRightButton->setText(QCoreApplication::translate("MainWindow", "Rechts", nullptr));
        qStopButton->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        qLinieFolgenButton->setText(QCoreApplication::translate("MainWindow", "Linie folgen", nullptr));
        qDriveCircleButton->setText(QCoreApplication::translate("MainWindow", "Kreis fahren", nullptr));
        qActivateSensorsButton->setText(QCoreApplication::translate("MainWindow", "Sensoren aktivieren", nullptr));
        qFollowCorridorButton->setText(QCoreApplication::translate("MainWindow", "Korridor folgen", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Radius [cm]", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Geschwindigkeit [%]", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
