#ifndef INBOXVIEW_H
#define INBOXVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScreen>
#include <QPropertyAnimation>
#include <QList>
#include <QScrollArea>
#include <QPushButton>

class InboxView : public QWidget
{
    Q_OBJECT
public:
    InboxView(QWidget* parent = nullptr);

private:
    void acceptedQuestionSet(int index);
    void rejectedQuestionSet(int index);

    // Set GUI of the inboxView
    void setAcceptButton(int index);
    void setRejectButton(int index);
    void setMenuItemLayout(int index);
    void addFrame(int index);
    void setMainLayout();
    void setSlidingMenu();

    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_menuLayout;
    QVBoxLayout* m_slidingMenuLayout;

    QWidget* m_slidingMenu;
    QPropertyAnimation* m_animation;
    QScrollArea* m_scrollArea;
    QWidget* m_menuContentWidget;

    QList<QGridLayout*> m_menuItemLayouts;
    QList<QLabel*> m_menuItemInfo;
    QList<QPushButton*> m_acceptButtons;
    QList<QPushButton*> m_rejectButtons;
};


#endif 
