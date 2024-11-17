#ifndef FILLINEXAMINATIONWINDOW_H
#define FILLINEXAMINATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

#include "../../model/Questions/FillInQuestion.h"

class FillInExaminationView : public QWidget {
    Q_OBJECT

public:
    explicit FillInExaminationView(QWidget* parent = nullptr) {}
    void setQuestion(const FillInQuestion* question);
    void showAnswer(const FillInQuestion* question);

private:
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_questionLayout;
    QList<QVBoxLayout*> m_fillInLayouts;
    QList<QLabel*> m_textLabels;
    QList<QTextEdit*> m_answerInputs;
    QList<QLabel*> m_correctAnswer;
};

#endif