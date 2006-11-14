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
#ifndef __MUXER_H__
#define __MUXER_H__

#ifdef DLL_EXPORT
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#endif

// Collada API includes
#include "dae.h" 
#include "dom/domCOLLADA.h"
#include "dom/domConstants.h"
#include "dom/domProfile_COMMON.h"
#include "dom/domCommon_color_or_texture_type.h"

#include "defines.h"

//using namespace std;

DECLARESPEC  int CALLBACKSPEC muxer (DAE *input, const char *inputdocumentname, const char *outputdocumentname, int verbose = 0);


#endif //__MUXER_H__
