
#include "vakkenmanager.h"
#include <QException>
#include <QJsonArray>

VakkenManager::VakkenManager(NetworkManager* networkmanager) : m_networkmanager(networkmanager)
{
    connect(m_networkmanager, &NetworkManager::vakkenChanged, this, &VakkenManager::newServerData);
    connect(m_networkmanager, &NetworkManager::docentenChanged, this, &VakkenManager::newDocentData);

}

VakkenManager::~VakkenManager()
{
    for (int i = 0; i < m_vakken.length(); i++)
    {
        delete m_vakken.at(i);
    }
}

void VakkenManager::receiveScore(QMap<QString, QString> data, int examId)
{
    m_networkmanager->sendScore(
        (int)(((data.value("right_answers").toInt() - data.value("total_wrong_answers").toInt()) / data.value("total_answers").toInt())*100),
        examId
        );
}


void VakkenManager::addVak(QString naam)
{
    m_networkmanager->addVak(naam);

//    m_vakken.append(new Vak(
//        naam,
//        {Docent("Nieuwe", "beheerder", 4)},
//        "3"
//        ));
//    emit changed();
}

void VakkenManager::addDocentToVak(int docentID, int vakID, QString titel)
{
    m_networkmanager->addDocentToVak(docentID, vakID, titel);
}

void VakkenManager::AddExamToVak(int vakID, QString fileName)
{
    m_networkmanager->addExamToVak(fileName, vakID);
}

/**
 * Gaat van de server de vakken ophalen om zo de lokale lijst te refreshen
 *
 */
void VakkenManager::refreshVakken()
{
    m_networkmanager->getAllVakData();
}

void VakkenManager::removeDocent(int docentID, int vakID)
{
    m_networkmanager->removeDocent(docentID, vakID);
}

void VakkenManager::newDocentData(QJsonArray jsonData)
{
    try
    {
        QList<Docent> newDocenten;
        for (int i = 0; i < jsonData.size(); i++)
        {
            newDocenten.append(Docent(jsonData.at(i).toObject()));
        }

        m_allDocenten = newDocenten;

    } catch (QException e)
    {
        qDebug() << e.what();
    }
}

//gaat de gegeven json data processen
void VakkenManager::newServerData(QJsonArray jsonData)
{
    try
    {
        QList<Vak*> newVakken;
        for (int i = 0; i < jsonData.size(); i++)
        {
            newVakken.append(new Vak(jsonData.at(i).toObject()));
        }

        for (int i = 0; i < m_vakken.length(); i++)
        {
            delete m_vakken.at(i);
        }

        m_vakken = newVakken;
        emit changed();

    } catch (QException e)
    {
        qDebug() << e.what();
    }
}
