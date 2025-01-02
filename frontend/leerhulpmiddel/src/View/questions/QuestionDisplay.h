#pragma once

#include "../../model/Questions/Question.h"

#include <QWidget>

class QuestionDisplay : public QWidget
{
	Q_OBJECT
public:
	QuestionDisplay(Question* question);
};

