#include "CountdownTimer.h"

CountdownTimer::CountdownTimer(QWidget* parent, int minutes, int seconds)
    : QWidget(parent), countdownTime(0, minutes, seconds), totalTime(0, minutes, seconds + 1)       // Buffer 1 seconde voor als je de timer reset dat het systeem tijd heeft om te resetten (bv. 1 min gaat anders 59 seconde weergeven)
{
    // Zet de label van de timer goed
    timerLabel = new QLabel(this);
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setText(countdownTime.toString("mm:ss"));

    // Zet de timer goed
    connect(&timer, &QTimer::timeout, this, &CountdownTimer::updateCountdown);
    timer.setInterval(1000);

    // Zet de layout goed
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(timerLabel);
    setLayout(layout);
}

// Update de label van de timer
void CountdownTimer::updateCountdown()
{
    // De tijd is afgelopen, dus de timer is op 0 uur 0 minuten 0 seconde
    if (countdownTime == QTime(0, 0, 0)) {
        timer.stop();
        timerLabel->setText("Time's up!");
        emit countdownFinished();
    }
    else {
        countdownTime = countdownTime.addSecs(-1);              // Er is 1 sec verlopen
        timerLabel->setText(countdownTime.toString("mm:ss"));
    }
}
