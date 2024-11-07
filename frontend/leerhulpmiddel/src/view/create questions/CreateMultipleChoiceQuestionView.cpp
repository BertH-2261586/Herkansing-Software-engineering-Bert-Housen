#include "CreateMultipleChoiceQuestionView.h"

#include <QLabel>
#include <QVBoxLayout>

CreateMultipleChoiceQuestionView::CreateMultipleChoiceQuestionView(QWidget* parent) : QWidget(parent) {
	QLabel* lblMC = new QLabel(this);
	lblMC->setText("Multiple Choice");

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(lblMC);

	setLayout(mainLayout);
}
