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
#ifndef __AXISCONVERTER_H__
#define __AXISCONVERTER_H__

#include "conditionerBase.h"

// Collada API includes
#include "dae.h" 
#include "dae/daeSIDResolver.h"
#include "dom/domCOLLADA.h"
#include "dom/domConstants.h"
//#include "dom/domProfile_COMMON.h"
//#include "dom/domCommon_color_or_texture_type.h"
//#include "dom/domPlane.h"

#include "defines.h"

#include <string>
#include <set>
#include <map>
#include <iostream>
using namespace std;

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

#define Y_TO_X 1
#define Z_TO_X 2
#define X_TO_Y 3
#define Z_TO_Y 4
#define X_TO_Z 5
#define Y_TO_Z 6

class Axisconverter : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "Axisconverter"; }
	REF_EXPORT std::string getDescription() const { return "change axis"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
};

class AxisconverterProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "Axisconverter"; }
	REF_EXPORT std::string getDescription() const { return "change axis"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
	REF_EXPORT Conditioner *create() const { return new Axisconverter(); }
};

#endif //__AXISCONVERTER_H__
