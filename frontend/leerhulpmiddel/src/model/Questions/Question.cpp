#include "Question.h"

//Convert a questionType to a string that represents that type
QString Question::questionTypeToString() const {
    switch (m_questionType) {
        case QuestionType::MultipleChoice: 
            return "MultipleChoice";
        case QuestionType::FillIn: 
            return "FillIn";
        case QuestionType::Flashcard:
            return "Flashcard";
        default: 
            return "None";
    }
}

/*
* Convert a string that represents a questionType to a questionType
* @pre this assumes the string follows the same conversion as questionTypeToString
* @return QuestionType this is the question type converted from the string
*/
QuestionType Question::stringToQuestionType(const QString questionTypeStr) {
    if (questionTypeStr == "FillIn") 
        return QuestionType::FillIn;
    else if (questionTypeStr == "MultipleChoice") 
        return QuestionType::MultipleChoice;
    else if (questionTypeStr == "Flashcard") 
        return QuestionType::Flashcard;
    else if (questionTypeStr == "None")
        return QuestionType::None;
}
