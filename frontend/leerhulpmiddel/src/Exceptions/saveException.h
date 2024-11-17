#ifndef SAVEEXCEPTION_H
#define SAVEEXCEPTION_H

#include <stdexcept>
#include <QString>

class saveException : public std::runtime_error {
public:
    explicit saveException(const QString& message) : std::runtime_error(message.toStdString()) {}
};

#endif