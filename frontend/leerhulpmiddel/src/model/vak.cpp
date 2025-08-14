
#include "vak.h"
#include <QJsonArray>

Vak::Vak(QString VakNaam, QList<Docent> Onderwijsteam, int uniqueID) : m_naam(VakNaam), m_onderwijsteam(Onderwijsteam), m_uniqueID(uniqueID)
{

}

Vak::Vak(QJsonObject jsonData)
{
    m_naam = jsonData["vaknaam"].toString();
    m_uniqueID = jsonData["id"].toInt();
    for (int i = 0; i < jsonData["docenten"].toArray().size(); i++)
    {
        QJsonObject tempObj = jsonData["docenten"].toArray().at(i).toObject();
        m_onderwijsteam.append(Docent(tempObj));
    }

    for (int i = 0; i < jsonData["proefexamen"].toArray().size(); i++)
    {
        QJsonObject tempObj = jsonData["proefexamen"].toArray().at(i).toObject();
        m_examens.append(OnlineExamen(tempObj));
    }
}

QString Vak::getNaam()
{
    return m_naam;
}


bool Vak::isDeelVanOnderwijsteam(int DocentID)
{
    for (int i = 0; i < m_onderwijsteam.length(); i++)
    {
        Docent tempDocent = m_onderwijsteam.at(i);
        if (tempDocent.getID() == DocentID)
        {
            return true;
        }
    }
    return false;
}
