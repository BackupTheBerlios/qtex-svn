/*
 *  highlither.h
 *  qtex
 *
 *  Created by Michel Steuwer on 18.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

class Highlighter : public QSyntaxHighlighter
	{
		Q_OBJECT
		
	public:
		Highlighter(QTextDocument *parent = 0);
		
	protected:
		void highlightBlock(const QString &text);
		
	private:
		struct HighlightingRule
		{
			QRegExp pattern;
			QTextCharFormat format;
		};
		QVector<HighlightingRule> highlightingRules;
		
		QRegExp mathModeStartExpression;
		QRegExp mathModeEndExpression;
		
		//QTextCharFormat keywordFormat;
		QTextCharFormat latexFormat;
		QTextCharFormat singleLineCommentFormat;
		QTextCharFormat mathModeFormat;
		//QTextCharFormat quotationFormat;
		//QTextCharFormat functionFormat;
	};

#endif

