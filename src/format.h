/*
 *  format.h
 *  qtex
 *
 */
#ifndef __FORMAT_H_
#define __FORMAT_H_

#include <QColor>
#include <QTextCharFormat>

#include <utility>
using namespace std;

class Format {
public:
	// constructor
	Format();
	// copy constructor
	Format( const Format& );
	// deconstructor
	~Format();
	// the rest
	void setBold(const bool);
	void setItalic(const bool);
	void setForeground(const QColor&);
	void setBackground(const QColor&);
	void merge(QTextCharFormat&);
private:
	pair<bool,bool> m_bold;
	pair<bool,bool> m_italic;
	pair<bool,QColor> m_foreground;
	pair<bool,QColor> m_background;
};

#endif