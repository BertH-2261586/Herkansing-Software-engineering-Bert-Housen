#ifndef FILLINWINDOW_H
#define FILLINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QGridLayout>

#include "../../model/Questions/FillInQuestion.h"
#include "../QFlowLayout.h" 

class FillInView : public QWidget {
    Q_OBJECT

public:
    explicit FillInView(QWidget* parent = nullptr) {}
    void setQuestion(const FillInQuestion* question);
    void showAnswer(QVector<int> wrongAnswers);
    QVector<QString> getAllAnswerText();  
    void clearPreviousQuestion();

private:
    QVBoxLayout* m_mainLayout = nullptr;
    QFlowLayout* m_questionLayout = nullptr;
    QList<QVBoxLayout*> m_fillInLayouts;

    QList<QLabel*> m_textLabels;
    QList<QTextEdit*> m_answerInputs;
    QList<QLabel*> m_correctAnswer;
};

#endif
