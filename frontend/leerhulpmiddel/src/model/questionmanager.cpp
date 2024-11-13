#include "questionmanager.h"

#include "Questions/Flashcard.h"
#include "Questions/Question.h"
#include "fileManager.h"

QuestionManager::QuestionManager()
{
    //hier temp data inzetten

    m_questionsets.append(new Questionset(
            "naam1",
        {new Flashcard("ah", "", Answer({})), new Flashcard("bah", "", Answer({}))},
        {new Questionset("subset1", {new Flashcard("boop", "", Answer({})), new Flashcard("boop2", "", Answer({}))}, {})},
        {255, 0, 255}
        ));
    m_questionsets.append(new Questionset(
        "naam2aaaaaaaaaa",
        {new Flashcard("eh", "", Answer({})), new Flashcard("bo", "", Answer({}))},
        {new Questionset("subset1", {new Flashcard("boop", "", Answer({})), new Flashcard("boop2", "", Answer({}))}, {})},
        {255, 255, 0}
        ));
    m_questionsets.append(new Questionset(
        "naam3",
        {new Flashcard("bap", "", Answer({})), new Flashcard("bop", "", Answer({}))},
        {new Questionset("subset1", {new Flashcard("boop", "", Answer({})), new Flashcard("boop2", "", Answer({}))}, {})},
        {0, 255, 255}
        ));
    m_questionsets.append(new Questionset(
        "naam4",
        {new Flashcard("ah", "", Answer({})), new Flashcard("bah", "", Answer({}))},
        {new Questionset("subset1", {new Flashcard("boop", "", Answer({})), new Flashcard("boop2", "", Answer({}))}, {})},
        {0, 100, 255}
        ));
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

