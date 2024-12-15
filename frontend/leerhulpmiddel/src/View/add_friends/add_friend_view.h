#ifndef ADD_FRIEND_VIEW_H
#define ADD_FRIEND_VIEW_H

#include "../../Controller/addFriendController.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class AddFriendView : public QWidget
{
    Q_OBJECT
public:
    AddFriendView(QWidget* parent = nullptr);

private slots:
    void goBack() { this->close(); }

private:
    // Set GUI
    void setGoBack();
    void setSearchBar();
    void setUserSearch();
    void setMainLayout();

    void setAllUsers();
    void deleteUsersFromList();
   
    addFriendController m_addFriendController = addFriendController();

    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_userSearch;

    QWidget* m_userSearchWidget;
    QFrame* m_userSearchFrame;

    QPushButton* m_goBack;
    QLineEdit* m_searchBar;

    QList<QHBoxLayout*> m_userLayouts;
    QList<QLabel*> m_userNames;
    QList<QPushButton*> m_addUserButtons;
    QList<QFrame*> m_frames;
    QLabel* m_noUserFound;
};

#endif // QUESTIONSETBROWSER_H
