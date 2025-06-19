 #pragma once

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class SerialReader : public QObject {
    Q_OBJECT

public:
    explicit SerialReader(QObject *parent = nullptr);
    void start(const QString &portName);
    void stop();

signals:
    void newSample(int ch1, int ch2);

private slots:
    void handleReadyRead();

private:
    QSerialPort serial;
    QByteArray buffer;
};

