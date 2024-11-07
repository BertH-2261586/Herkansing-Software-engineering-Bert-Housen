#include "CreateFillInQuestionView.h"

#include <QLabel>
#include <QVBoxLayout>

CreateFillInQuestionView::CreateFillInQuestionView(QWidget* parent) {
	QLabel* lblMC = new QLabel(this);
	lblMC->setText("Fill In");

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(lblMC);

	setLayout(mainLayout);
}
