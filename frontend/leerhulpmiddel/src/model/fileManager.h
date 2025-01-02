#ifndef LOADER_H
#define LOADER_H

#include <QString>
#include <stdexcept>
#include <QDir>

#include "Questions/Question.h"
#include "Questions/Answer.h"
#include "questionset.h"
#include "../Exceptions/FolderQuestionSetMovedException.h"
#include "../Exceptions/loadException.h"
#include "../Exceptions/saveException.h"

using namespace std;

// The FileManager class is responsible for loading and saving question sets and questions to and from JSON files.
class FileManager {
public:
    QList<Questionset*> loadQuestionSetsObject(const QString path = "") const;
    Answer convertToAnswerObject(QJsonObject answer) const;
    QMap<QString, QVariantList> loadQuestionSets(QString path = "") const;
    void makeQuestionSet(QString path, QString fileName) const;
    void saveQuestionToJSON(const QString questionSet, const QString subset, const Question& question) const;
    unique_ptr<Question> loadQuestionFromJSON(const QString questionSet, const QString subset, const QString questionName) const;
    QVector<shared_ptr<Question>> getAllQuestionsFromQuestionSet(const QString questionSetPath) const;
    QVector<shared_ptr<Question>> getAllQuestionsFromQuestionSet(const QList<QString> questionSetPath) const;
    QByteArray createZip(const QStringList& questionSetPaths);
    void unzip(const QByteArray zipData);

    // = PRIVATE, is for unit testing 
    QString getPath() const;

private:
    QMap<QString, QVariantList> loadFilesAndQuestions(const QDir& dir, int currentDepth = 0) const;
    QString getUniqueName(QString baseName, QStringList existingNames);
    QString createTemporaryFolder();
    void renameDuplicates(const QString tempFolderPath, const QString destinationDir);
};

#endif 