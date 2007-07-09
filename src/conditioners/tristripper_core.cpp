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
#include "tristripper.h"

#include <dae/daeDatabase.h>

void Edge::addRelativeTriangle(Triangle *t){
	if (t1 == NULL){
		t1 = t;
	} else if (t2 == NULL){
		t2 = t;
	} else {
//		cout<< "An edge has more than 2 faces linked";
	}
}

Triangle *Edge::getOtherTriangleAndCheckOrientation(Triangle *t){
	Triangle *result = getOtherTriangle(t);
	if (result == NULL) return NULL;
	if (t->isEdgeSameDirection(this) != result->isEdgeSameDirection(this) ){
		return result;
	} else {
		return NULL;
	}
}

Triangle::Triangle( Edge *_e1, Edge *_e2, Edge *_e3, Vertex *_v1, Vertex *_v2, Vertex *_v3, int _index){
		e1 = _e1; e2 = _e2; e3 = _e3;
		v1 = _v1; v2 = _v2; v3 = _v3;
		index = _index;

		e1->addRelativeTriangle(this);
		e2->addRelativeTriangle(this);
		e3->addRelativeTriangle(this);

		inStrip = false;
		unstripedNeighboursCount = 0;
		
		Triangle *neighbor;
		if( (neighbor = e1->getOtherTriangle(this)) != NULL) {
			neighbor->unstripedNeighboursCount++;
			unstripedNeighboursCount++;
		}
		if( (neighbor = e2->getOtherTriangle(this)) != NULL) {
			neighbor->unstripedNeighboursCount++;
			unstripedNeighboursCount++;
		}
		if( (neighbor = e3->getOtherTriangle(this)) != NULL) {
			neighbor->unstripedNeighboursCount++;
			unstripedNeighboursCount++;
		}


	}

bool Triangle::isEdgeSameDirection(Edge *e){
	if (e == e1) {
		return (e1->v1 == v1);
	} else if (e == e2) {
		return (e2->v1 == v2);
	} else if (e == e3) {
		return (e3->v1 == v3);
	}
	// Pb
//	cout << "isEdgeSameDirection called with a non matching edge" <<endl;
	return false;
}


Edge *TriStripper::findEdge(Vertex *v1, Vertex *v2){
	vector<Edge*>::iterator edgeIterator;
	for (edgeIterator = edgeList.begin(); edgeIterator != edgeList.end(); edgeIterator++){
		Edge* e= (*edgeIterator);
		if ( e->containsVertex(v1) && e->containsVertex(v2)){
			return e;
		}
	}

	return NULL;
}

Edge *TriStripper::findOrAddEdge(Vertex *v1, Vertex *v2){
	Edge *e = findEdge(v1, v2);
	if (e == NULL){
		e = new Edge(v1, v2);
		edgeList.push_back(e);
	}
	return e;
}

Vertex *TriStripper::assignIndex(Vertex *v){
	v->newIndex = uniqueVertexCount++;
	pair<set<Vertex*, vertexcmp>::iterator,bool> result = uniqueVertices.insert(v);
	if (!result.second){
		v->newIndex =  (*result.first)->newIndex;
		uniqueVertexCount--;
		return (*result.first);
	} else {
		//cout << "New vertex, index " << v->newIndex <<endl;
		return v;
	}
}


void TriStripper::addTriangle(Vertex *v1, Vertex *v2, Vertex *v3){
	
	// First we assign indices to these vertex, by searching if we already have them
	v1 = assignIndex(v1);
	v2 = assignIndex(v2);
	v3 = assignIndex(v3);

	// Then we find or create the egdes
	// Edge 1 is v1-v2, Edge 2 is v2-v3 and Edge 3 is v3-v1
	Edge *e1 = findOrAddEdge(v1, v2);
	Edge *e2 = findOrAddEdge(v2, v3);
	Edge *e3 = findOrAddEdge(v3, v1);

	Triangle *t = new Triangle(e1, e2, e3, v1, v2, v3, triangleCount++);
	triangleList.push_back(t);

}

