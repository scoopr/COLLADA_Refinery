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
	int error = 0;
    
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
