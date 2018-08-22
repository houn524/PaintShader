#pragma once
#ifndef __HIGHLIGHTER_H__
#define __HIGHLIGHTER_H__

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	Highlighter(QTextDocument *parent = 0);

protected:
	void highlightBlock(const QString &text) override;

private:
	struct HighlightingRule
	{
		QRegularExpression pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightinRules;

	QRegularExpression commentStartExpression;
	QRegularExpression commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat builtInFormat;
	QTextCharFormat numberFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat preprocessorFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
};

#endif