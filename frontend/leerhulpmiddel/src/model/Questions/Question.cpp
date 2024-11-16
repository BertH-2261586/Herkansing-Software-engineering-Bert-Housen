#include "Question.h"


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
