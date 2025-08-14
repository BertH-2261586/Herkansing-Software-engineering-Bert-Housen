
#include "adddocenttovak.h"
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

AddDocentToVak::AddDocentToVak(VakkenManager* vakkenManager, Vak* vak) :
    m_vakkenManager(vakkenManager), m_vak(vak), m_vmanagerController(vakkenManager)
{
    setWindowFlags(Qt::Popup);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLineEdit* inputField = new QLineEdit();
    inputField->setPlaceholderText("Titel/rol");

    mainLayout->addWidget(inputField);

    QSettings settings("groep_7", "leerhulpmiddel");
    //int userId = settings.value("userId").toInt();
    QList<int> IDList;

    QListWidget* listWidget = new QListWidget();
    for (int i = 0; i < m_vakkenManager->getDocenten()->length(); i++)
    {
        Docent tempDocent = m_vakkenManager->getDocenten()->at(i);
        if (!m_vak->isDeelVanOnderwijsteam(tempDocent.getID()))
        {
            listWidget->addItem(tempDocent.getNaam());
            IDList.append(tempDocent.getID());
        }
    }

    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    mainLayout->addWidget(listWidget);

    QPushButton* confirmButton = new QPushButton("Bevestig");
    connect(confirmButton, &QPushButton::pressed, this, [=]{
        if (!inputField->text().isEmpty() && !listWidget->selectedItems().isEmpty() && inputField->text().length() < 32)
        {
            QList<QModelIndex> indexes = listWidget->selectionModel()->selectedIndexes();       //om de indices te vinden

            QList<int> selectedRows;
            for (const QModelIndex &index : indexes) {
                selectedRows.append(index.row());
            }
            this->close();
            m_vmanagerController.AddDocentToVak(IDList[selectedRows[0]], m_vak->getID(), inputField->text());       //kan maar 1 ding geselcteerd zijn

        }
    });
    mainLayout->addWidget(confirmButton);
}

