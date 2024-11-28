#include "CountdownTimer.h"

CountdownTimer::CountdownTimer(QWidget* parent) : QWidget(parent)
{
    // Zet de label van de timer goed
    m_timerLabel = new QLabel(this);
    m_timerLabel->setAlignment(Qt::AlignCenter);
    m_timerLabel->setText(m_countdownTime.toString("mm:ss"));

    // Zet de timer goed
    connect(&m_timer, &QTimer::timeout, this, &CountdownTimer::updateCountdown);
    m_timer.setInterval(1000);

    // Zet de layout goed
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_timerLabel);
    setLayout(m_mainLayout);
}

// Update de label van de timer
void CountdownTimer::updateCountdown()
{
    // De tijd is afgelopen, dus de timer is op 0 uur 0 minuten 0 seconde
    if (m_countdownTime == QTime(0, 0, 0)) {
        m_timer.stop();
        m_timerLabel->setText("Time's up!");
        emit countdownFinished();
    }
    else {
        m_countdownTime = m_countdownTime.addSecs(-1);              // Er is 1 sec verlopen
        m_timerLabel->setText(m_countdownTime.toString("mm:ss"));
    }
}

void CountdownTimer::setupTimer(QTime time) {
    m_countdownTime = time;
    m_totalTime = time;
}