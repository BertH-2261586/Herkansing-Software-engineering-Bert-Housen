#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTime>

class CountdownTimer : public QWidget
{
    Q_OBJECT

public:
    explicit CountdownTimer(QWidget* parent, int minutes, int seconds = 0);
    ~CountdownTimer() { delete timerLabel; }

    // Timer functionality
    void startCountdown() { timer.start(); }
    void resetTimer() { countdownTime = totalTime; }
    void pauseCountdown() { timer.stop(); }
  
signals:
    void countdownFinished();

private slots:
    void updateCountdown(); 

private:
    QTime countdownTime;   
    QTimer timer;          
    QLabel* timerLabel;
    const QTime totalTime;          // This time is used to reset the timer don't change it
};

#endif 
