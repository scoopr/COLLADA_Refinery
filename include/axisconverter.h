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
#ifndef __AXISCONVERTER_H__
#define __AXISCONVERTER_H__

#include "conditionerBase.h"

// Collada API includes
#include "dae.h" 
#include "dom/domCOLLADA.h"
#include "dom/domConstants.h"
//#include "dom/domProfile_COMMON.h"
//#include "dom/domCommon_color_or_texture_type.h"
//#include "dom/domPlane.h"

#include "defines.h"

#include <string>
#include <set>
#include <map>
#include <iostream>
using namespace std;

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

#define Y_TO_X 1
#define Z_TO_X 2
#define X_TO_Y 3
#define Z_TO_Y 4
#define X_TO_Z 5
#define Y_TO_Z 6

class Axisconverter : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "Axisconverter"; }
	REF_EXPORT std::string getDescription() const { return "change axis"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
};

class AxisconverterProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "Axisconverter"; }
	REF_EXPORT std::string getDescription() const { return "change axis"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
	REF_EXPORT Conditioner *create() const { return new Axisconverter(); }
};

#endif //__AXISCONVERTER_H__
