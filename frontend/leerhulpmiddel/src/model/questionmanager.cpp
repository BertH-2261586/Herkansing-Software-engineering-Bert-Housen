#include "questionmanager.h"

#include "Questions/Flashcard.h"
#include "Questions/Question.h"
#include "fileManager.h"

QuestionManager::QuestionManager()
{
    //hier temp data inzetten

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

void QuestionManager::addQuestion(QString questionSetName, QString subsection, Question* question) {
    FileManager fManager = FileManager();

    fManager.saveQuestionToJSON(questionSetName, subsection, *question);

    Questionset* questionSet = getQuestionSet(questionSetName);
    questionSet->addQuestion(question, subsection);
}

Questionset* QuestionManager::getQuestionSet(QString questionSetName)
{
    for (Questionset* questionset : m_questionsets) {
        if (questionset->GetName() == questionSetName) {
            return questionset;
        }
    }

    qDebug() << "Questionset " << questionSetName << " not found";
}

