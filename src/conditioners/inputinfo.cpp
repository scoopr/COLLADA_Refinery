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
