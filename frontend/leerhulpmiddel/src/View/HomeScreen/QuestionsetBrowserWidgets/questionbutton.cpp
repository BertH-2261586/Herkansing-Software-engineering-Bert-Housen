
#include "questionbutton.h"
#include <QVBoxLayout>

QuestionButton::QuestionButton(Question* question, int indentation, QWidget* parent) : m_question(question), QWidget(parent)
{
    QVBoxLayout* container = new QVBoxLayout(this);
    container->setContentsMargins(0, 0, 0, 0);

    m_label = new EditableLabel(question->getName());

    connect(m_question, &Question::changed, this, &QuestionButton::update);

    connect(m_label, &EditableLabel::textChanged, question, &Question::setName);

    setStyleSheet(QString(                            //de styling van een subdeel, werk met padding zodat de button background er goed uitziet
            "{ "
            "   border: none;"
            "   border-radius: 0px;"
            "   padding: 5px 30px 5px %1px;"
            "   text-align: left;"
            "}"
            "QWidget:hover {"
            "   background-color: #4d4d4d;"
            "}"
            ).arg((m_indentationOfset * indentation) + m_baseOfset));

    container->addWidget(m_label);
}

void QuestionButton::update()
{
    m_label->setText(m_question->getName());
}

