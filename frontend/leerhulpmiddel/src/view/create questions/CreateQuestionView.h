#pragma once

#include <QWidget>
#include <QComboBox>
#include <QStackedWidget>
#include <QLineEdit>

#include "CreateFlashcardView.h"
#include "CreateMultipleChoiceQuestionView.h"
#include "CreateFillInQuestionView.h"
#include "../../Controller/questionsetcontroller.h"

class CreateQuestionView : public  QWidget {
	Q_OBJECT
public:
    CreateQuestionView(QuestionsetController questionsetController, QWidget* parent = nullptr);
private:
	QLineEdit* m_txtQuestionName;
	QComboBox* m_cboQuestionType;
	QStackedWidget* m_questionSelection;

	CreateFlashcardView* m_createFlashcardView;
	CreateMultipleChoiceQuestionView* m_createMultipleChoiceQuestionView;
	CreateFillInQuestionView* m_createFillInQuestionView;

    QuestionsetController m_questionsetController;
private slots:
	void confirm();
};
