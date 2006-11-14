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
#include "trifanner.h"

struct ltstr
{
  bool operator()(const triangleStruct s1, const triangleStruct s2) const
  {
	  if (s1.first < s2.first) {
		return true;
	  } else if (s1.first == s2.first) {
		  if (s1.second < s2.second){
			return true;
		  } else if (s1.second == s2.second){
			if (s1.third <= s2.third)
				return true;
			else
				return false;
		  } else {
			return false;
		  }
	  } else {
		return false;
	  }
  }
};

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void createTrifansFromTriangles( domMesh *thisMesh, domTriangles *thisTriangles, int verbose ) {

	map<triangleStruct, bool, ltstr> triangleArray;
	map<triangleStruct, bool, ltstr>::iterator ti;
	map<triangleStruct, bool, ltstr>::iterator tti;

	// Create a new <trifans> inside the mesh that has the same material as the <triangles>
	domTrifans *thisTrifans = (domTrifans *)thisMesh->createAndPlace("trifans");
	thisTrifans->setCount( 0 );
	thisTrifans->setMaterial(thisTriangles->getMaterial());

	// Give the new <trifans> the same <input> and <parameters> as the old <triangles>
	for(int i=0; i<(int)(thisTriangles->getInput_array().getCount()); i++)
	{
		thisTrifans->placeElement( thisTriangles->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the triangles array
	domUint offsetcount = (int)getMaxOffset(thisTriangles->getInput_array())+1;

	// set P
	domListOfUInts triangleValue = thisTriangles->getP()->getValue();

	for(size_t i=0; i<thisTriangles->getCount(); i++)
	{
		triangleStruct tri;
		domUint values[3];
		values[0] = triangleValue[(size_t) (3*i*offsetcount)];
		values[1] = triangleValue[(size_t) (3*i*offsetcount + offsetcount)];
		values[2] = triangleValue[(size_t) (3*i*offsetcount + 2*offsetcount)];
		qsort (values, 3, sizeof(int), compare);
		tri.first = values[0];
		tri.second = values[1];
		tri.third = values[2];
		triangleArray.insert(pair<triangleStruct, bool>(tri, false));
	}

	// check for the possible line strips assume the duplicates have been removed
	vector<domUint> trifans;
	vector<domUint>::iterator vi;
	int trifansCount = 0;
	for (ti = triangleArray.begin(); ti != triangleArray.end(); ti++) {
		if ( (*ti).second == false) {
			trifans.push_back((*ti).first.first);
			trifans.push_back((*ti).first.second);
			trifans.push_back((*ti).first.third);
			(*ti).second = true;

			domUint temp1 = (*ti).first.first;
			domUint temp2 = (*ti).first.third;
			for (tti = triangleArray.begin(); tti != triangleArray.end(); tti++) {
				if (((*tti).second == false) && (temp1 == (*tti).first.first) && (temp2 == (*tti).first.second)) {
					trifans.push_back((*tti).first.third);
					temp2 = (*tti).first.third;
					(*tti).second = true;
				} 
			}

			// create <p> for each trifan
			domP*  trifanP = (domP*) thisTrifans->createAndPlace("p");
			for (vi = trifans.begin(); vi != trifans.end(); vi++) {
				if (offsetcount == 1)
					trifanP->getValue().append((*vi));
				else {
					if (verbose) printf("inside trifan append %d\n", (*vi));
					trifanP->getValue().append((*vi));
					for (size_t i = 0; i < thisTriangles->getCount()*offsetcount*3; i++) {
						if (triangleValue[i] == (*vi)) {
							if (verbose) printf("find match i is %d, triangleValue is %d\n", i, triangleValue[i]); 
							for (size_t j = 1; j < offsetcount; j++) {
								trifanP->getValue().append(triangleValue[i+j]);
							} // for
							break;
						} // if
					} // for
				}
			} // for
			trifansCount++;
			trifans.clear();
		} // if
	} // for

	thisTrifans->setCount(trifansCount);
}

bool Trifanner::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

int Trifanner::execute()
{
	bool verbose;
	getBoolOption("v", verbose);
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

		if (thisMesh == NULL) continue;
		// Loop over all the triangle elements
		int trianglesElementCount = (int)(thisMesh->getTriangles_array().getCount());
		//if(verbose)	cerr<<"There are "<<trianglesElementCount<<"triangle elements in this file\n"; 
		for(int currentTriangles = 0; currentTriangles < trianglesElementCount; currentTriangles++)
		{
			// Get the triangles out of the mesh
			domTriangles *thisTriangles = thisMesh->getTriangles_array().get(0);  
			createTrifansFromTriangles( thisMesh, thisTriangles, verbose );
			// Remove the triangles from the mesh
			thisMesh->removeChildElement(thisTriangles);
		}  
	} 
	return 0; 
}

Conditioner::Register<TrifannerProxy> registerObject;


