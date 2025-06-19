#pragma once

#include <QObject>
#include <QTimer>
#include <cmath>

class DataGenerator : public QObject {
    Q_OBJECT

public:
    explicit DataGenerator(QObject *parent = nullptr);
    void start();
    void stop();
    void setAmplitude(int amp);

signals:
    void newSample(int ch1, int ch2);

private slots:
    void generate();

private:
    QTimer timer;
    float phase = 0.0f;
    int amplitude = 5;
};
