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
