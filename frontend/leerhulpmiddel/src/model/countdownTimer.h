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
    explicit CountdownTimer(QWidget* parent);
    ~CountdownTimer() { delete m_timerLabel; }

    // Timer functionality
    void startCountdown() { m_timer.start(); }
    void resetTimer() { m_countdownTime = m_totalTime; }
    void pauseCountdown() { m_timer.stop(); }
    void setupTimer(QTime time);

signals:
    void countdownFinished();
    
private slots:
    void updateCountdown(); 

private:
    QVBoxLayout* m_mainLayout;

    QTime m_countdownTime;   
    QTimer m_timer;          
    QLabel* m_timerLabel;
    QTime m_totalTime;          // This time is used to reset the timer don't change it
};

#endif 