int Triangle::getNeighborConnectivity(Edge *destinationEdge){
	Triangle *neighbor = destinationEdge->getOtherTriangle(this);
	if (neighbor == NULL) return 999; // 999 is not a normally reachable weight

	return neighbor->unstripedNeighboursCount;
}

bool Triangle::needSwap(Edge *destinationEdge, Vertex *firstVertex){
	return (destinationEdge->containsVertex(firstVertex));
}

bool Triangle::isSameDirection(Edge *e){
	// Find a common vertex
	if (e->v1 == v1){
		return e->v2 == v2;
	} else if (e->v1 == v2) {
		return e->v2 == v3;
	} else { // We suppose this is well v3, if not it meens e is not an edge from this triangle
		return (e->v2 == v1);
	}
}

Strip *TriStripper::stripifyOnce(Triangle *startingTriangle, int StripMaxLen){
	Strip *strip = new Strip;

	Triangle *currentTriangle = startingTriangle;
	Vertex *firstVertex = currentTriangle->v1;
	Vertex *secondVertex = currentTriangle->v2;
	strip->push_back(currentTriangle->v1);
	strip->push_back(currentTriangle->v2);
	Edge *lastEdge = currentTriangle->getOppositeEdge(currentTriangle->v3);
	long stripLength = 0;

	do {
		currentTriangle->inStrip = true; // should update neighbors values
		Edge *firstChoice = currentTriangle->getOppositeEdge(secondVertex);
		Edge *secondChoice = currentTriangle->getOppositeEdge(firstVertex);

		//Some debug
		//cout << "Current triangle :" << currentTriangle->v1->newIndex << " " << currentTriangle->v2->newIndex << " " << currentTriangle->v3->newIndex << endl;

		// We choose first and second edge so that the first will always need a first type swap and the second never.
		// so we can add the weight of swap right now.
		int firstChoiceWeight = 1;
		int secondChoiceWeight = -1;
		bool faceOrientationSwap1 = false;
		bool faceOrientationSwap2 = false;

		if (firstChoice == 0 || secondChoice == 0) //TODO: it crashes in some document when they are NULL
		{
			if (lastEdge == 0) break;  //TODO: it crashes in some document when they are NULL
			continue;								
		}
		Triangle *neighbor1 = firstChoice->getOtherTriangle(currentTriangle);
		Triangle *neighbor2 = secondChoice->getOtherTriangle(currentTriangle);

		if(neighbor1 && neighbor1->inStrip){
			firstChoiceWeight += 1000;
		}
		if(neighbor2 && neighbor2->inStrip){
			secondChoiceWeight += 1000;
		}
		// Add weights from neighbor triangle connectivity
		firstChoiceWeight += currentTriangle->getNeighborConnectivity(firstChoice);
		secondChoiceWeight += currentTriangle->getNeighborConnectivity(secondChoice);

		// Add weight from possible swaps (from face orientations)
		if (neighbor1){
            
			if( faceOrientationSwap1 = (currentTriangle->isSameDirection(firstChoice) == neighbor1->isSameDirection(firstChoice))){
				firstChoiceWeight++;
			}
		}
		if (neighbor2){
			
			if( faceOrientationSwap2 = (currentTriangle->isSameDirection(secondChoice) == neighbor2->isSameDirection(secondChoice))){
				secondChoiceWeight++;
			}
		}

		Vertex *newVertex = currentTriangle->getOppositeVertex(lastEdge);

		//cout << "Scores : " << firstChoiceWeight << " ; " << secondChoiceWeight << endl;

		if (firstChoiceWeight < secondChoiceWeight) {
			if (faceOrientationSwap1){
				strip->push_back(newVertex);
				strip->push_back(newVertex);
				strip->push_back(firstVertex);
				secondVertex = firstVertex;
				firstVertex = newVertex;
				//cout << "First Choice, plus swap" <<endl;

			} else {
				strip->push_back(firstVertex);
				strip->push_back(newVertex);
				secondVertex = newVertex;
				//cout << "First Choice, no swap" <<endl;
			}

			lastEdge = firstChoice;
			

		} else { // We choose second edge
			if (faceOrientationSwap2){
				strip->push_back(newVertex);
				strip->push_back(newVertex);
				strip->push_back(secondVertex);
				firstVertex = newVertex;
				//cout << "Second Choice, plus swap" <<endl;
			} else {
				strip->push_back(newVertex);
				firstVertex = secondVertex;
				secondVertex = newVertex;
				//cout << "Second Choice, no swap" <<endl;
			}

			lastEdge = secondChoice;
			stripLength++;
		}

		// Update the unstripedNeighboursCount info in neighbors
		Triangle *neighbor;
		if( (neighbor = currentTriangle->e1->getOtherTriangle(currentTriangle)) != NULL) {
			neighbor->unstripedNeighboursCount++;
		}
		if( (neighbor = currentTriangle->e2->getOtherTriangle(currentTriangle)) != NULL) {
			neighbor->unstripedNeighboursCount++;
		}
		if( (neighbor = currentTriangle->e3->getOtherTriangle(currentTriangle)) != NULL) {
			neighbor->unstripedNeighboursCount++;
		}

	} while (( currentTriangle = lastEdge->getOtherTriangle(currentTriangle) ) != NULL && (!currentTriangle->inStrip) && stripLength < StripMaxLen);

	list<Vertex*>::iterator stripIterator;
	/*
	cout << "Strip :";
	for(stripIterator = strip->begin(); stripIterator != strip->end(); stripIterator++){
		cout << " " << (*stripIterator)->newIndex;
	}
	cout <<endl;
	*/

	return strip;

}

