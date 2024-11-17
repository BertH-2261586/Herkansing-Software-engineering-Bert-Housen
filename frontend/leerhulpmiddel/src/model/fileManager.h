#ifndef LOADER_H
#define LOADER_H

#include <QString>
#include <stdexcept>
#include <QDir>

#include "Questions/Question.h"
#include "Questions/Answer.h"
#include "../Exceptions/FolderQuestionSetMovedException.h"
#include "../Exceptions/loadException.h"
#include "../Exceptions/saveException.h"

using namespace std;

// De klasse voor alle files en folders te managen omremt vragen en vragensets
class FileManager {
private:
    QMap<QString, QVariantList> loadFilesAndQuestions(const QDir& dir, int currentDepth = 0) const;

public:
    QString getPath() const;

    Answer convertToAnswerObject(QJsonObject answer) const;
    QMap<QString, QVariantList> loadQuestionSets(QString path = "") const;
    void makeQuestionSet(QString path, QString fileName) const;
    void saveQuestionToJSON(const QString questionPath, const Question& question) const;
    unique_ptr<Question> loadQuestionFromJSON(const QString questionPath, const QString questionName) const;
    QVector<shared_ptr<Question>> getAllQuestionsFromQuestionSet(const QString path) const;
};

#endif 