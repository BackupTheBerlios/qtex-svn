/*
 *  editor.h
 *  qtex
 *
 *  Created by Michel Steuwer on 18.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EDITOR_H
#define EDITOR_H

#include <QColor>
#include <QEvent>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QRectF>
#include <QScrollBar>
#include <QSettings>
#include <QTextEdit>
#include <QTextLayout>
#include "highlighter.h"

class Editor : public QTextEdit
	{
		Q_OBJECT
		
	public:
		Editor( QWidget * parent = 0 );
    QString getFilename();
    bool getCopy();
    bool getRedo();
    bool getUndo();
    bool gotoLine(int);
    bool hasChanged();
    void loadSettings();
    bool maybeSave();
    bool openDocument(QString);
    bool save();
    bool saveAs();
    void setChanged(bool);
    void setFilename(QString);
    
  signals:
    void newCursorPosition(QString);
    
  private slots:
    void changed();
    void dummy();
    void setCopy(bool);
    void setRedo(bool);
    void setUndo(bool);
		
	private: 
    bool event(QEvent *);
    
    bool canCopy;
    bool canRedo;
    bool canUndo;
    bool changeState;
		Highlighter *highlighter;
    int lineMargin;
    QLabel *lineNumbers;
    QString filename;
    QTextCursor *cursor;
	};

#endif
