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
#ifndef __POLYGONS2POLYLIST_H__
#define __POLYGONS2POLYLIST_H__

#include "conditionerBase.h"

// Collada API includes
#include "dae.h" 
#include "dom/domCOLLADA.h"

#include "defines.h"
#include "utility.h"

class Polygons2Polylists : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "polygons2polylists"; }
	REF_EXPORT std::string getDescription() const { return "converts polygon elements to polylist elements"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }
private:
	void createTrianglesFromTrifans( domMesh *thisMesh, domTrifans *thisTrifans );
};

class Polygons2PolylistsProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "polygons2polylists"; }
	REF_EXPORT std::string getDescription() const { return "converts polygon elements to polylist elements"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }
	REF_EXPORT Conditioner *create() const { return new Polygons2Polylists(); }
};


#endif //__POLYGONS2POLYLIST_H__
