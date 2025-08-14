
#ifndef VAK_H
#define VAK_H

#include <QString>
#include <QList>
#include <QJsonObject>


#include "onlineexamen.h"
#include "docent.h"

class Vak
{
public:
    Vak(QString VakNaam, QList<Docent> Onderwijsteam, int uniqueID);
    Vak(QJsonObject jsonData);

    QString getNaam();
    int getID(){return m_uniqueID;};
    QList<Docent>* getOnderwijsteam(){return &m_onderwijsteam;};
    QList<OnlineExamen> getExamens(){return m_examens;};

    bool isDeelVanOnderwijsteam(int DocentID);


private:


    QList<Docent> m_onderwijsteam;
    QList<OnlineExamen> m_examens;
    QString m_naam;
    int m_uniqueID;

};

#endif // VAK_H
