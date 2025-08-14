
#ifndef EXAMSELECTOR_H
#define EXAMSELECTOR_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QCoreApplication>

class ExamSelector : public QWidget {
    Q_OBJECT
public:
    explicit ExamSelector(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QString projectDirPath = QCoreApplication::applicationDirPath();
        QVBoxLayout *layout = new QVBoxLayout(this);
        m_listWidget = new QListWidget(this);

        // Single selection only
        m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        // Get all .json files from directory
        QDir dir(projectDirPath + "/examens/");
        QStringList filters;
        filters << "*.json";
        QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);

        // Populate list
        for (const QFileInfo &fileInfo : fileList) {
            m_listWidget->addItem(fileInfo.fileName());
        }

        layout->addWidget(m_listWidget);
        setLayout(layout);
    }

    QString selectedFile() const {
        QListWidgetItem *item = m_listWidget->currentItem();
        if (item)
            return item->text();
        return {};
    }

private:
    QListWidget *m_listWidget;
};




#endif // EXAMSELECTOR_H
