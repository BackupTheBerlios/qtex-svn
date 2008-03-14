/*
 *  format.cpp
 *  qtex
 *
 */

#include "format.h"

//constructor

Format::Format() {
	m_bold.first = false; // Bold has not been modified (yet)
	m_italic.first = false;// Italic modified?
	m_foreground.first = false;// Foreground modified?
	m_background.first = false;// Background modified?
}

//copy constructor
Format::Format( const Format& format ) {
	m_bold = format.m_bold;
	m_italic = format.m_italic;
	m_foreground = format.m_foreground;
	m_background = format.m_background;
}

//deconstructor
Format::~Format() {
	
}

//the rest
void Format::setBold(const bool b) {
	m_bold.first = true; //Bold has been modified
	m_bold.second = b;//Set the value
}

void Format::setItalic(const bool b) {
	m_italic.first = true;//Italic has been modified
	m_italic.second = b;//Set the value
}

void Format::setForeground(const QColor& color) {
	m_foreground.first = true;//Foreground has been modified
	m_foreground.second = color;//Set the color
}

void Format::setBackground(const QColor& color) {
	m_background.first = true;//Background has been modified
	m_background.second = color;//Set the color
}

void Format::merge(QTextCharFormat& TextFormat) {
	if (m_bold.first == true) { //Bold has been modified
		if (m_bold.second == true)//Bold-value is set to bold
			TextFormat.setFontWeight(QFont::Bold);
		else //Bold-value is set to normal
			TextFormat.setFontWeight(QFont::Normal);
	}
	if (m_italic.first == true) { //Italic has been modified
		if (m_italic.second == true)
			TextFormat.setFontItalic(true);
		else
			TextFormat.setFontItalic(false);
	}
	if (m_foreground.first == true) { //Foreground has been modified
		TextFormat.setForeground(m_foreground.second);
	}
	if (m_background.first == true) { //Background has been modified
		TextFormat.setBackground(m_background.second);
	}
}
