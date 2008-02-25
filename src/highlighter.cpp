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
	loadHighlighting();
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

void Highlighter::loadHighlighting() {
  highlightingRules.clear();
  HighlightingRule rule;
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("syntaxhighlighting"));
	
  int state = settings.value(QString("latexBold"), Qt::Checked).toInt();
  if (state != 0) {
    latexFormat.setFontWeight(QFont::Bold);
  } else {
    latexFormat.setFontWeight(QFont::Normal);
  }
  state = settings.value(QString("latexItalic"), Qt::Unchecked).toInt();
  if (state != 0) {
    latexFormat.setFontItalic(true);
  } else {
    latexFormat.setFontItalic(false);
  }
	latexFormat.setForeground(settings.value(QString("latexForeground"), Qt::darkMagenta).value<QColor>());
  latexFormat.setBackground(settings.value(QString("latexBackground"), Qt::white).value<QColor>());
	rule.expression = QRegExp("\\\\[A-Za-z]+");
	rule.format = latexFormat;
	rule.block = false;
	highlightingRules.append(rule);
	
  state = settings.value(QString("commentBold"), Qt::Unchecked).toInt();
  if (state != 0) {
    singleLineCommentFormat.setFontWeight(QFont::Bold);
  } else {
    singleLineCommentFormat.setFontWeight(QFont::Normal);
  }
  state = settings.value(QString("commentItalic"), Qt::Checked).toInt();
  if (state != 0) {
    singleLineCommentFormat.setFontItalic(true);
  } else {
    singleLineCommentFormat.setFontItalic(false);
  }
	singleLineCommentFormat.setForeground(settings.value(QString("commentForeground"), Qt::red).value<QColor>());
  singleLineCommentFormat.setBackground(settings.value(QString("commentBackground"), Qt::white).value<QColor>());
	rule.expression = QRegExp("%[^\n]*");
	rule.format = singleLineCommentFormat;
	rule.block = false;
	highlightingRules.append(rule);
	
	//mathModeFormat.setBackground(Qt::yellow);
	
	//mathModeStartExpression = QRegExp("\\$.");
	//mathModeEndExpression = QRegExp("\\$");
  state = settings.value(QString("mathBold"), Qt::Unchecked).toInt();
  if (state != 0) {
    mathModeFormat.setFontWeight(QFont::Bold);
  } else {
    mathModeFormat.setFontWeight(QFont::Normal);
  }
  state = settings.value(QString("mathItalic"), Qt::Unchecked).toInt();
  if (state != 0) {
    mathModeFormat.setFontItalic(true);
  } else {
    mathModeFormat.setFontItalic(false);
  }
	mathModeFormat.setForeground(settings.value(QString("mathForeground"), Qt::black).value<QColor>());
  mathModeFormat.setBackground(settings.value(QString("mathBackground"), Qt::yellow).value<QColor>());
	rule.startExpression = QRegExp("\\$.");
	rule.endExpression = QRegExp("\\$");
	rule.format = mathModeFormat;
	rule.block = true;
	highlightingRules.append(rule);
  
  settings.endGroup();
}
