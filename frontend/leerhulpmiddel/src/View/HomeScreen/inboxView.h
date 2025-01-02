#ifndef INBOXVIEW_H
#define INBOXVIEW_H

#include "../../Controller/inboxController.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScreen>
#include <QPropertyAnimation>
#include <QList>
#include <QScrollArea>
#include <QPushButton>
#include <QGuiApplication>

class InboxView : public QWidget
{
    Q_OBJECT
public:
    InboxView(QWidget* parent = nullptr);
    int getAmountInboxRequests() { return m_inboxController.getAmountOfMessages(); }
    void resetInboxView();

signals:
    void removeInboxItem();
    void updateInboxItemLabel();
    void questionSetSucces();
    void questionSetFailed();

private:
    void inboxRequestResponse(const int index, const bool accepted);

    // Set GUI of the inboxView
    void setInboxRequests();
    void setAcceptButton(const int index);
    void setRejectButton(const int index);
    void setMenuItemLayout(const int index);
    void addFrame(const int index);
    void setMainLayout();
    void setSlidingMenu();

    void deleteInboxItem(const int index = -1);

    inboxController m_inboxController = inboxController();

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