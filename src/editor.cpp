/*
 *  editor.cpp
 *  qtex
 *
 *  Created by Michel Steuwer on 18.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "editor.h"

Editor::Editor( QWidget * parent )
: QTextEdit(parent)
{
	highlighter = new Highlighter(document());
}

