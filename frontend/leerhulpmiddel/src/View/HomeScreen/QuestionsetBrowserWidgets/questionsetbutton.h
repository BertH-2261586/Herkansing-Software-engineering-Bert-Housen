
#ifndef QUESTIONSETBUTTON_H
#define QUESTIONSETBUTTON_H

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyle>

#include "../../../model/questionset.h"
#include "../../EditableLabel.h"
#include "../../../Controller/questionsetcontroller.h"
#include "questionsettreewidget.h"


class QuestionsetButton : public QWidget
{
    Q_OBJECT


public:
    QuestionsetButton(Questionset* questionset, int indentation, QuestionsetTreeWidget *parent);

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        QWidget::mousePressEvent(event);
        emit clicked();
    };


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
    Questionset* m_questionset;
    EditableLabel* m_label;
    QuestionsetTreeWidget* m_parent;

    QuestionsetController m_controller;

    const int m_baseOfset = 30;
    const int m_indentationOfset = 10;

    void update();
    QPushButton* GenerateMenuButton();
};

#endif // QUESTIONSETBUTTON_H
