#include "DataGenerator.h"

DataGenerator::DataGenerator(QObject *parent) : QObject(parent) {
    connect(&timer, &QTimer::timeout, this, &DataGenerator::generate);
}

void DataGenerator::startSine(float amp, float freq) {
    amplitude = amp;
    frequency = freq;
    mode = Sine;
    phase = 0.0f;
    timer.start(1); // 1ms interval for better resolution
}

void DataGenerator::startDC(float value) {
    amplitude = value;
    frequency = 0.0f;
    mode = DC;
    timer.start(10);
}

void DataGenerator::stop() {
    timer.stop();
    mode = Off;
}

void DataGenerator::generate() {
    int sample = 0;
    if (mode == Sine) {
        float dt = timer.interval() / 1000.0f;
        phase += 2.0f * M_PI * frequency * dt;
        sample = static_cast<int>(amplitude * std::sin(phase) * 100);
    } else if (mode == DC) {
        sample = static_cast<int>(amplitude * 100);
    } else {
        return;
    }
    emit newSample(sample, sample);
}
