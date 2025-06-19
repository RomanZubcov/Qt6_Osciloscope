 #include "OscilloscopeView.h"
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QWheelEvent>

OscilloscopeView::OscilloscopeView(QWidget *parent) : QWidget(parent) {
    channel1.fill(0, bufferSize);
    channel2.fill(0, bufferSize);

    connect(&updateTimer, &QTimer::timeout, this, QOverload<>::of(&OscilloscopeView::update));
    updateTimer.start(30); // 30 ms ≈ 33 FPS
}

void OscilloscopeView::addSample(int ch1, int ch2) {
    QMutexLocker locker(&dataMutex);
    channel1.push_back(ch1);
    channel2.push_back(ch2);
    if (channel1.size() > bufferSize) channel1.pop_front();
    if (channel2.size() > bufferSize) channel2.pop_front();
}

void OscilloscopeView::setTriggerLevel(int level) {
    triggerLevel = level;
}

void OscilloscopeView::setZoom(float zoomFactor) {
    timeZoom = zoomFactor; // sau voltZoom dacă se referă la axa Y
}

void OscilloscopeView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    drawWaveform(painter);
    drawGrid(painter);
    drawAxes(painter);
}

void OscilloscopeView::drawWaveform(QPainter &painter) {
    QMutexLocker locker(&dataMutex);
    int w = width();
    int h = height();
    int midY = h / 2;
    int numSamples = channel1.size();

    if (numSamples < 2) return;

    float xScale = static_cast<float>(w) / bufferSize * timeZoom;
    float yScale = voltZoom;
    float offsetPx = timeOffset * xScale;


    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen1(Qt::green, 1);
    QPen pen2(Qt::red, 1);

    // Canal 1
    painter.setPen(pen1);
    for (int i = 1; i < numSamples; ++i) {
        float x1 = (i - 1) * xScale - offsetPx;
        float x2 = i * xScale - offsetPx;
        if (x2 < 0 || x1 > w) continue;
        painter.drawLine(QPointF(x1, midY - channel1[i - 1] * yScale),
                         QPointF(x2, midY - channel1[i] * yScale));
    }

    // Canal 2
    painter.setPen(pen2);
    for (int i = 1; i < numSamples; ++i) {
        float x1 = (i - 1) * xScale - offsetPx;
        float x2 = i * xScale - offsetPx;
        if (x2 < 0 || x1 > w) continue;
        painter.drawLine(QPointF(x1, midY - channel2[i - 1] * yScale),
                         QPointF(x2, midY - channel2[i] * yScale));
    }


}
void OscilloscopeView::setTimeZoom(float zoomFactor) {
    timeZoom = zoomFactor;
}
void OscilloscopeView::setVoltZoom(float zoomFactor) {
    voltZoom = zoomFactor;
}

void OscilloscopeView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        panning = true;
        lastPanX = event->pos().x();
    }
}

void OscilloscopeView::mouseMoveEvent(QMouseEvent *event) {
    if (panning) {
        int dx = event->pos().x() - lastPanX;
        lastPanX = event->pos().x();
        float xScale = static_cast<float>(width()) / bufferSize * timeZoom;
        timeOffset -= dx / xScale;
        if (timeOffset < 0) timeOffset = 0;
        if (timeOffset > bufferSize) timeOffset = bufferSize;
        update();
    }
}

void OscilloscopeView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        panning = false;
    }
}

void OscilloscopeView::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0)
        timeZoom *= 1.1f;
    else
        timeZoom /= 1.1f;
    if (timeZoom < 0.1f) timeZoom = 0.1f;
    if (timeZoom > 10.f) timeZoom = 10.f;
    update();
    event->accept();
}
void OscilloscopeView::drawGrid(QPainter &painter) {
    int w = width();
    int h = height();

    int numVertLines = 10;
    int numHorizLines = 8;

    QPen gridPen(QColor(255, 255, 255, 40)); // alb semi-transparent
    gridPen.setStyle(Qt::DotLine);
    painter.setPen(gridPen);

    // Linii verticale
    for (int i = 1; i < numVertLines; ++i) {
        int x = i * w / numVertLines;
        painter.drawLine(x, 0, x, h);
    }

    // Linii orizontale
    for (int i = 1; i < numHorizLines; ++i) {
        int y = i * h / numHorizLines;
        painter.drawLine(0, y, w, y);
    }

    // Linie centrală
    painter.setPen(QPen(QColor(255, 255, 255, 100), 1));
    painter.drawLine(0, h / 2, w, h / 2);
}

void OscilloscopeView::drawAxes(QPainter &painter) {
    painter.setPen(QColor(180, 180, 180));
    painter.setFont(QFont("Consolas", 8));

    int w = width();
    int h = height();

    // Timp / div pe X
    float totalTimeMs = timePerDiv * horizontalDivs * timeZoom;
    for (int i = 0; i <= horizontalDivs; ++i) {
        float t = (totalTimeMs * i) / horizontalDivs;
        int x = i * w / horizontalDivs;
        painter.drawText(x + 2, h - 5, QString("%1 ms").arg(t, 0, 'f', 0));
    }

    // Volt/div pe Y (cu centru pe 0)
    for (int i = 0; i <= verticalDivs; ++i) {
        int y = i * h / verticalDivs;
        int value = static_cast<int>((verticalDivs / 2 - i) * 1000 / voltZoom);
        if (i != verticalDivs / 2)
            painter.drawText(4, y - 2, QString::number(value));
        else
            painter.drawText(4, y - 2, "0");
    }
}
