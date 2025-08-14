
#ifndef VAKKENMANAGER_H
#define VAKKENMANAGER_H


#include <QObject>

#include <QList>
#include <QString>
#include "../vak.h"
#include "NetworkManager.h"

class VakkenManager : public QObject
{
    Q_OBJECT
public:
    VakkenManager(NetworkManager* networkmanager);
    ~VakkenManager();

    void addVak(QString naam);
    void addDocentToVak(int docentID, int vakID, QString titel);
    void AddExamToVak(int vakID, QString fileName);
    void removeDocent(int docentID, int vakID);

    QList<Vak*>* getVakken(){return &m_vakken;};
    QList<Docent>* getDocenten(){return &m_allDocenten;};

    void refreshVakken();

private:
    QList<Vak*> m_vakken;
    QList<Docent> m_allDocenten;
    NetworkManager* m_networkmanager;


signals:
    void changed();

public slots:
    void newServerData(QJsonArray jsonData);
    void newDocentData(QJsonArray jsonData);
    void receiveScore(QMap<QString, QString> data, int examId);

};

#endif // VAKKENMANAGER_H
