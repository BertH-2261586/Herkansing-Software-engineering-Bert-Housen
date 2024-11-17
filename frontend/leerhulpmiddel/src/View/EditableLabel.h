
#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QEvent>
#include <QVBoxLayout>
#include "focusoutlineedit.h"

class EditableLabel : public QWidget
{

    Q_OBJECT

public:
    EditableLabel(QString text, QWidget* parent = nullptr) : m_label(new QLabel(text, this)), m_lineEdit(new FocusOutLineEdit(this))
    {
        m_lineEdit->hide();

        QVBoxLayout* container = new QVBoxLayout(this);
        container->setContentsMargins(0, 0, 0, 0);

        container->addWidget(m_label);
        container->addWidget(m_lineEdit);

        m_label->installEventFilter(this);
        connect(m_lineEdit, &FocusOutLineEdit::lostFocus, this, &EditableLabel::StopEditing);

        connect(m_lineEdit, &FocusOutLineEdit::escPressed, this, [this]{        //Als er op esc geduwt wordt moet er geen signaal gestuurd worden want er zijn geen veranderingen gebuert
            m_label->show();
            m_lineEdit->hide();
        });


        QPalette labelPalette = m_label->palette();
        labelPalette.setColor(QPalette::WindowText, Qt::black);

        m_label->setPalette(labelPalette);

        QPalette lineEditPalette = m_lineEdit->palette();
        lineEditPalette.setColor(QPalette::Text, Qt::black);

        m_lineEdit->setPalette(lineEditPalette);
    }

    void setText(QString newText)
    {
        m_label->setText(newText);
    }


protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == m_label && event->type() == QEvent::MouseButtonDblClick)
        {
            StartEditing();
            return true;
        }
        return QWidget::eventFilter(watched, event);
    }

private:
    QLabel* m_label;
    FocusOutLineEdit* m_lineEdit;

    void StartEditing()
    {
        m_label->hide();
        m_lineEdit->show();

        m_lineEdit->setText(m_label->text());
        m_lineEdit->setFocus();
        m_lineEdit->selectAll();
    }

    void StopEditing()
    {
        emit textChanged(m_lineEdit->text());

        m_label->show();
        m_lineEdit->hide();
    }

signals:
    void textChanged(QString newText);

};


#endif // EDITABLELABEL_H
