#include "MainWindow.h"
#include <QSerialPortInfo>
#include <QWidget>
#include <QLabel>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      view(new OscilloscopeView),
      reader(new SerialReader(this)),
      generator(new DataGenerator(this)),
      settings("QtOsc", "Oscilloscope")
{
    setupUI();
    connectSignals();
    refreshPorts();

    bool dark = settings.value("dark", true).toBool();
    darkCheck->setChecked(dark);
    applyPalette(dark);
}

MainWindow::~MainWindow() {
    reader->stop();
    generator->stop();
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    view->setMinimumHeight(400);
    mainLayout->addWidget(view);

    QHBoxLayout *controls = new QHBoxLayout;

    QLabel *portLabel = new QLabel("Serial Port:");
    portComboBox = new QComboBox;
    QPushButton *refreshButton = new QPushButton("Refresh");

    startButton = new QPushButton("▶ Start");
    stopButton  = new QPushButton("■ Stop");
    resetButton = new QPushButton("Reset Zoom");
    csvButton   = new QPushButton("Save CSV");

    triggerCheck = new QCheckBox("Trigger");
    triggerSlider = new QSlider(Qt::Vertical);
    triggerSlider->setRange(0, 255);
    triggerSlider->setValue(128);

    voltSlider = new QSlider(Qt::Vertical);
    voltSlider->setRange(1, 10);
    voltSlider->setValue(1);
    voltSlider->setTickPosition(QSlider::TicksRight);

    voltLabel = new QLabel("Volt/Div");
    QVBoxLayout *voltLayout = new QVBoxLayout;
    voltLayout->addWidget(voltLabel, 0, Qt::AlignHCenter);
    voltLayout->addWidget(voltSlider);

    QVBoxLayout *trigLayout = new QVBoxLayout;
    trigLayout->addWidget(triggerCheck, 0, Qt::AlignHCenter);
    trigLayout->addWidget(triggerSlider);

    darkCheck = new QCheckBox("Dark Mode");

    controls->addWidget(portLabel);
    controls->addWidget(portComboBox);
    controls->addWidget(refreshButton);
    controls->addWidget(startButton);
    controls->addWidget(stopButton);
    controls->addWidget(resetButton);
    controls->addWidget(csvButton);
    controls->addLayout(voltLayout);
    controls->addLayout(trigLayout);
    controls->addWidget(darkCheck);

    mainLayout->addLayout(controls);

    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setRange(1, 10);
    timeSlider->setValue(1);
    timeSlider->setTickPosition(QSlider::TicksBelow);

    timeLabel = new QLabel("Time/Div: x1");

    QVBoxLayout *timeLayout = new QVBoxLayout;
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeSlider);
    mainLayout->addLayout(timeLayout);

    central->setLayout(mainLayout);
    setCentralWidget(central);
    setWindowTitle("Qt6 Osciloscop");
    resize(1000, 600);

    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshPorts);
}

void MainWindow::connectSignals() {
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startAcquisition);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopAcquisition);
    connect(reader, &SerialReader::newSample, view, &OscilloscopeView::addSample);
    connect(generator, &DataGenerator::newSample, view, &OscilloscopeView::addSample);
    connect(voltSlider, &QSlider::valueChanged, this, &MainWindow::onVoltSliderChanged);
    connect(timeSlider, &QSlider::valueChanged, this, &MainWindow::onTimeSliderChanged);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetZoom);
    connect(csvButton, &QPushButton::clicked, this, &MainWindow::onSaveCsv);
    connect(triggerSlider, &QSlider::valueChanged, this, &MainWindow::onTriggerLevelChanged);
    connect(triggerCheck, &QCheckBox::toggled, this, &MainWindow::onTriggerEnabled);
    connect(darkCheck, &QCheckBox::toggled, this, &MainWindow::onDarkModeToggled);
}

void MainWindow::startAcquisition() {
    QString port = portComboBox->currentText();
    if (!port.isEmpty()) {
        reader->start(port);
    }
}

void MainWindow::stopAcquisition() {
    reader->stop();
}

void MainWindow::refreshPorts() {
    portComboBox->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        portComboBox->addItem(info.portName());
    }
}

void MainWindow::onVoltSliderChanged(int value) {
    float zoom = value / 1.0f;
    view->setVoltZoom(zoom);
    voltLabel->setText(QString("Volt/Div: x%1").arg(value));
}

void MainWindow::onTimeSliderChanged(int value) {
    float zoom = value / 1.0f;
    view->setTimeZoom(zoom);
    timeLabel->setText(QString("Time/Div: x%1").arg(value));
}

void MainWindow::onTriggerLevelChanged(int value) {
    view->setTriggerLevel(value);
}

void MainWindow::onTriggerEnabled(bool checked) {
    view->enableTrigger(checked);
}

void MainWindow::resetZoom() {
    view->setTimeZoom(1.0f);
    view->setVoltZoom(1.0f);
    timeSlider->setValue(1);
    voltSlider->setValue(1);
    timeLabel->setText("Time/Div: x1");
    voltLabel->setText("Volt/Div: x1");
}

void MainWindow::onSaveCsv() {
    QString file = QFileDialog::getSaveFileName(this, "Save CSV", QString(), "CSV Files (*.csv)");
    if (!file.isEmpty()) {
        view->saveCsv(file, 5000);
    }
}

void MainWindow::applyPalette(bool dark) {
    QPalette pal;
    if (dark) {
        pal.setColor(QPalette::Window, QColor("#121212"));
        pal.setColor(QPalette::WindowText, Qt::white);
        pal.setColor(QPalette::Mid, QColor(255,255,255,40));
        pal.setColor(QPalette::Midlight, QColor(255,255,255,100));
        pal.setColor(QPalette::Highlight, Qt::green);
        pal.setColor(QPalette::Link, Qt::red);
        pal.setColor(QPalette::Button, QColor("#333333"));
        pal.setColor(QPalette::ButtonText, Qt::white);
    } else {
        pal = QPalette();
    }
    qApp->setPalette(pal);
    settings.setValue("dark", dark);
}

void MainWindow::onDarkModeToggled(bool checked) {
    applyPalette(checked);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_S) {
        if (genMode == DataGenerator::Sine) {
            generator->stop();
            genMode = DataGenerator::Off;
        } else {
            generator->startSine(230.0f, 50.0f);
            genMode = DataGenerator::Sine;
        }
    } else if (event->key() == Qt::Key_D) {
        if (genMode == DataGenerator::DC) {
            generator->stop();
            genMode = DataGenerator::Off;
        } else {
            generator->startDC(12.0f);
            genMode = DataGenerator::DC;
        }
    }
    QMainWindow::keyPressEvent(event);
}
