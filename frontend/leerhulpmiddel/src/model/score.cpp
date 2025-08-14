
#include "score.h"

Score::Score(QJsonObject jsonData)
{
    m_percent = jsonData["percent"].toInt();
    m_student = jsonData["student"].toString();
}

