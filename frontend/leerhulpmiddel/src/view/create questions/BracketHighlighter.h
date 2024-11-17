#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QPalette>

class BracketHighlighter : public QSyntaxHighlighter {
	Q_OBJECT
public:
	BracketHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent) {
		QPalette palette = qApp->palette();

		m_bracketFormat.setForeground(palette.color(QPalette::WindowText));
		m_bracketFormat.setBackground(palette.color(QPalette::Highlight).lighter(150));
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