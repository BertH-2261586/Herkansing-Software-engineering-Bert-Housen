#ifndef UNZIPEXCEPTION_H
#define UNZIPEXCEPTION_H

#include <stdexcept>
#include <QString>

class unzipException : public std::runtime_error {
public:
    explicit unzipException(const QString& message) : std::runtime_error(message.toStdString()) {}
};

#endif