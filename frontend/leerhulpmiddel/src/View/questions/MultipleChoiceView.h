#ifndef MULTIPLECHOICEWINDOW_H
#define MULTIPLECHOICEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QFontMetrics>

#include "../../model/Questions/MultipleChoiceQuestion.h"

class MultipleChoiceView : public QWidget {
    Q_OBJECT

public:
    MultipleChoiceView(QWidget* parent = nullptr) {}
    void setQuestion(const MultipleChoiceQuestion* question);
    void clearPreviousQuestion();
    void showAnswer(int correctAnswer);
    QString getCheckedAnswers();

private:
    void setQuestionLabel(const MultipleChoiceQuestion* question);
    void setRadioButtons(const MultipleChoiceQuestion* question);

    // All GUI elements
    QLabel* m_questionLabel = nullptr;
    QVBoxLayout* m_mainQuestionLayout = nullptr;
    QGridLayout* m_radioButtonLayout = nullptr;
    QVector<QRadioButton*> m_radioButtonList = { nullptr };
    // Make a button group so that functionality can be shared (for example only be able to select up to 1 radio button)
    QButtonGroup* m_buttonGroup = nullptr;               
};

#endif