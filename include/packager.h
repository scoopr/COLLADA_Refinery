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
//
// herbert.law@playstation.sony.com
#ifndef _PACKAGER_H_
#define _PACKAGER_H_

#include <list>
#include <set>
#include <vector>
using namespace std;

#include "dae.h" 
#include "dom/domCOLLADA.h"
#include "dom/domfx_include_common.h"
#include "dom/domConstants.h"
#include "conditionerBase.h"
#include "defines.h"

class Packager : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "Packager"; }
	REF_EXPORT std::string getDescription() const { return "zip all dae and resources"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }

private:
	vector<string> gatherExternals( const daeString &docName, bool needsTemp, bool verbose );
};

class PackagerProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "Packager"; }
	REF_EXPORT std::string getDescription() const { return "zip all dae and resources"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
	REF_EXPORT Conditioner *create() const { return new Packager(); }
};

#endif //_PACKAGER_H_
