
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

class HomeScreen;


class QuestionsetWidget : public QPushButton
{
    Q_OBJECT


public:
    QuestionsetWidget(Questionset* questionset, HomeScreen* homeScreenParent = nullptr, QWidget* parent = nullptr);

    ~QuestionsetWidget();

    QuestionsetTreeWidget* getUnderlyingTree(){return m_underlyingTree;}

    void setPermaDisplay(bool value){m_permaDisplay = value;}
    bool getPermaDisplay(){return m_permaDisplay;}

protected:
    void enterEvent(QEnterEvent *event) override
    {
        emit hoverEnter();
        QPushButton::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override
    {

        emit hoverLeave();
        QPushButton::leaveEvent(event);
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

};

#endif // QUESTIONSETWIDGET_H
