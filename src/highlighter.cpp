/*
 *  highlither.cpp
 *  qtex
 *
 *  Created by Michel Steuwer on 18.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
	loadHighlighting();
}

void Highlighter::highlightBlock(const QString &text)
{	
	// zwei dynamische Listen
	//QList<IntervallWithFormat> keywords, mathModes;
	
	foreach (HighlightingRule rule, m_highlightingRules) {
		if(!rule.block) {
			QRegExp expression(rule.expression);
			int index = text.indexOf(expression);
			while (index >= 0) {
				int length = expression.matchedLength();
				/*IntervallWithFormat temp;
				temp.index = index;
				temp.length = length;
				temp.format = rule.format;
				keywords.append(temp);*/
				//setFormat(index, length, rule.format);
				applyFormat(index, length, rule.format);
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
				/*IntervallWithFormat temp;
				temp.index = startIndex;
				temp.length = mathModeLength;
				temp.format = m_mathModeFormat;
				mathModes.append(temp);*/
				//setFormat(startIndex, mathModeLength, m_mathModeFormat);
				applyFormat(startIndex, mathModeLength, m_mathModeFormat);
				/*startIndex = text.indexOf(mathModeStartExpression,
										  startIndex + mathModeLength);*/
				startIndex = text.indexOf(rule.startExpression,
										  startIndex + mathModeLength);
			}
		}
	}
	
	/*foreach (IntervallWithFormat mathMode, mathModes) {
    foreach (IntervallWithFormat keyword, keywords) {
			if (mathMode.index < keyword.index && mathMode.length+mathMode.index >= keyword.length+keyword.index) {
				QTextCharFormat mixed;
				mixed.setFontWeight(keyword.format.fontWeight());
				mixed.setForeground(keyword.format.foreground());
				mixed.setBackground(mathMode.format.background());
				setFormat(keyword.index, keyword.length, mixed);
			}
		}
	}*/
}

void Highlighter::loadHighlighting() {
  m_highlightingRules.clear();
  HighlightingRule rule;
	QColor tempColor;
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("syntaxhighlighting"));
	
  int state = settings.value(QString("latexBold"), Qt::Checked).toInt();
  if (state != 0) {
    //m_latexFormat.setFontWeight(QFont::Bold);
	  m_latexFormat.setBold(true);
  } else {
    //m_latexFormat.setFontWeight(QFont::Normal);
	  m_latexFormat.setBold(false);
  }
  state = settings.value(QString("latexItalic"), Qt::Unchecked).toInt();
  if (state != 0) {
    //m_latexFormat.setFontItalic(true);
	  m_latexFormat.setItalic(true);
  } else {
    //m_latexFormat.setFontItalic(false);
	  m_latexFormat.setItalic(false);
  }
	tempColor = settings.value(QString("latexForeground"), Qt::darkMagenta).value<QColor>();
	m_latexFormat.setForeground(tempColor);
	tempColor = settings.value(QString("latexBackground"), Qt::white).value<QColor>();
	m_latexFormat.setBackground(tempColor);
	
	rule.expression = QRegExp("\\\\[A-Za-z]+");
	rule.format = m_latexFormat;
	rule.block = false;
	m_highlightingRules.append(rule);
	
  state = settings.value(QString("commentBold"), Qt::Unchecked).toInt();
  if (state != 0) {
    //m_singleLineCommentFormat.setFontWeight(QFont::Bold);
	  m_singleLineCommentFormat.setBold(true);
  } else {
    //m_singleLineCommentFormat.setFontWeight(QFont::Normal);
	  m_singleLineCommentFormat.setBold(false);
  }
  state = settings.value(QString("commentItalic"), Qt::Checked).toInt();
  if (state != 0) {
    //m_singleLineCommentFormat.setFontItalic(true);
	  m_singleLineCommentFormat.setItalic(true);
  } else {
    //m_singleLineCommentFormat.setFontItalic(false);
	  m_singleLineCommentFormat.setItalic(false);
  }
	tempColor = settings.value(QString("commentForeground"), Qt::red).value<QColor>();
	m_singleLineCommentFormat.setForeground(tempColor);
	tempColor = settings.value(QString("commentBackground"), Qt::white).value<QColor>();
  m_singleLineCommentFormat.setBackground(tempColor);
	rule.expression = QRegExp("%[^\n]*");
	rule.format = m_singleLineCommentFormat;
	rule.block = false;
	m_highlightingRules.append(rule);
	
  state = settings.value(QString("mathBold"), Qt::Unchecked).toInt();
  if (state != 0) {
    //m_mathModeFormat.setFontWeight(QFont::Bold);
	  m_mathModeFormat.setBold(true);
  } else {
    //m_mathModeFormat.setFontWeight(QFont::Normal);
	  m_mathModeFormat.setBold(false);
  }
  state = settings.value(QString("mathItalic"), Qt::Unchecked).toInt();
  if (state != 0) {
    //m_mathModeFormat.setFontItalic(true);
	  m_mathModeFormat.setItalic(true);
  } else {
    //m_mathModeFormat.setFontItalic(false);
	  m_mathModeFormat.setItalic(false);
  }
	tempColor = settings.value(QString("mathForeground"), Qt::black).value<QColor>();
	m_mathModeFormat.setForeground(tempColor);
	tempColor = settings.value(QString("mathBackground"), Qt::yellow).value<QColor>();
  m_mathModeFormat.setBackground(tempColor);
	rule.startExpression = QRegExp("\\$.");
	rule.endExpression = QRegExp("\\$");
	rule.format = m_mathModeFormat;
	rule.block = true;
	m_highlightingRules.append(rule);
  
  settings.endGroup();
}

void Highlighter::applyFormat(int start,int length,Format& TextFormat) {
	QTextCharFormat TextCharFormat;
	TextCharFormat = format(start); //get current TextCharFormat
	TextFormat.merge(TextCharFormat); //merge current Format with the Highlighting Format
	setFormat(start, length, TextCharFormat);
}
