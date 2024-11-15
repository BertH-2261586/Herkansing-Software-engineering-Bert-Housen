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
    QTime totalTime;            // Dit de time die bijhoud hoeveel tijd ingesteld is, dit wordt dan gebruikt voor de timer te resetten. Tel dit niet af!
};

#endif 
