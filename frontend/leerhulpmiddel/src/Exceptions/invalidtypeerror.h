
#ifndef INVALIDTYPEERROR_H
#define INVALIDTYPEERROR_H

#include <QException>


class InvalidTypeError: public QException
{
public:
    void raise() const override {
        throw *this;
    }

    InvalidTypeError* clone() const override {
        return new InvalidTypeError(*this);
    }
};

#endif // INVALIDTYPEERROR_H
