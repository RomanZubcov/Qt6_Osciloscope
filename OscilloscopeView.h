#pragma once

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QMutex>
#include <QElapsedTimer>

class OscilloscopeView : public QWidget {
    Q_OBJECT

public:
    explicit OscilloscopeView(QWidget *parent = nullptr);
    void addSample(int ch1, int ch2);
    void setTriggerLevel(int level);
    void enableTrigger(bool enabled);
    void setZoom(float zoomFactor); // unused
    void setTimeZoom(float zoomFactor);
    void setVoltZoom(float zoomFactor);
    void saveCsv(const QString &fileName, int maxSamples = 5000);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QVector<int> channel1;
    QVector<int> channel2;
    QVector<qint64> timestamps;
    QElapsedTimer elapsed;
    QMutex dataMutex;

    static constexpr int bufferSize = 5000;
    int triggerLevel = 128;
    bool triggerEnabled = false;
    bool triggered = false;
    int prevCh1 = 0;
    float timeZoom = 1.0f;
    float voltZoom = 1.0f;
    float timeOffset = 0.0f; // for pan
    bool panning = false;
    int lastPanX = 0;

    QTimer updateTimer;

    void drawWaveform(QPainter &painter);
    void drawGrid(QPainter &painter);

    static constexpr float timePerDiv = 10.0f; // în ms/div
    static constexpr int verticalDivs = 8;
    static constexpr int horizontalDivs = 10;
    void drawAxes(QPainter &painter);
};
