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
#ifndef __INPUT_INFO_H__
#define __INPUT_INFO_H__

#include "dom/domCOLLADA.h"

class inputInfo
{
public:
	inputInfo();
	virtual ~inputInfo();
	inputInfo(domSource *thisSource);

	// Allocate space to store a COLLADA float array in the class
	void allocateData(int size);
	void allocateControllerData(int size);

	domFloat_array	*output;		// This is the location of the (cleared) source array where we put our deindexed data
	domAccessor		*accessor;		// The accessor for this input so we can change the count
	domFloat		*data;			// This is a copy of the original source data
	int				stride;
	domFloat_array	*controller_output;	// Same as above but for an associated controller, may be null if no controller
	domAccessor		*controller_accessor;
	domFloat		*controller_data;
};

#endif //__INPUT_INFO_H__
