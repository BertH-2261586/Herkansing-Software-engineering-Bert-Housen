
#include "docent.h"

Docent::Docent(QString docentnaam, QString rol, int ID) : m_docentnaam(docentnaam), m_rol(rol), m_uniqueID(ID)
{

}

Docent::Docent(QJsonObject jsonData)
{
    m_docentnaam = jsonData["username"].toString();
    m_rol = jsonData["titel"].toString();
    m_uniqueID = jsonData["id"].toInt();
}
