
#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H


#include <QObject>
#include <QLineEdit>


class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget* parent = nullptr) : QLineEdit(parent) {}

protected:
    void focusOutEvent(QFocusEvent* event) override
    {
        QLineEdit::focusOutEvent(event);
        emit lostFocus();
    }

signals:
    void lostFocus();
};

#endif // MYLINEEDIT_H
