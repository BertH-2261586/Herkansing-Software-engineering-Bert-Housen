
#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H


#include <QObject>
#include <QLineEdit>
#include <QKeyEvent>
#include <QMouseEvent>

class FocusOutLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit FocusOutLineEdit(QWidget* parent = nullptr) : QLineEdit(parent) {}

protected:
    void focusOutEvent(QFocusEvent* event) override
    {
        QLineEdit::focusOutEvent(event);
        emit lostFocus();

    }

    void keyPressEvent(QKeyEvent* event) override
    {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        {
            emit lostFocus();
        }
        else if (event->key() == Qt::Key_Escape)
        {
            emit escPressed();
        }
        else
        {
            QLineEdit::keyPressEvent(event);
        }
    }

signals:
    void lostFocus();
    void escPressed();
};

#endif // MYLINEEDIT_H
