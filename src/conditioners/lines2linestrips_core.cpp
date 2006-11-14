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
#include "lines2linestrips.h"

struct ltstr
{
  bool operator()(const lineStruct s1, const lineStruct s2) const
  {
	  if (s1.start < s2.start) {
		return true;
	  } else if (s1.start == s2.start) {
		if (s1.end <= s2.end) 
			return true;
		else 
			return false;
	  } else
		return false;
  }
};

bool Lines2linestrips::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

int Lines2linestrips::execute()
{
	string axis;
	bool verbose = false;
	getBoolOption("v", verbose);

	int VERBOSE = verbose;
	map<lineStruct, bool, ltstr> lineArray;
	map<lineStruct, bool, ltstr>::iterator li;
	map<lineStruct, bool, ltstr>::iterator lli;

	int error = 0;

	// How many geometry elements are there?
	int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry", getInput(0).c_str()));
//	if(verbose)	cerr<<"There are "<<geometryElementCount<<" geometry elements in this file\n"; 

	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Find the next geometry element
		domGeometry *thisGeometry;
		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", getInput(0).c_str());

		// Get the mesh out of the geometry
		domMesh *thisMesh = thisGeometry->getMesh();
		if (thisMesh == 0) continue;

		// Loop over all the lines elements
		int lineCount = (int)(thisMesh->getLines_array().getCount());

		if (lineCount  == 0)
			continue;

		domLinestrips *thisLinestrips = (domLinestrips *)thisMesh->createAndPlace("linestrips");

//		if(verbose)	cerr<<"There are "<<lineCount<<" line elements in this file\n"; 
		for(int currLine = 0; currLine<lineCount; currLine++)
		{
			// Get the lines out of the mesh
			domLines *thisLines = thisMesh->getLines_array().get(0);
			
			// add inputs
			unsigned int offsetcount = 0;
			for(unsigned int i=0; i<thisLines->getInput_array().getCount(); i++)
			{
				unsigned int tempoffset = (unsigned int) thisLines->getInput_array()[i]->getOffset();
				if (offsetcount < tempoffset) offsetcount = tempoffset;
				thisLinestrips->placeElement( thisLines->getInput_array()[i]->clone() );
			}
			offsetcount++; //offset is 1 start
			// add names
			if (thisLines->getName())
				thisLinestrips->setName(thisLines->getName());
			// set materials
			thisLinestrips->setMaterial(thisLines->getMaterial());

			// set P
			domListOfUInts lineValue = thisLines->getP()->getValue();

			for(unsigned int i=0; i<thisLines->getCount(); i++)
			{
				lineStruct line;
				if (lineValue[2*i*offsetcount] <= lineValue[2*i*offsetcount+offsetcount]) {
					line.start = lineValue[2*i*offsetcount];
					line.end = lineValue[2*i*offsetcount+offsetcount];
				} else {
					line.start = lineValue[2*i*offsetcount+offsetcount];
					line.end = lineValue[2*i*offsetcount];
				}
				lineArray.insert(pair<lineStruct, bool>(line, false));
			}
    
			// check for the possible line strips assume the duplicates have been removed
			vector<domUint> lineStrips;
			vector<domUint>::iterator vi;
			unsigned int lineStripsCount = 0;
			for (li = lineArray.begin(); li != lineArray.end(); li++) {
				if ( (*li).second == false) {
					lineStrips.push_back((*li).first.start);
					lineStrips.push_back((*li).first.end);
					(*li).second = true;

					domUint temp = (*li).first.end;
					for (lli = lineArray.begin(); lli != lineArray.end(); lli++) {
						if (((*lli).second == false) && (temp == (*lli).first.start)) {
							lineStrips.push_back((*lli).first.end);
							temp = (*lli).first.end;
							(*lli).second = true;
						} 
					}

					// create <p> for each line strips
					domP*  linestripP = (domP*) thisLinestrips->createAndPlace("p");
					for (vi = lineStrips.begin(); vi != lineStrips.end(); vi++) {
						if (offsetcount == 1)
							linestripP->getValue().append((*vi));
						else {
							if (verbose) printf("inside strip append %d\n", (*vi));
							linestripP->getValue().append((*vi));
							for (unsigned int i = 0; i < thisLines->getCount()*offsetcount*2; i++) {
								if (lineValue[i] == (*vi)) {
									if (verbose) printf("find match i is %d, lineValue is %d\n", i, lineValue[i]); 
									for (unsigned int j = 1; j < offsetcount; j++) {
										linestripP->getValue().append(lineValue[i+j]);
									} // for
									break;
								} // if
							} // for
						}
					} // for
					lineStripsCount++;
					lineStrips.clear();
				} // if
			} // for

			thisLinestrips->setCount(lineStripsCount);
			thisMesh->removeChildElement(thisLines);
		}
	}
	return 0;
}

Conditioner::Register<Lines2linestripsProxy> registerObject;
