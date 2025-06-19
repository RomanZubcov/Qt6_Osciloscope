#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QSettings>
#include <QKeyEvent>

#include "OscilloscopeView.h"
#include "SerialReader.h"
#include "DataGenerator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void startAcquisition();
    void stopAcquisition();
    void refreshPorts();
    void onVoltSliderChanged(int value);
    void onTimeSliderChanged(int value);
    void onTriggerLevelChanged(int value);
    void onSaveCsv();
    void onTriggerEnabled(bool checked);
    void onDarkModeToggled(bool checked);
    void resetZoom();

private:
    OscilloscopeView *view;
    SerialReader *reader;
    DataGenerator *generator;
    DataGenerator::Mode genMode = DataGenerator::Off;
    QSlider *timeSlider;
    QLabel *timeLabel;

    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *resetButton;
    QPushButton *csvButton;
    QComboBox *portComboBox;
    QSlider *voltSlider;
    QLabel *voltLabel;
    QSlider *triggerSlider;
    QCheckBox *triggerCheck;
    QCheckBox *darkCheck;

    QSettings settings;

    void setupUI();
    void connectSignals();
    void applyPalette(bool dark);
};
