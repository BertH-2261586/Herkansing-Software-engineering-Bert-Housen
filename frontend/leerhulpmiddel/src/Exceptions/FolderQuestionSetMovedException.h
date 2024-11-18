#ifndef FOLDERQUESTIONSETMOVEDEXCEPTION_H
#define FOLDERQUESTIONSETMOVEDEXCEPTION_H

#include <stdexcept>
#include <QString>

class FolderQuestionSetMovedException : public std::runtime_error {
public:
    explicit FolderQuestionSetMovedException(const QString& message) : std::runtime_error(message.toStdString()) {}
};

#endif