#ifndef CHOOSEQUESTIONVIEW_H
#define CHOOSEQUESTIONVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "../model/fileManager.h"

class ChooseQuestionView : public QWidget {
    Q_OBJECT

public:
    ChooseQuestionView(bool questionSelectOnly,bool selectMultiple = false, QWidget* parent = nullptr);
    QString getQuestionSetPath();
    QList<QString> getQuestionSetPaths();   //used for multiple select
    QList<QString> getQuestionSets();

private:
    void setupTreeWidget(bool questionSetSelectOnly,bool selectMultiple);
    void populateTreeWithQuestionset(QTreeWidgetItem* parentItem, Questionset* questionSet, bool questionSetSelectOnly);
    void handleQuestionSetOnlyItemCheckChange(QTreeWidgetItem* item, int column);
    void handleItemCheckChange(QTreeWidgetItem* item, int column);
    void CheckChildren(QTreeWidgetItem* parent, Qt::CheckState state);
    void CheckParents(QTreeWidgetItem* child, Qt::CheckState state);
    QList<QString> getCheckedChildren(QTreeWidgetItem* parent);


    QVBoxLayout* m_mainLayout;
    QTreeWidget* m_treeWidget;

    QList<Questionset*> m_questionSets;

    // Icons
    QIcon m_folderIcon;
    QIcon m_fileIcon;
};

#endif
