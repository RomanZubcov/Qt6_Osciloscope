#include "MainWindow.h"
#include <QSerialPortInfo>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), view(new OscilloscopeView), reader(nullptr), generator(new DataGenerator)
 {
    setupUI();
    connectSignals();
    refreshPorts();
}

MainWindow::~MainWindow() {
    reader->stop();
    delete reader;
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Fundal închis pentru tot
    central->setStyleSheet("background-color: #121212; color: white;");

    // Osciloscop view (central)
    view->setMinimumHeight(400);
    mainLayout->addWidget(view);

    // Control Panel
    QHBoxLayout *controls = new QHBoxLayout;

    QLabel *portLabel = new QLabel("Serial Port:");
    portLabel->setStyleSheet("color: white;");
    portComboBox = new QComboBox;
    portComboBox->setStyleSheet("background-color: #1e1e1e; color: white;");

    startButton = new QPushButton("▶ Start");
    stopButton  = new QPushButton("■ Stop");
    resetButton = new QPushButton("Reset Zoom");

    QString btnStyle = "QPushButton { padding: 8px 16px; background-color: #007acc; color: white; border: none; border-radius: 4px; }"
                       "QPushButton:hover { background-color: #005f99; }";

    startButton->setStyleSheet(btnStyle);
    stopButton->setStyleSheet(btnStyle);
    resetButton->setStyleSheet(btnStyle);

    controls->addWidget(portLabel);
    controls->addWidget(portComboBox);
    controls->addWidget(startButton);
    controls->addWidget(stopButton);
    controls->addWidget(resetButton);

    // Slider Volt/Div
    voltSlider = new QSlider(Qt::Vertical);
    voltSlider->setRange(1, 10); // Zoom de la 1x la 10x
    voltSlider->setValue(1);
    voltSlider->setTickPosition(QSlider::TicksRight);

    voltLabel = new QLabel("Volt/Div");
    voltLabel->setStyleSheet("color: white;");
    QVBoxLayout *voltLayout = new QVBoxLayout;
    voltLayout->addWidget(voltLabel, 0, Qt::AlignHCenter);
    voltLayout->addWidget(voltSlider);
    controls->addLayout(voltLayout);

    mainLayout->addLayout(controls);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    setWindowTitle("Qt6 Osciloscop");
    resize(1000, 600);


    // Time/Div Slider (zoom pe X)
    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setRange(1, 10); // 1x la 10x zoom
    timeSlider->setValue(1);
    timeSlider->setTickPosition(QSlider::TicksBelow);

    timeLabel = new QLabel("Time/Div: x1");
    timeLabel->setStyleSheet("color: white;");

    QVBoxLayout *timeLayout = new QVBoxLayout;
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeSlider);
    mainLayout->addLayout(timeLayout); // sub control panelul de sus

}

void MainWindow::connectSignals() {
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startAcquisition);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopAcquisition);
    connect(reader, &SerialReader::newSample, view, &OscilloscopeView::addSample);
    connect(voltSlider, &QSlider::valueChanged, this, &MainWindow::onVoltSliderChanged);
    connect(generator, &DataGenerator::newSample, view, &OscilloscopeView::addSample);
    connect(timeSlider, &QSlider::valueChanged, this, &MainWindow::onTimeSliderChanged);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetZoom);

}

void MainWindow::startAcquisition() {
    QString port = portComboBox->currentText();
    if (!port.isEmpty()) {
        generator->start();
    }
}

void MainWindow::stopAcquisition() {
    generator->stop();

}

void MainWindow::refreshPorts() {
    portComboBox->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        portComboBox->addItem(info.portName());
    }
}
void MainWindow::onVoltSliderChanged(int value) {
    float zoom = value / 1.0f; // Zoom vertical (1x - 10x)
    view->setVoltZoom(zoom);
    voltLabel->setText(QString("Volt/Div: x%1").arg(value));
}
void MainWindow::onTimeSliderChanged(int value) {
    float zoom = value / 1.0f;
    view->setTimeZoom(zoom);
    timeLabel->setText(QString("Time/Div: x%1").arg(value));
}

void MainWindow::resetZoom() {
    view->setTimeZoom(1.0f);
    view->setVoltZoom(1.0f);
    timeSlider->setValue(1);
    voltSlider->setValue(1);
    timeLabel->setText("Time/Div: x1");
    voltLabel->setText("Volt/Div: x1");
}
