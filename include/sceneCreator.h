/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License 
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 * implied. See the License for the specific language governing permissions and limitations under the 
 * License. 
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
