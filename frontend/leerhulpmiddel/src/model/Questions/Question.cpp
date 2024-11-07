#include "Question.h"

<<<<<<< HEAD
Question::Question(QString name) : m_name(name) {}
=======
string Question::questionTypeToString() const {
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
>>>>>>> main
