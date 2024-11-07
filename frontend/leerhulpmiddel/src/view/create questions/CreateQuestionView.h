#pragma once

#include <QWidget>
#include <QComboBox>
#include <QStackedWidget>

#include "CreateFlashcardView.h"
#include "CreateMultipleChoiceQuestionView.h"
#include "CreateFillInQuestionView.h"

class CreateQuestionView : public  QWidget {
	Q_OBJECT
public:
	CreateQuestionView(QWidget* parent=nullptr);
private:
	QComboBox* m_cboQuestionType;
	QStackedWidget* m_questionSelection;

	CreateFlashcardView* m_createFlashcardView;
	CreateMultipleChoiceQuestionView* m_createMultipleChoiceQuestionView;
	CreateFillInQuestionView* m_createFillInQuestionView;
private slots:
	void confirm();
};