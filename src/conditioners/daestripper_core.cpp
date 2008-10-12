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
#include "daestripper.h"
#include "utility.h"
#include "dom/domConstants.h"


void stripoutextra(DAE *input)
{
	int error = 0;
	int extraElementCount = (int)(input->getDatabase()->getElementCount(NULL, "extra", NULL));
	for (int currentExtra = 0; currentExtra < extraElementCount; currentExtra++)
	{
		domExtra *thisExtra;
		error = input->getDatabase()->getElement((daeElement**)&thisExtra,0, NULL, "extra");
		if (thisExtra != NULL) {
			daeElement *parent = thisExtra->getParentElement();
			parent->removeChildElement(thisExtra);
		}
	}
}

void stripouttechnique(DAE *input)
{
	int error = 0;
	int techniqueElementCount = (int)(input->getDatabase()->getElementCount(NULL, "technique", NULL));
	for (int currentTechnique = 0; currentTechnique < techniqueElementCount; currentTechnique++)
	{
		domTechnique *thisTechnique;
		error = input->getDatabase()->getElement((daeElement**)&thisTechnique,0, NULL, "technique");
		if (thisTechnique != NULL) {
			daeElement *parent = thisTechnique->getParentElement();
			parent->removeChildElement(thisTechnique);
		}
	}
}

void stripouttechniquespecified(DAE *input, const char *attribute)
{
	int error = 0;
	int position = 0;

	int techniqueElementCount = (int)(input->getDatabase()->getElementCount(NULL, "technique", NULL));
	for (int currentTechnique = 0; currentTechnique < techniqueElementCount; currentTechnique++)
	{
		domTechnique *thisTechnique;
		error = input->getDatabase()->getElement((daeElement**)&thisTechnique,position, NULL, "technique");
		if (thisTechnique != NULL) {
			xsNMTOKEN profile = thisTechnique->getProfile();
			if ((profile != NULL) && (stricmp(profile, attribute) == 0)) {
				daeElement *parent = thisTechnique->getParentElement();
				parent->removeChildElement(thisTechnique);
			} else {
				position++;
			}
		}
	}
}

int daestrip (DAE *input, const char *element, const char *attribute, int verbose = 0)
{
//	int error = 0;
    
	if (strcmp(element, "extra") == 0) {
		stripoutextra(input);
	} else if (strcmp(element, "technique") == 0) {
		if (strcmp(attribute, "any") == 0) 
			stripouttechnique(input);
		else 
			stripouttechniquespecified(input, attribute);
	}
	return 0;
}

bool Daestripper::init()
{
	addStringOption( "element", "element", "Element to remove", "");
	addStringOption( "attribute", "attribute", "Attribute to remove", "");
	return true;
}

int Daestripper::execute()
{
	std::string element;
	getStringOption( "element", element );
	std::string attribute;
	getStringOption( "attribute", attribute );

	daestrip(_dae, element.c_str(), attribute.c_str());

	return 0;
}

Conditioner::Register< DaestripperProxy >DaestripperProxy;
