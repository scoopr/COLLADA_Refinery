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
#ifndef __LINES2LINESTRIPS_H__
#define __LINES2LINESTRIPS_H__

// Collada API includes
#include "dae.h" 
#include "dom/domCOLLADA.h"
#include "conditionerBase.h"
#include "defines.h"

#include <map>
#include <vector>
using namespace std;

typedef struct {
	domUint start;
	domUint end;
} lineStruct;

class Lines2linestrips : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "Lines2linestrips"; }
	REF_EXPORT std::string getDescription() const { return "turn all lines to linestrips"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }
};

class Lines2linestripsProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "Lines2linestrips"; }
	REF_EXPORT std::string getDescription() const { return "turn all lines to linestrips"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }
	REF_EXPORT Conditioner *create() const { return new Lines2linestrips(); }
};

#endif //__LINES2LINESTRIPS_H__
