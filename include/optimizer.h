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
#ifndef __OPTIMIZER_H__
#define __OPTIMIZER_H__

#include <map>
#include <list>

using namespace std;

// Collada API includes

#include "dae.h" 
#include "dom/domCOLLADA.h"

#include "conditionerBase.h"
#include "defines.h"

#include "packedvertexindex.h"
#include "vertexcache.h"

#define MAX_INPUTS 10  // Maximum number of inputs per polygon (this should really be dynamic)


//--------------------------------------------------------------------------------

class Optimizer : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "Optimizer"; }
	REF_EXPORT std::string getDescription() const { return "optimizer vertex to get max cache hits"; }
	REF_EXPORT std::string getCategory() const  { return"geometry processing"; }
};

class OptimizerProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "Optimizer"; }
	REF_EXPORT std::string getDescription() const { return "optimizer vertex to get max cache hits"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }
	REF_EXPORT Conditioner *create() const { return new Optimizer(); }
};


#endif //__OPTIMIZER_H__
