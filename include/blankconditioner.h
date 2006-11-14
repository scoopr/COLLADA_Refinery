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
// System includes
#ifdef DLL_EXPORT
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#endif

// Collada API includes

#include "dae.h" 
#include "dom/domCOLLADA.h"
#include "dom/domConstants.h"

#include "defines.h"
#include <vector>
#include <string>
#include <set>
using namespace std;

DECLARESPEC int CALLBACKSPEC blank (DAE *input, bool verbose = 0);
