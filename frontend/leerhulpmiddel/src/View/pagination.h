#ifndef PAGINATION_H
#define PAGINATION_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QList>
#include <QDebug>

class Pagination : public QWidget {
    Q_OBJECT

public:
    Pagination(int totalPageAmount, int currentPage, QWidget* parent = nullptr);

signals:
    void newPage(const int newPage);

private:
    void createNavigationButtons(bool backwards);
    void createPageButtons();
    QPushButton* createButton(const QString& text, bool selectedPage = false);

    void updateNavigationButtonState();

    int m_currentPage;
    int m_totalPageAmount;
    QHBoxLayout* m_paginationLayout;
    QList<QPushButton*> m_paginationButtons;
};

#endif