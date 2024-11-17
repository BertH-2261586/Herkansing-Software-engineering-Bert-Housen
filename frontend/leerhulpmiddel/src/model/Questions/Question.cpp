#include "Question.h"

void Question::setName(QString newName)
{
    m_name = newName;
    emit changed();
}


QString Question::questionTypeToString() const {
    switch (m_questionType) {
        case QuestionType::MultipleChoice: 
            return "MultipleChoice";
        case QuestionType::FillIn: 
            return "FillIn";
        case QuestionType::Flashcard:
            return "TrueFalse";
        default: 
            return "None";
    }
}

QuestionType Question::stringToQuestionType(const QString questionTypeStr) {
    if (questionTypeStr == "FillIn") 
        return QuestionType::FillIn;
    if (questionTypeStr == "MultipleChoice") 
        return QuestionType::MultipleChoice;
    if (questionTypeStr == "FlashCard") 
        return QuestionType::Flashcard;
    if (questionTypeStr == "None")
        return QuestionType::None;
}
