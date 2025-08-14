
#include "singlevakdialog.h"

#include <QLabel>
#include <QPushButton>
#include "adddocenttovak.h"
#include "addexamtovak.h"
#include "../Examination/examinationView.h"
#include "../../Controller/examinationController.h"

SingleVakDialog::SingleVakDialog(VakkenManager* vakkenmanager, Vak* vak) : m_vakkenManager(vakkenmanager), m_vmanagerController(vakkenmanager), m_vak(vak)
{
    setWindowFlags(Qt::Popup);
    m_mainLayout = new QVBoxLayout(this);

    BuildLayout();
}


void SingleVakDialog::ClearLayout(QLayout* layout) {
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
}

void SingleVakDialog::BuildLayout()
{
    m_mainLayout->addWidget(new QLabel(m_vak->getNaam()));

    QSettings settings("groep_7", "leerhulpmiddel");
    bool isDocent = settings.value("isDocent", false).toBool();
    int userID = settings.value("userId", -1).toInt();

    QHBoxLayout* topHbox = new QHBoxLayout();
    topHbox->addWidget(new QLabel("Onderwijsteam:"));
    if (isDocent)
    {
        QPushButton* newDocent = new QPushButton("toevoegen");
        connect(newDocent, &QPushButton::pressed, this, [=]{
            AddDocentToVak dialog(m_vakkenManager, m_vak);
            dialog.move(this->pos().x() + this->size().width() + 10, this->pos().y());
            dialog.exec();
        });
        topHbox->addWidget(newDocent);
    }
    m_mainLayout->addLayout(topHbox);
    for (int i = 0; i < m_vak->getOnderwijsteam()->length(); i++)
    {
        Docent tempDocent = m_vak->getOnderwijsteam()->at(i);
        QString label = tempDocent.getNaam() + " | " + tempDocent.getRol();
        QHBoxLayout* hBox = new QHBoxLayout();
        hBox->addWidget(new QLabel(label));
        if (isDocent && userID != tempDocent.getID())
        {
            int tempId = tempDocent.getID();
            QPushButton* removeDocent = new QPushButton("X");
            connect(removeDocent, &QPushButton::pressed, this, [=]{
                //qDebug() << tempId;
                m_vmanagerController.removeDocent(tempId, m_vak->getID());
            });
            hBox->addWidget(removeDocent);
        }
        m_mainLayout->addLayout(hBox);
    }

    QHBoxLayout* middleHbox = new QHBoxLayout();
    middleHbox->addWidget(new QLabel("Proefexamens:"));
    if (isDocent)
    {
        QPushButton* newExam = new QPushButton("toevoegen");
        connect(newExam, &QPushButton::pressed, this, [=]{
            AddExamToVak dialog(m_vakkenManager, m_vak);
            dialog.move(this->pos().x() + this->size().width() + 10, this->pos().y());
            dialog.exec();
        });
        middleHbox->addWidget(newExam);
    }
    m_mainLayout->addLayout(middleHbox);

    for (int i = 0; i < m_vak->getExamens().length(); i++)
    {
        OnlineExamen tempEx = m_vak->getExamens().at(i);
        QHBoxLayout* hBox = new QHBoxLayout();
        QPushButton* examBtn = new QPushButton(tempEx.getNaam());
        hBox->addWidget(examBtn);

        if (isDocent)
        {
            connect(examBtn, &QPushButton::pressed, this, [=]() mutable{
                showScoreDialog(tempEx);
            });
        }
        else
        {
            connect(examBtn, &QPushButton::pressed, this, [=]() mutable{
                QDialog dialog;
                dialog.setWindowFlags(Qt::Popup);
                QVBoxLayout* lay = new QVBoxLayout();
                ExaminationView* examView = new ExaminationView(nullptr, false);
                examView->startExamination(tempEx.getQuestions(), tempEx.getTijd());
                lay->addWidget(examView);
                dialog.setLayout(lay);

                ExaminationController* eCont = examView->getController();

                connect(eCont, &ExaminationController::ended, &dialog, &QDialog::close);

                connect(examView, &ExaminationView::sendData, m_vakkenManager, [=](QMap<QString, QString> examinationData) mutable{
                    m_vakkenManager->receiveScore(examinationData, tempEx.getId());
                });

                dialog.exec();
            });
        }
        m_mainLayout->addLayout(hBox);
    }
}

void SingleVakDialog::showScoreDialog(OnlineExamen exam)
{
    QDialog dialog;
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel("gemiddelde:"));
    hbox->addWidget(new QLabel(QString::number(exam.getAverage()) + "%"));

    layout->addLayout(hbox);

    for (int i = 0; i < exam.getScores().length(); i++)
    {
        Score s = exam.getScores().at(i);
        QHBoxLayout* temp = new QHBoxLayout();
        temp->addWidget(new QLabel(s.getNaam()));
        temp->addWidget(new QLabel(QString::number(s.getPercent()) + "%"));

        layout->addLayout(temp);
    }

    dialog.setWindowFlags(Qt::Popup);
    dialog.setLayout(layout);
    dialog.move(this->pos().x() + this->size().width() + 10, this->pos().y());

    dialog.exec();
}


void SingleVakDialog::rebuild()
{
    ClearLayout(m_mainLayout);
    BuildLayout();
}
