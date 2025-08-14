
#include "allvakkendialog.h"
#include "qpushbutton.h"
#include <QLabel>

#include "../focusoutlineedit.h"
#include "singlevakdialog.h"

AllVakkenDialog::AllVakkenDialog(VakkenManager* vakkenManager) : m_vakkenManager(vakkenManager), m_vmanagerController(vakkenManager)
{
    setWindowFlags(Qt::Popup);

    m_mainLayout = new QVBoxLayout(this);

    QSettings settings("groep_7", "leerhulpmiddel");
    bool status = settings.value("loggedIn", false).toBool();    //TODO dit veranderen naar false
    if (status == false)
    {
        QLabel* message = new QLabel("Je moet ingelogd zijn hiervoor");
        m_mainLayout->addWidget(message);
    }
    else
    {
        m_mainLayout->addWidget(new QLabel("Loading..."));

        connect(m_vakkenManager, &VakkenManager::changed, this, &AllVakkenDialog::VakkenChanged);

        m_vakkenManager->refreshVakken();
    }
}

void AllVakkenDialog::VakkenChanged()
{
    QSettings settings("groep_7", "leerhulpmiddel");
    bool isDocent = settings.value("isDocent", false).toBool();
    if (isDocent)
    {
        BuildDocentLayout();
    }
    else
    {
        BuildStudentLayout();
    }
}

void AllVakkenDialog::ClearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = m_mainLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        if (item->layout()) {
            ClearLayout(item->layout());
        }
        delete item;
    }
    //layout->setParent(nullptr);
}

void AllVakkenDialog::BuildStudentLayout()
{
    ClearLayout(m_mainLayout);

    QLabel* label = new QLabel("Alle vakken");
    m_mainLayout->addWidget(label);

    QList<Vak*>* allVakken = m_vakkenManager->getVakken();
    for (int i = 0; i < allVakken->length(); i++)
    {
        Vak* tempVak = allVakken->at(i);
        QPushButton* vakButton = new QPushButton(tempVak->getNaam());
        connect(vakButton, &QPushButton::pressed, this, [=]{
            showSingleVakDialog(tempVak);
        });
        m_mainLayout->addWidget(vakButton);
    }
}

void AllVakkenDialog::BuildDocentLayout()
{
    ClearLayout(m_mainLayout);

    QHBoxLayout* hBoxLayout = new QHBoxLayout();

    QLabel* label = new QLabel("Mijn vakken");
    hBoxLayout->addWidget(label);

    QPushButton* addVakButton = new QPushButton("Voeg vak toe");
    connect(addVakButton, &QPushButton::pressed, this, [=]{
        ShowNewVakInput();
    });
    hBoxLayout->addWidget(addVakButton);
    m_mainLayout->addLayout(hBoxLayout);


    QSettings settings = QSettings("groep_7", "leerhulpmiddel");
    int userId = settings.value("userId").toInt();                 //op dit punt is de user al ingelogd

    QList<Vak*>* allVakken = m_vakkenManager->getVakken();
    for (int i = 0; i < allVakken->length(); i++)
    {
        Vak* tempVak = allVakken->at(i);
        if (tempVak->isDeelVanOnderwijsteam(userId))
        {
            QPushButton* vakButton = new QPushButton(tempVak->getNaam());
            connect(vakButton, &QPushButton::pressed, this, [=]{
                showSingleVakDialog(tempVak);
            });
            m_mainLayout->addWidget(vakButton);
        }
    }
}

void AllVakkenDialog::showSingleVakDialog(Vak* vak)
{
    SingleVakDialog dialog(m_vakkenManager, vak);
    dialog.move(this->pos().x() + this->size().width() + 10, this->pos().y());
    int vakId = vak->getID();

    disconnect(m_vakkenManager, nullptr, nullptr, nullptr);
    connect(m_vakkenManager, &VakkenManager::changed, this, &AllVakkenDialog::VakkenChanged);
    connect(m_vakkenManager, &VakkenManager::changed, &dialog, [=, &dialog]{

        Vak* changedVak;
        for (int i = 0; i < m_vakkenManager->getVakken()->length(); i++)
        {
            if (vakId == m_vakkenManager->getVakken()->at(i)->getID())
            {
                changedVak = m_vakkenManager->getVakken()->at(i);
            }
        }
        dialog.close();
        showSingleVakDialog(changedVak);
    });

    dialog.exec();
}


//Het input veld tonen als je op voeg vak toe klikt
void AllVakkenDialog::ShowNewVakInput()
{
    FocusOutLineEdit* inputField = new FocusOutLineEdit();
    m_mainLayout->insertWidget(1, inputField);

    connect(inputField, &FocusOutLineEdit::escPressed, this, [=]{
        m_mainLayout->removeWidget(inputField);
        delete inputField;
        });

    connect(inputField, &FocusOutLineEdit::lostFocus, this, [=]{
        if (!inputField->text().isEmpty() && inputField->text().length() < 255)
        {
            QString input = inputField->text();

            ClearLayout(m_mainLayout);
            m_mainLayout->addWidget(new QLabel("Loading..."));

            m_vmanagerController.AddVak(input);
        }
        else
        {
            m_mainLayout->removeWidget(inputField);
            delete inputField;
        }
    });
}
