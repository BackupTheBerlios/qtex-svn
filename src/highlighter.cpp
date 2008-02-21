/*
 *  highlither.cpp
 *  qtex
 *
 *  Created by Michel Steuwer on 18.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <QtGui>

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;
	
	latexFormat.setFontWeight(QFont::Bold);
	latexFormat.setForeground(Qt::darkMagenta);
	rule.expression = QRegExp("\\\\[A-Za-z]+");
	rule.format = latexFormat;
	rule.block = false;
	highlightingRules.append(rule);
	
	singleLineCommentFormat.setForeground(Qt::red);
	rule.expression = QRegExp("%[^\n]*");
	rule.format = singleLineCommentFormat;
	rule.block = false;
	highlightingRules.append(rule);
	
	mathModeFormat.setBackground(Qt::yellow);
	
	//mathModeStartExpression = QRegExp("\\$.");
	//mathModeEndExpression = QRegExp("\\$");
	rule.startExpression = QRegExp("\\$.");
	rule.endExpression = QRegExp("\\$");
	rule.format = mathModeFormat;
	rule.block = true;
	highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{	
	// zwei dynamische Listen
	QList<IntervallWithFormat> keywords;
	QList<IntervallWithFormat> mathModes;
	
	foreach (HighlightingRule rule, highlightingRules) {
		if(!rule.block) {
			QRegExp expression(rule.expression);
			int index = text.indexOf(expression);
			while (index >= 0) {
				int length = expression.matchedLength();
				IntervallWithFormat temp;
				temp.index = index;
				temp.length = length;
				temp.format = rule.format;
				keywords.append(temp);
				setFormat(index, length, rule.format);
				index = text.indexOf(expression, index + length);
			}
		} else {
			// mathMode
			setCurrentBlockState(0);
			
			int startIndex = 0;
			if (previousBlockState() != 1)
				//startIndex = text.indexOf(mathModeStartExpression);
				startIndex = text.indexOf(rule.startExpression);
			
			while (startIndex >= 0) {
				//int endIndex = text.indexOf(mathModeEndExpression, startIndex+1);
				int endIndex = text.indexOf(rule.endExpression, startIndex+1);
				int mathModeLength;
				if (endIndex == -1) {
					setCurrentBlockState(1);
					mathModeLength = text.length() - startIndex;
				} else {
					/*mathModeLength = endIndex - startIndex
					+ mathModeEndExpression.matchedLength();*/
					mathModeLength = endIndex - startIndex
					+ rule.endExpression.matchedLength();
				}
				IntervallWithFormat temp;
				temp.index = startIndex;
				temp.length = mathModeLength;
				temp.format = mathModeFormat;
				mathModes.append(temp);
				setFormat(startIndex, mathModeLength, mathModeFormat);
				/*startIndex = text.indexOf(mathModeStartExpression,
										  startIndex + mathModeLength);*/
				startIndex = text.indexOf(rule.startExpression,
										  startIndex + mathModeLength);
			}
		}
	}
	foreach (IntervallWithFormat keyword, keywords) {
		foreach (IntervallWithFormat mathMode, mathModes) {
			if (mathMode.index < keyword.index && mathMode.length+mathMode.index >= keyword.length+keyword.index) {
				QTextCharFormat mixed;
				mixed.setFontWeight(keyword.format.fontWeight());
				mixed.setForeground(keyword.format.foreground());
				mixed.setBackground(mathMode.format.background());
				setFormat(keyword.index, keyword.length, mixed);
			}
		}
	}
}