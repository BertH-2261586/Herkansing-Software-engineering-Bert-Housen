#pragma once

#include <QWidget>
#include <QComboBox>
#include <QStackedWidget>
#include <QLineEdit>

#include "CreateFlashcardView.h"
#include "CreateMultipleChoiceQuestionView.h"
#include "CreateFillInQuestionView.h"
#include "../../Controller/questionmanagercontroller.h"

class CreateQuestionView : public  QWidget {
	Q_OBJECT
public:
	CreateQuestionView(QuestionManagerController* questionController, QWidget* parent = nullptr);
private:
	QLineEdit* m_txtQuestionName;
	QComboBox* m_cboQuestionType;
	QStackedWidget* m_questionSelection;

	CreateFlashcardView* m_createFlashcardView;
	CreateMultipleChoiceQuestionView* m_createMultipleChoiceQuestionView;
	CreateFillInQuestionView* m_createFillInQuestionView;

	QuestionManagerController* m_questionController;
private slots:
	void confirm();
};