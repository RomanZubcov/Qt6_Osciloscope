#include "SerialReader.h"
#include <QStringList>

SerialReader::SerialReader(QObject *parent) : QObject(parent) {
    connect(&serial, &QSerialPort::readyRead, this, &SerialReader::handleReadyRead);
}

void SerialReader::start(const QString &portName) {
    if (serial.isOpen()) serial.close();

    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.open(QIODevice::ReadOnly);
}

void SerialReader::stop() {
    if (serial.isOpen())
        serial.close();
}

void SerialReader::handleReadyRead() {
    buffer += serial.readAll();

    while (true) {
        int newlineIndex = buffer.indexOf('\n');
        if (newlineIndex < 0) break;

        QByteArray line = buffer.left(newlineIndex).trimmed();
        buffer.remove(0, newlineIndex + 1);

        QList<QByteArray> parts = line.split(',');
        if (parts.size() == 2) {
            bool ok1, ok2;
            int ch1 = parts[0].toInt(&ok1);
            int ch2 = parts[1].toInt(&ok2);
            if (ok1 && ok2)
                emit newSample(ch1, ch2);
        }
    }
}

