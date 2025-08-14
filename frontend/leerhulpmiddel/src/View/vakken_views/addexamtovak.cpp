
#include "addexamtovak.h"
#include "../ExamSelector.h"
#include <QVBoxLayout>
#include <QPushButton>

AddExamToVak::AddExamToVak(VakkenManager* vakkenmanager, Vak* vak) :
    m_vak(vak), m_vakkenManager(vakkenmanager), m_vmanagerController(vakkenmanager)
{
    setWindowFlags(Qt::Popup);

    QVBoxLayout* vbox = new QVBoxLayout(this);
    ExamSelector* fileSelec = new ExamSelector();

    vbox->addWidget(fileSelec);

    QPushButton* saveButton = new QPushButton("save");
    vbox->addWidget(saveButton);

    connect(saveButton, &QPushButton::pressed, this, [=]{
        qDebug() << fileSelec->selectedFile();
        m_vmanagerController.AddExamToVak(vak->getID(), fileSelec->selectedFile());
        this->close();
    });
}

