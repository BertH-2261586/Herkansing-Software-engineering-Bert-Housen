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
    int getAmountInboxRequests() { return m_inboxRequests.size(); }

signals:
    void removeInboxItem();

private:
    void acceptedQuestionSet(int index);
    void rejectedQuestionSet(int index);

    // Set GUI of the inboxView
    void setInboxRequests();
    void setAcceptButton(int index);
    void setRejectButton(int index);
    void setMenuItemLayout(int index);
    void addFrame(int index);
    void setMainLayout();
    void setSlidingMenu();

    void deleteInboxItem(int index);

    QList<QString> m_inboxRequests = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"};

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
    QList<QFrame*> m_itemFrames;
    QLabel* m_noItemsInInbox;
};


#endif