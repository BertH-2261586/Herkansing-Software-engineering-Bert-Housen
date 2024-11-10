#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class BracketHighlighter : public QSyntaxHighlighter {
	Q_OBJECT
public:
	BracketHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent) {
		m_bracketFormat.setForeground(Qt::black);
		m_bracketFormat.setBackground(Qt::blue);
		m_bracketFormat.setFontWeight(QFont::Bold);

		m_bracketPattern = QRegularExpression(R"(\[[^\[\]]*\])");
	}

private:
	QTextCharFormat m_bracketFormat;
	QRegularExpression m_bracketPattern;

	void highlightBlock(const QString& text) override {
		QRegularExpressionMatchIterator matchIterator = m_bracketPattern.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			int start = match.capturedStart(0);
			int length = match.capturedLength(0);

			setFormat(start, length, m_bracketFormat);
		}
	}
};