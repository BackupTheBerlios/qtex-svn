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
#include <QFileDialog>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QMessageBox>
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
    bool find(QString, QTextDocument::FindFlags);
    QString getFilename();
    bool getCopy();
    bool getRedo();
    bool getUndo();
    bool gotoLine(int);
    bool hasChanged();
    void loadSettings();
    bool maybeSave();
    bool openDocument(QString);
    void replace(QString, QString, QTextDocument::FindFlags, bool);
    bool save();
    bool saveAs();
    void setChanged(bool);
    void setFilename(QString);
    
  signals:
    void signalCursorPositionChanged(QString);
    
  private slots:
    void slotChanged();
    void slotCursorPositionChanged();
    void slotCopyAvailable(bool);
    void slotRedoAvailable(bool);
    void slotUndoAvailable(bool);
		
	private: 
    bool event(QEvent *);
    void setCursorPos(int);
    
    bool m_canCopy;
    bool m_canRedo;
    bool m_canUndo;
    bool m_changeState;
		Highlighter *m_highlighter;
    int m_lineMargin;
    QString m_filename;
    QTextCursor *m_cursor;
	};

#endif
