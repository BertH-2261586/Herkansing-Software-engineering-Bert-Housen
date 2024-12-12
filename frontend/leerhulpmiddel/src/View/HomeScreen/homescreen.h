#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>

class LeerhulpmiddelMainWindow;

#include "questionsetbrowser.h"
#include "../../Controller/questionmanagercontroller.h"
#include "inboxView.h"

class HomeScreen: public QWidget
{
    Q_OBJECT
public:
    HomeScreen(QuestionManagerController* questionManagerController, LeerhulpmiddelMainWindow* parent);

public slots:
    void DisplayWidget(QWidget* displayWidget);
    void removeInboxItem() { setInboxRequestAmount(); }

private slots:
    void startInboxAnimation();

private:
    QWidget* GenerateTopButtonBar();

    void setInboxView();
    void setAddFriendButton(QHBoxLayout* container);
    void setInboxButton(QHBoxLayout* container);
    QWidget* setInboxRequestLabel(QPushButton* inboxButton);
    void setIconButton(QPushButton* button, QString iconName);

    void setInboxRequestAmount();

    QuestionsetBrowser* m_vragensetBrowser;
    QWidget* m_rightSideScreen;
    QHBoxLayout* m_container;

    LeerhulpmiddelMainWindow* m_mainWindow;

    InboxView* m_inboxView;
    QLabel* m_requestAmountLabel;
    QPropertyAnimation* m_inboxAnimation;
};

#endif // HOMESCREEN_H
