
#ifndef QUESTIONBUTTON_H
#define QUESTIONBUTTON_H


#include <QWidget>
#include <QPushButton>
#include "../../../model/Questions/Question.h"
#include "../../EditableLabel.h"


class QuestionButton : public QWidget
{
    Q_OBJECT
public:
    QuestionButton(Question* question, int indentation, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event)
    {
        QWidget::mousePressEvent(event);
        emit clicked();
    };

signals:
    void clicked();

private:
    Question* m_question;
    EditableLabel* m_label;

    const int m_baseOfset = 30;
    const int m_indentationOfset = 10;

    void update();
};

#endif // QUESTIONBUTTON_H
