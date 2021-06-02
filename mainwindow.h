#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mobileplatform.h>
#include <linesensor.h>
#include <encoder.h>
#include <iostream>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * Diese Klasse enthält sämtliche Elemente der Benutzeroberfläche (.ui Form).
 * Die Benutzeroberfläche besitzt verschiedene Buttons und andere Elemente zur
 * Steuerung der mobilen Plattform.
 * @brief The MainWindow class
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MobilePlatform* platform, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MobilePlatform *m_pMobilePlatform;

private slots:
    void slot_stop();
    void slot_driveManual();
    void slot_driveAutomatic();
    void slot_sensorsActive();

};

#endif // MAINWINDOW_H









