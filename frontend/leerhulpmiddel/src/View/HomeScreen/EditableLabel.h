
#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>


class EditableLabel : public QWidget
{
public:
    EditableLabel(QString text, QWidget* parent = nullptr);




private:
    QLabel* m_label;

};


#endif // EDITABLELABEL_H
