#ifndef FILLINEXAMINATIONWINDOW_H
#define FILLINEXAMINATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "../../model/Questions/FillInQuestion.h"

class FillInExaminationView : public QWidget {
    Q_OBJECT

public:
    explicit FillInExaminationView(QWidget* parent = nullptr) {}
    void setQuestion(FillInQuestion* question);

private:
    //bool eventFilter(QObject* watched, QEvent* event) override;

    QLabel* questionLabel;
    QLineEdit* answerInput;

    Question* m_currentQuestion;
};

#endif