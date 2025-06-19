#pragma once

#include <QObject>
#include <QTimer>
#include <cmath>

class DataGenerator : public QObject {
    Q_OBJECT

public:
    enum Mode { Off, Sine, DC };

    explicit DataGenerator(QObject *parent = nullptr);

    void startSine(float amp, float freq);
    void startDC(float value);
    void stop();

signals:
    void newSample(int ch1, int ch2);

private slots:
    void generate();

private:
    QTimer timer;
    float phase = 0.0f;
    float amplitude = 0.0f;
    float frequency = 0.0f;
    Mode mode = Off;
};
