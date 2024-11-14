
#include "questionset.h"

Questionset::Questionset(QString name, QList<Question*> looseQuestions, QList<Questionset*> subSets, QColor color) :
    m_name(name), m_looseQuestions(looseQuestions), m_subSets(subSets), m_color(color)
{

};

//questionset zijn verandtwoordelijkheid om de onderliggende questions op te ruimen
//TODO subsets ook opruimen
Questionset::~Questionset()
{
    for (int i = 0; i < m_looseQuestions.length(); i++)
    {
        delete m_looseQuestions[i];
    }
}

//TODO mogelijks alles alfabetisch displayen en hier dan de goede index aan megeven
void Questionset::addSubSet(QString name)
{
    Questionset* newSubSet = new Questionset(name, {}, {});

    m_subSets.append(newSubSet);

    emit displayNewSubSet(newSubSet, 0);
}

void Questionset::addComponent(Question* question)
{

}



