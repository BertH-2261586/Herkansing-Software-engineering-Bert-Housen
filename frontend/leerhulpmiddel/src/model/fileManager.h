#ifndef LOADER_H
#define LOADER_H

#include <QString>
#include <stdexcept>
#include <QDir>

#include "Questions/Question.h"
#include "Questions/Answer.h"
#include "questionset.h"

using namespace std;

// Alle exceptions die kunnen voorkomen in de klasse
class FolderQuestionSetMovedException : public std::runtime_error {
public:
    explicit FolderQuestionSetMovedException(const QString& message) : std::runtime_error(message.toStdString()) {}
};

class saveException : public std::runtime_error {
public:
    explicit saveException(const QString& message) : std::runtime_error(message.toStdString()) {}
};

class loadException : public std::runtime_error {
public:
    explicit loadException(const QString& message) : std::runtime_error(message.toStdString()) {}
};

// De klasse voor alle files en folders te managen omremt vragen en vragensets
class FileManager {
private:
    QString getPath() const;
    QMap<QString, QVariantList> loadFilesAndQuestions(const QDir& dir, int currentDepth = 0) const;
public:
    QList<Questionset*> loadQuestionSetsObject(const QString path = "") const;
    Answer convertToAnswerObject(QJsonObject answer) const;
    QMap<QString, QVariantList> loadQuestionSets(QString path = "") const;
    void makeQuestionSet(QString path, QString fileName) const;
    void saveQuestionToJSON(const QString questionSet, const QString subset, const Question& question) const;
    unique_ptr<Question> loadQuestionFromJSON(const string questionSet, const string subset, const QString questionName) const;
};


#endif // LOADER_H