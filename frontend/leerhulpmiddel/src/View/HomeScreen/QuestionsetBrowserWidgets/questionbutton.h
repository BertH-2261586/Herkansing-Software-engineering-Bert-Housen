
#ifndef QUESTIONBUTTON_H
#define QUESTIONBUTTON_H


#include <QWidget>
#include <QPushButton>
#include "../../../model/Questions/Question.h"
#include "../../EditableLabel.h"
#include "../../../Controller/questioncontroller.h"
#include <QPainter>
#include <QStyleOption>


class QuestionButton : public QWidget
{
    Q_OBJECT
public:
    QuestionButton(Question* question, int indentation, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        QWidget::mousePressEvent(event);
        emit clicked();
    };

//    void paintEvent(QPaintEvent *) override
//    {
//        QStyleOption opt;
//        opt.initFrom(this);
//        QPainter p(this);
//        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    }


    //wordt gedaan met hover events omdat deze ook permanent geselcteerd kunnen worden en dan moet de stylesheet niet veranderd worden
    void enterEvent(QEnterEvent* event) override
    {
        setStyleSheet("background-color: #AFAFAF;");

        QWidget::enterEvent(event);
    }

    void leaveEvent(QEvent* event) override
    {
        setStyleSheet("background-color: transparent;");

        QWidget::leaveEvent(event);
    }

signals:
    void clicked();

private:
    Question* m_question;
    EditableLabel* m_label;

    QuestionController m_controller;

    const int m_baseOfset = 30;
    const int m_indentationOfset = 10;

    void update();
};

#endif // QUESTIONBUTTON_H
