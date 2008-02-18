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

#include <QTextEdit>
#include "highlighter.h"

class Editor : public QTextEdit
	{
		Q_OBJECT
		
	public:
		Editor( QWidget * parent = 0 );
    QString getFilename();
    bool hasChanged();
    void save();
    void saveAs();
    void setChanged(bool);
    void setFilename(QString);
    
  private slots:
    void changed();
		
	private:
    bool changeState;
		Highlighter *highlighter;
    QString filename;
	};

#endif
