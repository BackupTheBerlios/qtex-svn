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

#include <QHash>
#include <QSettings>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT
	
  public:
  	Highlighter(QTextDocument *parent = 0);
    void loadHighlighting();
  	
  protected:
  	void highlightBlock(const QString &text);
  	
  private:
  	struct HighlightingRule
  	{	
  		QRegExp expression;
  		QRegExp startExpression;
  		QRegExp endExpression;
  		QTextCharFormat format;
  		int priority;
  		bool block;
  	};
  	
  	// Sktuktur, die ein Intevall(begin und länge) sowie ein dazugehöriges format aufnimmt
  	struct IntervallWithFormat
  	{
  		int index;
  		int length;
  		QTextCharFormat format;
  	};
    
  	QTextCharFormat m_latexFormat, m_singleLineCommentFormat, m_mathModeFormat;
    QVector<HighlightingRule> m_highlightingRules;
};

#endif

