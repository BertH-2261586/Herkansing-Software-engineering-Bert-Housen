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
protected:
    void resizeEvent(QResizeEvent* event) override {
        // Ensure layout recalculates on resize
        QWidget::resizeEvent(event);

        // Force layout update by calling updateGeometry on the widget
        updateGeometry();  // This requests the layout to be recalculated and updated
    }
private:
    QVBoxLayout* m_mainLayout;
    QFlowLayout* m_questionLayout;
    QList<QVBoxLayout*> m_fillInLayouts;

    QList<QLabel*> m_textLabels;
    QList<QTextEdit*> m_answerInputs;
    QList<QLabel*> m_correctAnswer;
};

#endif