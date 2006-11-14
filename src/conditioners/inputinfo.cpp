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
#include "inputinfo.h"
//--------------------------------------------------------------------------------
// Standard constructor
inputInfo::inputInfo()
{
	output = NULL;
	accessor = NULL;
	data = NULL;
	stride = 0;
	controller_output = NULL;
	controller_accessor = NULL;
	controller_data = NULL;
}
//--------------------------------------------------------------------------------
// Standard destructor
inputInfo::~inputInfo()
{
	// If any memory was allocated, free it
	if(data != NULL)
		delete [] data;
	if(controller_data != NULL)
		delete [] controller_data;
}
//--------------------------------------------------------------------------------
// Allocate space for "size" floats of geometry source data
void inputInfo::allocateData(int size)
{
	data = new domFloat[size];
}
//--------------------------------------------------------------------------------
// Allocate space for "size" floats of controller source data
void inputInfo::allocateControllerData(int size)
{
	controller_data = new domFloat[size];
}

inputInfo::inputInfo(domSource * thisSource)
{
	controller_output = NULL;
	controller_accessor = NULL;
	controller_data = NULL;
	domFloat_array * thisArray = thisSource->getFloat_array();
	output = thisArray;
	unsigned int count = (unsigned int)thisArray->getCount();
	allocateData(count);
	memcpy(data, thisArray->getValue().getRawData(), count * sizeof(domFloat));
	// Now figure out the stride of the data
	accessor = thisSource->getTechnique_common()->getAccessor();
	stride = (int)accessor->getStride();
	// Now clean out the float array in the source so we can put our rearranged data there later
	thisArray->getValue().clear();
}
