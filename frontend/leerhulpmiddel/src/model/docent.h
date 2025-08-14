
#ifndef DOCENT_H
#define DOCENT_H

#include <QString>
#include <QJsonObject>
class Docent
{
public:
    Docent(QString docentnaam, QString rol, int ID);
    Docent(QJsonObject jsonData);

    QString getNaam(){return m_docentnaam;};
    QString getRol(){return m_rol;};
    int getID(){return m_uniqueID;};


private:
    QString m_docentnaam;
    QString m_rol;
    int m_uniqueID;

};

#endif // DOCENT_H
