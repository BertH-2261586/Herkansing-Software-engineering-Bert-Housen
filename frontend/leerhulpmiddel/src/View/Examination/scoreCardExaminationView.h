#ifndef SCORECARDEXAMINARTIONVIEW_H
#define SCORECARDEXAMINARTIONVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class scoreCardExaminationView : public QWidget {
    Q_OBJECT

public:
    scoreCardExaminationView(int correctAnswers, int wrongAnswers, QWidget* parent = nullptr);
    ~scoreCardExaminationView() {}

private:
    void closeWindow() { this->close(); }

    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_scoreCardBoxLayout;
    QPushButton* m_closeButton;
    QLabel* m_correctAnswers;
    QLabel* m_wrongAnswers;
    //QLabel* m_wrongAnswers;
};

#endif