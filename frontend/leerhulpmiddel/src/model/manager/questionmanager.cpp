#include "questionmanager.h"

#include "../Questions/Flashcard.h"
#include "../Questions/Question.h"
#include "../fileManager.h"


QuestionManager::QuestionManager()
{
    FileManager fManager = FileManager();
    m_questionsets = fManager.loadQuestionSetsObject();
}


//free up heap memeory
QuestionManager::~QuestionManager()
{
    for (int i = 0; i < m_questionsets.length(); i++)
    {
        delete m_questionsets[i];
    }
}

void QuestionManager::changeQuestionsetName(QString newName, Questionset* questionset)
{
    for (int i = 0; i < m_questionsets.length(); i++)
    {
        if (m_questionsets[i]->GetName() == newName)
        {
            //TODO fout emitten

            return;
        }
    }

    questionset->SetName(newName);
    emit changed();
}

void QuestionManager::addQuestionset()
{
    QString newName = "Nieuwe vragenset";
    int number = 1;

    while (true)
    {
        QString tempName;
        if (number == 1)
        {
            tempName = newName;
        }
        else
        {
            tempName = newName + QString::number(number);
        }

        bool found = true;
        for (int i = 0; i < m_questionsets.length(); i++)
        {
            if (tempName == m_questionsets[i]->GetName())
            {
                found = false;
                number += 1;
                break;
            }
        }

        if (found)
        {
            newName = tempName;
            break;
        }
    }

    FileManager fManager = FileManager();
    fManager.makeQuestionSet("", newName);

    m_questionsets.append(new Questionset(newName, {}, {}));


    emit changed();
}

//void QuestionManager::addQuestion(QString questionSetName, QString subsection, Question* question) {
//    FileManager fManager = FileManager();

//    fManager.saveQuestionToJSON(questionSetName, subsection, *question);

//    Questionset* questionSet = getQuestionSet(questionSetName);
//    questionSet->addQuestion(question, subsection);
//}

//Questionset* QuestionManager::getQuestionSet(QString questionSetName)
//{
//    for (Questionset* questionset : m_questionsets) {
//        if (questionset->GetName() == questionSetName) {
//            return questionset;
//        }
//    }

//    qDebug() << "Questionset " << questionSetName << " not found";
//}

