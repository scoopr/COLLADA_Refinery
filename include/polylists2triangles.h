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

#ifndef _POLYLISTS2TRIANGLES_H_
#define _POLYLISTS2TRIANGLES_H_

#include "conditionerBase.h"

#include <dom/domCOLLADA.h>

class Polylist2Triangles : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "polylists2triangles"; }
	REF_EXPORT std::string getDescription() const { return "polylists2triangles"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }

private:
	void createTrianglesFromPolylist( domMesh *thisMesh, domPolylist *thisPolylist );
	//void createTrianglesFromPolygons( domMesh *thisMesh, domPolygons *thisPolygons );
	unsigned int getMaxOffset( domInputLocalOffset_Array &input_array );
};

class Polylist2TrianglesProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "polylists2triangles"; }
	REF_EXPORT std::string getDescription() const { return "polylists2triangles"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }
	REF_EXPORT Conditioner *create() const { return new Polylist2Triangles(); }
};

#endif


