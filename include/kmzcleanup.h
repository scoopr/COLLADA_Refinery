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
#ifndef __KMZCLEANUP_H__
#define __KMZCLEANUP_H__

#include "conditionerBase.h"

// Collada API includes
#include "dae.h" 
#include "dae/daeURI.h"
#include "dom/domCOLLADA.h"
#include "dom/domConstants.h"
#include "dom/domProfile_COMMON.h"
#include "dom/domCommon_color_or_texture_type.h"

#include "defines.h"

#include <string>
#include <map>
#include <iostream>
using namespace std;
/*template <typename T> 
inline T *daeSafeCast( daeElement *element ) 
{ 
	if ( element && element->getMeta() == T::_Meta ) 
		return (T *)element; 
	return NULL; 
} */

class Kmzcleanup : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "kmzcleanup"; }
	REF_EXPORT std::string getDescription() const { return "kmzcleanup"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
private:
	bool checkbindneeded(DAE* input, daeString material, int verbose);
	template<class T> int processprimitive(T primitive, int verbose);
};

class KmzcleanupProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "kmzcleanup"; }
	REF_EXPORT std::string getDescription() const { return "kmzcleanup"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
	REF_EXPORT Conditioner *create() const { return new Kmzcleanup(); }
};


#endif //__KMZCLEANUP_H__
