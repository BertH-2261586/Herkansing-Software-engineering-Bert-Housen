
#include "onlineexamen.h"
#include <QCoreApplication>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "fileManager.h"

OnlineExamen::OnlineExamen(QJsonObject jsonData)
{
    m_id = jsonData["id"].toInt();
    QString base64Str = jsonData["data"].toString();
    QByteArray jsonBytes = QByteArray::fromBase64(base64Str.toUtf8());
    QJsonDocument doc = QJsonDocument::fromJson(jsonBytes);

    m_naam = doc["naam"].toString();
    m_tijd = QTime::fromString(doc["timer"].toString());

    FileManager fManager;

    for (int i = 0; i < doc["questions"].toArray().size(); i++)
    {
        m_questions.append(
            fManager.loadQuestionFromJSONObject(
                doc["questions"].toArray().at(i).toObject()
                ));
        //qDebug() << m_questions.at(i)->getQuestion();
    }

    for (int i = 0; i < jsonData["score"].toArray().size(); i++)
    {
        m_scores.append(Score(jsonData["score"].toArray().at(i).toObject()));
    }

    //qDebug() << m_naam << " + " << m_tijd;
}

bool OnlineExamen::doesAlreadyExist(QString naam)
{
    QString projectDirPath = QCoreApplication::applicationDirPath();
    if (!QDir().exists(projectDirPath + "/examens"))
    {
        QString path = projectDirPath + "/examens";
        std::filesystem::create_directory(path.toStdString());
    }

    if (QDir().exists(projectDirPath + "/examens/" + naam + ".json"))
    {
        return true;
    }
    return false;

}

void OnlineExamen::saveExamen(QList<QString> questionPaths, QString naam, QTime timer)
{
    QString projectDirPath = QCoreApplication::applicationDirPath();
    if (!QDir().exists(projectDirPath + "/examens"))
    {
        QString path = projectDirPath + "/examens";
        std::filesystem::create_directory(path.toStdString());
    }

    QJsonArray fileDataArray;

    for (const QString &path : questionPaths) {
        QString temppath = projectDirPath + "/questionSets/" +  path + ".json";
        QFile file(temppath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open file:" << temppath;
            continue;
        }

        QByteArray fileContent = file.readAll();
        file.close();

        QJsonDocument innerDoc = QJsonDocument::fromJson(fileContent);

        // Store file content as base64 so binary files are safe in JSON
        fileDataArray.append(innerDoc.object());
    }


    QJsonObject obj;
    obj["questions"] = fileDataArray;
    obj["timer"] = timer.toString();
    obj["naam"] = naam;

    QJsonDocument doc(obj);

    QFile outFile(projectDirPath + "/examens/" + naam + ".json");
    if (!outFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to create output file:" << projectDirPath + "/examens/" + naam + ".json";
        return;
    }

    outFile.write(doc.toJson(QJsonDocument::Indented));
    outFile.close();
}

int OnlineExamen::getAverage()
{
    int out = 0;
    for (int i = 0; i < m_scores.length(); i++)
    {
        Score s = m_scores.at(i);
        out += s.getPercent();
    }

    if (m_scores.length() == 0)
    {
        return 0;
    }

    return (int)(out / m_scores.length());
}

