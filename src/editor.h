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
#include <QSettings>
#include <QTextEdit>
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
    bool hasChanged();
    void loadSettings();
    bool maybeSave();
    bool openDocument(QString);
    bool save();
    bool saveAs();
    void setChanged(bool);
    void setFilename(QString);
    
  private slots:
    void changed();
    void setCopy(bool);
    void setRedo(bool);
    void setUndo(bool);
		
	private: 
    bool canCopy;
    bool canRedo;
    bool canUndo;
    bool changeState;
		Highlighter *highlighter;
    QString filename;
    QTextCursor *cursor;
	};

#endif
