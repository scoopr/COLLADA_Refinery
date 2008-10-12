/*
The MIT License

Copyright 2006 Sony Computer Entertainment Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. 
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
