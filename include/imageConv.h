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
#ifndef __IMAGECONV_H__
#define __IMAGECONV_H__

#include "conditionerBase.h"

// Collada API includes
#include "dae.h" 
#include "dom/domCOLLADA.h"

#include "defines.h"

class Imageconv : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "imageconverter"; }
	REF_EXPORT std::string getDescription() const { return "imageconverter"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
};

class ImageconvProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "imageconverter"; }
	REF_EXPORT std::string getDescription() const { return "imageconverter"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
	REF_EXPORT Conditioner *create() const { return new Imageconv(); }
};

#endif //__IMAGECONV_H__
