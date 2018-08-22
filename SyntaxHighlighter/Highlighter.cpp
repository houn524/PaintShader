#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegularExpression("\".*\"");
	rule.format = quotationFormat;
	highlightinRules.append(rule);

	numberFormat.setForeground(QColor(0xFFCD22));
	rule.pattern = QRegularExpression("\\b\\d*\\.?\\d+\\b");
	rule.format = numberFormat;
	highlightinRules.append(rule);

	keywordFormat.setForeground(QColor(0x93C763));
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bvoid\\b" << "\\bif+(?=\\()" << "\\belse\\b"
		<< "\\buniform\\b" << "\\bout\\b" << "\\bin\\b" << "\\blayout\\b"
		<< "\\bfloat\\b" << "\\bint\\b" << "\\bbool\\b" << "\\bwhile+(?=\\()"
		<< "\\bfor+(?=\\()";
	foreach(const QString &pattern, keywordPatterns)
	{
		rule.pattern = QRegularExpression(pattern);
		rule.format = keywordFormat;
		highlightinRules.append(rule);
	}

	builtInFormat.setForeground(QColor(0x8CBBAD));
	keywordPatterns.clear();
	keywordPatterns << "\\bvec2\\b" << "\\bvec3\\b" << "\\bvec4\\b"
		<< "\\bmat2\\b" << "\\bmat3\\b" << "\\bmat4\\b";
	foreach(const QString &pattern, keywordPatterns)
	{
		rule.pattern = QRegularExpression(pattern);
		rule.format = builtInFormat;
		highlightinRules.append(rule);
	}

	singleLineCommentFormat.setForeground(QColor(0x818E96));
	rule.pattern = QRegularExpression("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightinRules.append(rule);

	preprocessorFormat.setForeground(QColor(0x8CBBAD));
	rule.pattern = QRegularExpression("#[^\n]*");
	rule.format = preprocessorFormat;
	highlightinRules.append(rule);

	multiLineCommentFormat.setForeground(QColor(0x818E96));
	commentStartExpression = QRegularExpression("/\\*");
	commentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
	foreach(const HighlightingRule &rule, highlightinRules)
	{
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
		while (matchIterator.hasNext())
		{
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf(commentStartExpression);

	while (startIndex >= 0)
	{
		QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
		int endIndex = match.capturedStart();
		int commentLength = 0;
		if (endIndex == -1)
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex + match.capturedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
	}
}