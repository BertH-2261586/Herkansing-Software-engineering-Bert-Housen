#include "questionset.h"
#include "fileManager.h"

Questionset::Questionset(QString name, QList<Question*> looseQuestions, QList<Questionset*> subSets, QColor color, Questionset* parent) :
    m_name(name), m_looseQuestions(looseQuestions), m_subSets(subSets), m_color(color), m_parent(parent)
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

void Questionset::addQuestion(Question* question) {

    //dit is retarded VVV
//    if (subsetName == "") {
//        m_looseQuestions.append(question);
//    }
//    else {
//        for (Questionset* subset : m_subSets) {
//            if (subset->GetName() == subsetName) {
//                subset->addQuestion(question);
//            }
//        }

//        qDebug() << "Subset " << subsetName << " not found";
//    }

    //TODO nog controle dat er geen dubbele worden toegevoegd

    FileManager fManager = FileManager();
    if (m_parent == nullptr) {
        fManager.saveQuestionToJSON(m_name, "", *question);
    }
    else {
        fManager.saveQuestionToJSON(m_parent->GetName(), m_name, *question);
    }

    m_looseQuestions.append(question);

    emit displayNewQuestion(question, 0);
}

//TODO mogelijks alles alfabetisch displayen en hier dan de goede index aan megeven
void Questionset::addSubSet(QString name)
{
    Questionset* newSubSet = new Questionset(name, {}, {});

    m_subSets.append(newSubSet);

    emit displayNewSubSet(newSubSet, 0);
}


void Questionset::SetName(QString newName)
{
    m_name = newName;
    emit changed();
}


