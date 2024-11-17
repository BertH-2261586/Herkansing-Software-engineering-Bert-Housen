#ifndef LOADEXCEPTION_H
#define LOADEXCEPTION_H

#include <stdexcept>
#include <QString>

class loadException : public std::runtime_error {
public:
    explicit loadException(const QString& message) : std::runtime_error(message.toStdString()) {}
};

#endif