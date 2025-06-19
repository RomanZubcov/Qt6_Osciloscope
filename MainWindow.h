 #pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

#include "OscilloscopeView.h"
#include "SerialReader.h"
#include "DataGenerator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startAcquisition();
    void stopAcquisition();
    void refreshPorts();
    void onVoltSliderChanged(int value);
    void onTimeSliderChanged(int value);


private:
    OscilloscopeView *view;
    SerialReader *reader;
    QSlider *timeSlider;
    QLabel *timeLabel;

    QPushButton *startButton;
    QPushButton *stopButton;
    QComboBox *portComboBox;
    DataGenerator *generator;
    QSlider *voltSlider;
    QLabel *voltLabel;

    void setupUI();
    void connectSignals();
};

