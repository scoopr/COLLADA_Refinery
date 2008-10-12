/*
The MIT License

Copyright 2006 Sony Computer Entertainment Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. 
 */
#ifndef __SCENECREATOR_H__
#define __SCENECREATOR_H__

#ifdef DLL_EXPORT
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#endif

// Collada API includes
//#include "dae.h" 
class DAE;
#include "defines.h"

DECLARESPEC  int CALLBACKSPEC sceneCreator (DAE *input, const char *output = 0 );

#include <gtk/gtk.h>

#include <dom/domElements.h>

class NodeMaker;

class SceneCreator
{
public:
	SceneCreator( DAE *input, const char *output = NULL );
	~SceneCreator();

	GtkWidget *getWidget() { return _sceneCreator; }

	void on_open();
	void on_load();
	void on_save_as();
	void on_quit();
    void on_NewNode();
    void on_EditNode();
    void on_SaveNQuit();

	void on_NMok();
	void on_NMcancel();

private:
	void create_GUI();

	DAE *_dae;

	domCOLLADARef _dom;
	domVisual_scene *_scene;

	GtkWidget *_sceneCreator;
	GtkWidget *_nodeList;
	GtkTreeSelection *_select;

	NodeMaker *_nm;

	bool _new;

	const char *_filename;
};

#endif //__SCENECREATOR_H__
