
#ifndef QUESTIONSETWIDGET_H
#define QUESTIONSETWIDGET_H


#include <QWidget>
#include "../../../model/questionset.h"
#include "questionsettreewidget.h"
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QEvent>
#include <QMouseEvent>

class HomeScreen;


class QuestionsetWidget : public QWidget
{
    Q_OBJECT


public:
    QuestionsetWidget(Questionset* questionset, HomeScreen* homeScreenParent, QWidget* parent = nullptr);

    ~QuestionsetWidget();

    QuestionsetTreeWidget* getUnderlyingTree(){return m_underlyingTree;}

    void setPermaDisplay(bool value){m_permaDisplay = value;}
    bool getPermaDisplay(){return m_permaDisplay;}

protected:
    void enterEvent(QEnterEvent *event) override
    {
        emit hoverEnter();
        QWidget::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override
    {

        emit hoverLeave();
        QWidget::leaveEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            emit clicked();
        }
        //QWidget::mousePressEvent(event);
    }

private:
    Questionset* m_questionset;
    QLabel* m_label;

    QuestionsetTreeWidget* m_underlyingTree;
    bool m_permaDisplay = false;

    HomeScreen* m_homeScreenParent;

signals:
    void hoverEnter();
    void hoverLeave();

    void clicked();

};

#endif // QUESTIONSETWIDGET_H
