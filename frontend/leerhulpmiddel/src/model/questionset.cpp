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

    emit displayNewQuestion(question, 0);
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

// Check if the question set has a question
bool Questionset::hasQuestion() const {
    // Question set has a loose question
    if (m_looseQuestions.size() > 0) {
        return true;
    }

    // Check the subfolders for questions
    for (Questionset* subset : m_subSets) {
        if (subset->hasQuestion()) {
            return true; 
        }
    }

    // No questions found
    return false;
}

