
#include "questionset.h"

Questionset::Questionset(QString name, QList<Question*> looseQuestions, QList<Questionset*> subSets, QColor color) :
    m_name(name), m_looseQuestions(looseQuestions), m_subSets(subSets), m_color(color)
{

};

//questionset zijn verandtwoordelijkheid om de onderliggende questions op te ruimen
Questionset::~Questionset()
{
    for (int i = 0; i < m_looseQuestions.length(); i++)
    {
        delete m_looseQuestions[i];
    }
}

void Questionset::addQuestion(Question* question, QString subsetName) {
    if (subsetName == "") {
        m_looseQuestions.append(question);
    }
    else {
        for (Questionset* subset : m_subSets) {
            if (subset->GetName() == subsetName) {
                subset->addQuestion(question);
            }
        }

        qDebug() << "Subset " << subsetName << " not found";
    }
}

void Questionset::addComponent(Questionset* subset)
{

}

void Questionset::addComponent(Question* question)
{

}