vector<Strip*> *TriStripper::stripify(int StripMaxLen){
	vector<Strip*> *result = new vector<Strip *>();

	std::vector<Triangle*>::iterator triangleListIterator;
	for (triangleListIterator = triangleList.begin(); triangleListIterator != triangleList.end(); triangleListIterator++){
		Triangle *startingTriangle = (*triangleListIterator);
		if (startingTriangle->inStrip) continue;
		result->push_back(stripifyOnce(startingTriangle, StripMaxLen));
	}
	return result;
}

bool TriStripper::init()
{
	return true;
}

int TriStripper::execute()
{
	int error = 0;
	std::string doc = getInput( 0 );
	// How many geometry assets are there?
	unsigned int geometryElementCount = _dae->getDatabase()->getElementCount(NULL, "geometry", doc.c_str());

	// Iterate over all the geometry assets
	for(unsigned int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++) {
		// Get the next geometry element
		domGeometry *thisGeometry;
		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", doc.c_str());
		if (error != DAE_OK) {
			return(-1);
		}

		// Get the geometry element's mesh
		domMesh *thisMesh = thisGeometry->getMesh();

		if (thisMesh==NULL) continue; // if there are no mesh in this geometry, skip to next one

		// Loop over all the triangle assets in this mesh
		int trianglesAssetCount = (int) thisMesh->getTriangles_array().getCount();
		for(int currentTriangles = 0; currentTriangles < trianglesAssetCount; currentTriangles++)
		{
			TriStripper triStripper;
			unsigned int storageSize = 0;

			// Get the next triangles out of the mesh
			// Always get index 0 because every pass through this loop deletes the <triangles> element as it finishes with it
			domTriangles *thisTriangles = thisMesh->getTriangles_array().get(0);

			domTristrips *thisTristrips = (domTristrips *)thisMesh->createAndPlace("tristrips");

			// Assume that all the <triangles> have the same inputs in the same order, so find all
			// the input sources for the first <triangles> and make copies of them.

			// find the max input offset, 0 start
			int inputCount = (int)(thisTriangles->getInput_array().getCount());
			unsigned int maxinputoffset		= 0;
			for (int i=0; i < inputCount; i++)
			{
				unsigned int thisinputoffset = (unsigned int) thisTriangles->getInput_array()[i]->getOffset();
				if (maxinputoffset < thisinputoffset) maxinputoffset = thisinputoffset;
			}
			maxinputoffset++; // make it 1 start


			InputInfo *inputs = new InputInfo[maxinputoffset];
			unsigned int *tmpIndices = new unsigned int[maxinputoffset];

			for(unsigned int inputNumber=0; inputNumber < maxinputoffset; inputNumber++)
			{
				domSource *thisSource;

				// Find the source for thisInput
				domInputLocalOffset_Array &thisInputArray = thisMesh->getTriangles_array().get(0)->getInput_array();
				domInputLocalOffsetRef thisInputRef = thisInputArray[inputNumber];

				thisTristrips->placeElement(thisInputRef->clone());

				thisInputRef->getSource().resolveElement();

				if(strcmp(thisInputRef->getSemantic(),"VERTEX")==0)
				{
					// The source is indirected through a vertex
					daeElement * thisElement = thisInputRef->getSource().getElement();
					domVertices * thisVertices = (domVertices*)thisElement; 
					thisVertices->getInput_array().get(0)->getSource().resolveElement();
					thisElement = thisVertices->getInput_array().get(0)->getSource().getElement();
					thisSource = (domSource *)thisElement;
				}
				else
				{
					// We are referencing the source directly
					daeElement * thisElement = thisInputRef->getSource().getElement();
					thisSource = (domSource *)thisElement;
				}
				// Now get the float array (only read the first one)
				domFloat_array * thisArray = thisSource->getFloat_array();

				domSource::domTechnique_commonRef pTechnique_common = thisSource->getTechnique_common();
				domAccessorRef pAccessor = pTechnique_common->getAccessor();
				inputs[inputNumber].stride = (unsigned int) pAccessor->getStride();

				inputs[inputNumber].data = thisArray;
				storageSize += inputs[inputNumber].stride;
			}

			// Create a new <tristrips> inside the mesh that has the same material as the <triangles>
			thisTristrips->setCount( 0 );
			thisTristrips->setMaterial(thisTriangles->getMaterial());  // can't use = to copy URLs

			for(int i=0; i<(int)(thisTriangles->getExtra_array().getCount()); i++)
			{
				thisTristrips->placeElement( thisTriangles->getExtra_array()[i]->clone() );
			}

			domListOfUInts pValues = thisTriangles->getP()->getValue();

			// There is only one P for triangles
			unsigned int indexCount = (unsigned int) pValues.getCount();
			unsigned int currentIndex = 0;

			while(currentIndex < indexCount)
			{
				//int currentVertexIndex = thisPrimitive->getValue()[currentIndex++];
				for(unsigned int i=0; i<maxinputoffset; i++){
					tmpIndices[i] = (unsigned int) pValues[currentIndex++];
				}
				Vertex *v1 = new Vertex(maxinputoffset, tmpIndices, storageSize, inputs);

				for(unsigned int i=0; i<maxinputoffset; i++){
					tmpIndices[i] = (unsigned int) pValues[currentIndex++];
				}
				Vertex *v2 = new Vertex(maxinputoffset, tmpIndices, storageSize, inputs);

				for(unsigned int i=0; i<maxinputoffset; i++){
					tmpIndices[i] = (unsigned int) pValues[currentIndex++];
				}
				Vertex *v3 = new Vertex(maxinputoffset, tmpIndices, storageSize, inputs);

				triStripper.addTriangle(v1, v2, v3);
			}

			vector<Strip*> *strip = triStripper.stripify(stripMaxLength);
			vector<Strip*>::iterator stripIterator;
			long tristripCount = 0;
			for(stripIterator = strip->begin(); stripIterator != strip->end(); stripIterator++){
				domP* p_tristrips = (domP*)thisTristrips->createAndPlace("p");

				list<Vertex*>::iterator vertexIterator;
				for(vertexIterator = (*stripIterator)->begin(); vertexIterator != (*stripIterator)->end(); vertexIterator++){
					Vertex *v = (*vertexIterator);
					for(unsigned int i=0; i<v->inputNumber; i++){
						p_tristrips->getValue().append(v->indices[i]);
					}
				}
				tristripCount ++;
			}
			thisTristrips->setCount(tristripCount );

			thisMesh->removeChildElement(thisTriangles);
			delete tmpIndices;
		} //Triangles

	} // geometry
	
	return 0;	
}

Conditioner::Register< TriStripperProxy > triangulateProxy;
