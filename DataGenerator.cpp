<<<<<<< HEAD
#include "DataGenerator.h"

DataGenerator::DataGenerator(QObject *parent) : QObject(parent) {
    connect(&timer, &QTimer::timeout, this, &DataGenerator::generate);
}

void DataGenerator::start() {
    timer.start(10); // generație la 100Hz
}

void DataGenerator::stop() {
    timer.stop();
}

void DataGenerator::setAmplitude(int amp) {
    amplitude = amp;
}

void DataGenerator::generate() {
    int ch1 = static_cast<int>(amplitude * std::sin(phase) * 100);
    int ch2 = static_cast<int>(amplitude * std::cos(phase) * 100);
    phase += 0.1f;
    emit newSample(ch1, ch2);
}
=======
#include "DataGenerator.h"

DataGenerator::DataGenerator(QObject *parent) : QObject(parent) {
    connect(&timer, &QTimer::timeout, this, &DataGenerator::generate);
}

void DataGenerator::start() {
    timer.start(10); // generație la 100Hz
}

void DataGenerator::stop() {
    timer.stop();
}

void DataGenerator::setAmplitude(int amp) {
    amplitude = amp;
}

void DataGenerator::generate() {
    int ch1 = static_cast<int>(amplitude * std::sin(phase) * 100);
    int ch2 = static_cast<int>(amplitude * std::cos(phase) * 100);
    phase += 0.1f;
    emit newSample(ch1, ch2);
}
>>>>>>> a07bf278d29211da5135a3cba45d86bfeef745c8
