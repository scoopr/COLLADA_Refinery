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
//
// herbert.law@playstation.sony.com
#ifndef _TRISTRIPPER_H_
#define _TRISTRIPPER_H_

#include <list>
#include <set>
#include <vector>
using namespace std;

#include "defines.h"
#include "conditionerBase.h"

#include "dae.h" 
#include "dom/domCOLLADA.h"

class Edge;
class Triangle;

struct InputInfo {
	domFloat_array *data;
	unsigned int stride;
};

class Vertex {
public:
	unsigned int size;
	unsigned int inputNumber;
	domFloat *values;
	int *indices;
	int newIndex;

	Vertex(unsigned int _inputNumber, unsigned int *_indices, unsigned int floatNumber, InputInfo* inputs){
		size = floatNumber;
		inputNumber = _inputNumber;
		values = new domFloat[size];
		indices = new int[inputNumber];
		memset(values, 0, size*sizeof(domFloat));
		domFloat *valuePtr = values;
		for(size_t i = 0; i < inputNumber; i++){
			indices[i] = _indices[i];
			for(size_t s = 0; s < inputs[i].stride; s++){
				*valuePtr++ = inputs[i].data->getValue().get(inputs[i].stride*indices[i] + s);
			}
		}
	}

	bool equals(const Vertex *v2) const {
		return memcmp(values, v2->values, size * sizeof(domFloat)) == 0;
	}

	bool less(const Vertex *v2) const {
		return memcmp(values, v2->values, size * sizeof(domFloat)) < 0;
	}

};

struct vertexcmp
{
  bool operator()(const Vertex* v1, const Vertex* v2) const
  {
    return v1->less(v2);
  }
};

class Edge{
public:
	Vertex *v1, *v2;
	Triangle *t1, *t2;

	Edge(Vertex *_v1, Vertex *_v2) {
		v1 = _v1; v2 = _v2;
		t1 = NULL;
		t2 = NULL;
	}

	bool equals(Edge *e2) {
		return (v1->equals(e2->v1) && v2->equals(e2->v2)) ;}

	bool containsVertex(Vertex *v){
		return (v->equals(v1) || v->equals(v2));
	}

	Vertex *getOtherVertex(Vertex *v){
		if ( v->equals(v1) ) {
			return v2;
		} else {
			return v1;
		}
	}

	void addRelativeTriangle(Triangle *t);

	Triangle *getOtherTriangle(Triangle *t) {
		return (t == t1 ? t2: t1); }

	Triangle *getOtherTriangleAndCheckOrientation(Triangle *t);

};

class Triangle{
public:
	Vertex *v1, *v2, *v3;
	Edge *e1, *e2, *e3;
	bool inStrip;
	int index;
	int unstripedNeighboursCount;

	Triangle( Edge *_e1, Edge *_e2, Edge *_e3, Vertex *_v1, Vertex *_v2, Vertex *_v3, int _index);

	bool isEdgeSameDirection(Edge *e);

	Edge *getOppositeEdge(Vertex *v){
		if ( !e1->containsVertex(v) ) return e1;
		if ( !e2->containsVertex(v) ) return e2;
		if ( !e3->containsVertex(v) ) return e3;

		return NULL;
	}

	Vertex *getOppositeVertex(Edge *e){
		if( ! e->containsVertex(v1)) { return v1; }
		if( ! e->containsVertex(v2)) { return v2; }
		if( ! e->containsVertex(v3)) { return v3; }
		return NULL;
	}

	Triangle *getOppositeNeighbor(Vertex *v){
		return getOppositeEdge(v)->getOtherTriangle(this);
	}

	int getNeighborConnectivity(Edge *destinationEdge);

	bool needSwap(Edge *destinationEdge, Vertex *firstVertex);

	bool Triangle::isSameDirection(Edge *e);

};

typedef std::list<Vertex*> Strip;


class TriStripper : public ConditionerBase
{
public:
	TriStripper(void){ uniqueVertexCount=0; triangleCount = 0; stripMaxLength = 100; }

	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "triangulate"; }
	REF_EXPORT std::string getDescription() const { return "triangulate"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }

private:
	std::set<Vertex*, vertexcmp> uniqueVertices;		// This tracks unique vertex combinations
	std::vector<Edge*> edgeList;
	std::vector<Triangle*> triangleList;

	void addTriangle(Vertex *v1, Vertex *v2, Vertex *v3);
	Strip *stripifyOnce(Triangle *startingTriangle, int StripMaxLen);
	std::vector<Strip*> *stripify(int StripMaxLen);

	Edge *findEdge(Vertex *v1, Vertex *v2);
	Edge *findOrAddEdge(Vertex *v1, Vertex *v2);
	Vertex *assignIndex(Vertex *v);

	int uniqueVertexCount;
	int triangleCount;

	int stripMaxLength;
};

class TriStripperProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "tristripper"; }
	REF_EXPORT std::string getDescription() const { return "tristripper"; }
	REF_EXPORT std::string getCategory() const  { return "geometry processing"; }
	REF_EXPORT Conditioner *create() const { return new TriStripper(); }
};

#endif //_TRISTRIPPER_H_
