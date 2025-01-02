#include "chooseQuestionView.h"

#include <QFileIconProvider>

// Initialize the GUI of the view
// @param questionSetSelectOnly this bool indicates that the user can select only the top level or individual questions
ChooseQuestionView::ChooseQuestionView(bool questionSetSelectOnly, bool selectMultiple,QWidget* parent) : QWidget(parent) {
    FileManager fileManager = FileManager();
    m_questionSets = fileManager.loadQuestionSetsObject();

    setupTreeWidget(questionSetSelectOnly, selectMultiple);

    // Create the icons for the folder and file structure
    QFileIconProvider iconProvider;
    m_folderIcon = iconProvider.icon(QFileIconProvider::Folder);
    m_fileIcon = iconProvider.icon(QFileIconProvider::File);

    // Populate the tree with the question sets and questions
    for (Questionset* questionSet : m_questionSets) {
        // Check if the question set has any questions, if not you dont need to display it
        if (questionSet->hasQuestion()) {
            populateTreeWithQuestionset(nullptr, questionSet, questionSetSelectOnly);
        }
    }

    // Set up the main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_treeWidget);

    setLayout(m_mainLayout);
}

// Initialize the QTreeWidget
void ChooseQuestionView::setupTreeWidget(bool questionSetSelectOnly,bool selectMultiple) {
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel(questionSetSelectOnly ? "Choose a question set" : "Choose the questions you want");
    m_treeWidget->setSelectionMode(selectMultiple ? QAbstractItemView::MultiSelection : QAbstractItemView::SingleSelection);

    // Set the color palette of the choose question view
    QPalette currentPalette = m_treeWidget->palette();
    QColor baseColor = currentPalette.color(QPalette::Window);
    QColor darkerColor = baseColor.darker(115);
    m_treeWidget->setStyleSheet(
        QString(
            "background-color: %1;"
            "QTreeWidget::item {"
            "    margin-bottom: 5px;"
            "    font-size: 14px;"
            "}"
        ).arg(darkerColor.name())
    );

    // Center the header text
    QTreeWidgetItem* headerItem = m_treeWidget->headerItem();
    if (headerItem) {
        headerItem->setTextAlignment(0, Qt::AlignCenter);
    }

    // Check if you only allow top level checkboxes (for examples with examinations) and connect the checkboxes to the correct handler
    if (questionSetSelectOnly && !selectMultiple) {
        connect(m_treeWidget, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int column) {
            handleQuestionSetOnlyItemCheckChange(item, column);
        });
    }
    // Individual questions selection is allowed (for examples with practice exam) and connect the checkboxes to the correct handler
    else {
        connect(m_treeWidget, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int column) {
            handleItemCheckChange(item, column);
        });
    }
}

/*
* This populates the tree with question set subfolders and the associated questions inside of them
* @param parentItem this is the parent of the next folder/file you want to create. If its a nullptr you're at the top of the tree
* @param questionSet this is the current questionSet you're working with, it contains all the files and folders that you need
* @param questionSetSelectOnly this bool indicates that the user can select only the top level or individual questions 
    (For example examination is only allowed to select the main question set)
* @post the tree with all the subfolders and questions get created and put in the proper placement in the tree
*/
void ChooseQuestionView::populateTreeWithQuestionset(QTreeWidgetItem* parentItem, Questionset* questionSet, bool questionSetSelectOnly) {
    QTreeWidgetItem* questionSetItem = nullptr;

    // If parentItem is not null, add the item you are going to create as its child
    if (parentItem) {
        questionSetItem = new QTreeWidgetItem(parentItem);
        if (!questionSetSelectOnly) {
            questionSetItem->setFlags(questionSetItem->flags() | Qt::ItemIsSelectable);
            questionSetItem->setCheckState(0, Qt::Unchecked);
        }
    }
    // If parentItem is null, add the item you are going to create as a top-level item
    else {
        questionSetItem = new QTreeWidgetItem(m_treeWidget);
        questionSetItem->setFlags(questionSetItem->flags() | Qt::ItemIsSelectable);
        questionSetItem->setCheckState(0, Qt::Unchecked);      
    }

    questionSetItem->setText(0, questionSet->GetName());
    questionSetItem->setIcon(0, m_folderIcon);
    // Disable the cliking of the folder rectangle (not the checkbox itself)
    questionSetItem->setFlags(questionSetItem->flags() & ~Qt::ItemIsSelectable);

    // Add questions to the tree
    for (Question* question : questionSet->GetLooseQuestions()) {
        QTreeWidgetItem* questionItem = new QTreeWidgetItem(questionSetItem);
        questionItem->setText(0, question->getName());  
        questionItem->setIcon(0, m_fileIcon);

        // Disable the cliking of the file rectangle (not the checkbox itself)
        questionItem->setFlags(questionItem->flags() & ~Qt::ItemIsSelectable);

        // Give questions checkboxes only if individual question selection is allowed
        if (!questionSetSelectOnly) {
            questionItem->setFlags(questionSetItem->flags() | Qt::ItemIsSelectable);
            questionItem->setCheckState(0, Qt::Unchecked);
        }
    }

    // Recursively add subsets
    for (Questionset* subset : questionSet->GetSubSets()) {
        populateTreeWithQuestionset(questionSetItem, subset, questionSetSelectOnly);
    }
}

// Handle the press of the checkbox for when you can only select one question set (for example examination). 
// In this case deselect all the other checkboxes
void ChooseQuestionView::handleQuestionSetOnlyItemCheckChange(QTreeWidgetItem* item, int column) {
    // Check if the current question set is checked
    if (item->checkState(0) == Qt::Checked) {
        // Uncheck all other top-level items
        for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
            QTreeWidgetItem* topLevelItem = m_treeWidget->topLevelItem(i);
            if (topLevelItem != item) {
                topLevelItem->setCheckState(0, Qt::Unchecked);
            }
        }
    }
}

// Get the question set that has been selected
QString ChooseQuestionView::getQuestionSetPath() {
    // Iterate over all top level question sets
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* item = m_treeWidget->topLevelItem(i);

        // Check if the item is selected
        if (item->checkState(0) == Qt::Checked) {
            // Item is selected, do something with it
            return item->text(0);
        }
    }

    // Nothing was selected
    return "";
}

QList<QString> ChooseQuestionView::getQuestionSetPaths()
{
    QList<QString> paths;
    // Iterate over all top level question sets
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
		QTreeWidgetItem* item = m_treeWidget->topLevelItem(i);

		if (item->checkState(0) == Qt::Checked) {
			paths.append(item->text(0));
		}
	}
	return paths;
}

/*
* Handle the press of the checkbox for when you can select whatever you want (for example practice exam). 
* In this case make it so that all the other checkboxes on a lower level also get selected
* TODO
*/
void ChooseQuestionView::handleItemCheckChange(QTreeWidgetItem* item, int column) {
    qDebug() << "went in";
}

