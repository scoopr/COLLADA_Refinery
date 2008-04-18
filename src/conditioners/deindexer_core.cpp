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
#include "deindexer.h"
#include "utility.h"
#include "dom/domConstants.h"

class SkinInfo
{
public:
	SkinInfo(domSkin::domVertex_weights * thisWeights, domUint inputnumbers);
	~SkinInfo();
	void AppendIndex(domUint i);
	void SetWeightsCount(domUint count) {weights->setCount(count);};
private:
	domUint vcountnumber;
	domUint * vcount;	// numbers of int for each vertices * numbers of inputs
	domUint ** v;		// the numbers
	domSkin::domVertex_weights::domVcountRef weightvcount;
	domSkin::domVertex_weights::domVRef weightv;
	domUint input_numbers;
	domSkin::domVertex_weights * weights;
};

SkinInfo::SkinInfo(domSkin::domVertex_weights * thisWeights, domUint inputnumbers)
{
	input_numbers = inputnumbers;
	weights = thisWeights;
	weightvcount = thisWeights->getVcount();
	weightv = thisWeights->getV();

	domListOfUInts vcountarray = weightvcount->getValue();
	domListOfInts varray = weightv->getValue();
	vcountnumber = vcountarray.getCount();
    domUint vnumber = varray.getCount();
	vcount = new domUint[(size_t) vcountnumber];
	v = new domUint*[(size_t)vcountnumber];
	size_t vindex = 0;
	for (size_t i=0; i<vcountnumber; i++)
	{
		vcount[i] = vcountarray[i];
		v[i] = new domUint[(size_t)(vcount[i] * inputnumbers)];
		for (domUint j=0; j< vcount[i] * inputnumbers; j++)
		{
			v[i][j] = varray[vindex];
			vindex ++;
		}
	}
	assert(vindex == vnumber);
	weightvcount->getValue().clear();
	weightv->getValue().clear();
}
SkinInfo::~SkinInfo()
{
	for (domUint i=0; i<vcountnumber; i++)
	{
		delete v[i];
	}
	delete v;
	delete vcount;
}
void SkinInfo::AppendIndex(domUint i)
{
	weightvcount->getValue().append(vcount[i]);
	for (domUint j=0; j< vcount[i] * input_numbers; j++)
	{
		weightv->getValue().append(v[i][j]);
	}
}
class VertexIndexes;
struct LessVertexIndexes;
typedef std::map<domElement*, domUint> IndexMap;
typedef std::set<VertexIndexes*, LessVertexIndexes> VertexSet;
class VertexIndexes
{
public:
	VertexIndexes();
	~VertexIndexes();
	void SetIndex(domElement * source, domUint index_value);
	IndexMap index_map;
	domUint new_index;
};
VertexIndexes::VertexIndexes()
{
	new_index = 0;
}
VertexIndexes::~VertexIndexes()
{
	index_map.clear();
}
struct LessVertexIndexes
{
	bool operator()(VertexIndexes * s1, VertexIndexes * s2)
	{
		IndexMap::iterator i;
		IndexMap::iterator j;
		for (i=s1->index_map.begin(); i!=s1->index_map.end(); i++)
		{
			for (j=s2->index_map.begin(); j!=s2->index_map.end(); j++)
			{
				if (i->first == j->first)
				{
					if (i->second == j->second)
						continue;
					else 
						return i->second < j->second;
				}
			}
		}
		return false;
	}
};

struct LessInNewIndex
{
	bool operator()(VertexIndexes * s1, VertexIndexes * s2)
	{
		return s1->new_index < s2->new_index;
	}
};


void VertexIndexes::SetIndex(domElement * source, domUint index_value)
{
	IndexMap::iterator i;
	for (i=index_map.begin(); i!=index_map.end(); i++)
	{
		if (i->first == source)
		{
			i->second = index_value;
			return;
		}
	}
	index_map[source] = index_value;
}


void MergeVertex(VertexIndexes * match_vertex, VertexIndexes *vertex)
{
	IndexMap::iterator srciter;
	for(srciter=vertex->index_map.begin(); srciter!=vertex->index_map.end(); srciter++)
	{
		match_vertex->index_map[srciter->first] = srciter->second;
	}
	delete vertex;
}


void ParseInputAndP(domInputLocalOffset_Array &input_array, domP * p, domPRef &newp, VertexSet & vertexset, domUint & vertexindex, domUint &maxoffset)
{
	std::map<domElement*, domUint> offsetmap;
	// set offsetmap<source_element_ptr, offset_number>
	for (size_t j=0; j<input_array.getCount(); j++)
	{
		xsAnyURI & uri = input_array[j]->getSource();
		uri.resolveElement();
		domElement * element = uri.getElement();
		offsetmap[element] = input_array[j]->getOffset();
//		input_array[j]->setOffset(0);			// reset all input's offset to 0 since all inputs use the same vertex index
	}
	domListOfUInts & value = p->getValue();

	for (size_t j=0; j<value.getCount(); j=(size_t) (j+maxoffset+1))
	{
		VertexIndexes* vertex = new VertexIndexes();

		std::map<domElement*, domUint>::iterator iter;
		for (iter=offsetmap.begin(); iter!=offsetmap.end(); iter++)
		{
			domUint index = value[(size_t) (j+(iter->second))];
			vertex->SetIndex(iter->first, index);
		}
		pair<VertexSet::iterator,bool> result = vertexset.insert(vertex);
		if (result.second)
		{	// it is unique, not duplicate in set
//			printf("vertex is unique\n");
			vertex->new_index = vertexindex;
			newp->getValue().append(vertexindex);
			vertexindex++;
		} else
		{	// it is not unique, duplicate found in set
//			printf("vertex is not unique\n"); 
			VertexIndexes * match_vertex = *(result.first);
			newp->getValue().append(match_vertex->new_index);
			MergeVertex(match_vertex, vertex);
		}
	}
}

void ResetSource(domSource * newsource, domUint new_vertex_size)
{
	newsource->getTechnique_common()->getAccessor()->setCount(new_vertex_size);
	domUint stride = newsource->getTechnique_common()->getAccessor()->getStride();

	domFloat_arrayRef newfloat_array = newsource->getFloat_array();
	if (newfloat_array)
	{
		newfloat_array->setCount(new_vertex_size * stride);
		newfloat_array->getValue().setCount((size_t)(new_vertex_size * stride));
	}

	domInt_arrayRef newint_array = newsource->getInt_array();
	if (newint_array)
	{
		newint_array->setCount(new_vertex_size * stride);
		newint_array->getValue().setCount((size_t)(new_vertex_size * stride));
	}

	domName_arrayRef newname_array = newsource->getName_array();
	if (newname_array)
	{
		newname_array->setCount(new_vertex_size * stride);
		newname_array->getValue().setCount((size_t)(new_vertex_size * stride));
	}

	domIDREF_arrayRef newidref_array = newsource->getIDREF_array();
	if (newidref_array)
	{
		newidref_array->setCount(new_vertex_size * stride);
		newidref_array->getValue().setCount((size_t)(new_vertex_size * stride));
	}

	domBool_arrayRef newbool_array = newsource->getBool_array();
	if (newbool_array)
	{
		newbool_array->setCount(new_vertex_size * stride);
		newbool_array->getValue().setCount((size_t)(new_vertex_size * stride));
	}
}

void SetSource(domSource * oldsource, domSource * newsource, domUint oldindex, domUint newindex)
{
	domUint stride = oldsource->getTechnique_common()->getAccessor()->getStride();

	domFloat_arrayRef oldfloat_array = oldsource->getFloat_array();
	if (oldfloat_array)
	{
		domFloat_arrayRef newfloat_array = newsource->getFloat_array();
		for (size_t i=0; i<stride; i++)
		{
			newfloat_array->getValue().set((size_t)(newindex*stride+i), oldfloat_array->getValue()[(size_t)(oldindex*stride+i)]);
		}
	}

	domInt_arrayRef oldint_array = oldsource->getInt_array();
	if (oldint_array)
	{
		domInt_arrayRef newint_array = newsource->getInt_array();
		for (size_t i=0; i<stride; i++)
		{
			newint_array->getValue().set((size_t)(newindex*stride+i), oldint_array->getValue()[(size_t)(oldindex*stride+i)]);
		}
	}

	domName_arrayRef oldname_array = oldsource->getName_array();
	if (oldname_array)
	{
		domName_arrayRef newname_array = newsource->getName_array();
		for (size_t i=0; i<stride; i++)
		{
			newname_array->getValue().set((size_t)newindex, oldname_array->getValue()[(size_t)oldindex]);
		}
	}

	domIDREF_arrayRef oldidref_array = oldsource->getIDREF_array();
	if (oldidref_array)
	{
		domIDREF_arrayRef newidref_array = newsource->getIDREF_array();
		for (size_t i=0; i<stride; i++)
		{
			newidref_array->getValue().set((size_t)newindex, oldidref_array->getValue()[(size_t)oldindex]);
		}
	}

	domBool_arrayRef oldbool_array = oldsource->getBool_array();
	if (oldbool_array)
	{
		domBool_arrayRef newbool_array = newsource->getBool_array();
		for (size_t i=0; i<stride; i++)
		{
			newbool_array->getValue().set((size_t)newindex, oldbool_array->getValue()[(size_t)oldindex]);
		}
	}
}

bool Deindexer::init()
{
	return true;
}

void SetAllInputToOffsetZero( domInputLocalOffset_Array &input_array )
{
	for ( unsigned int i = 0; i < input_array.getCount(); i++ ) {
		input_array[i]->setOffset(0);
	}	
}

int Deindexer::execute()
{
	domInt error = 0;
	domUint geometry_count = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_GEOMETRY, getInput(0).c_str());

	for (daeUInt c=0; c<geometry_count; c++)
	{	// for each geometry
		domGeometry *geometry;
		error = _dae->getDatabase()->getElement((daeElement**)&geometry,c, NULL, COLLADA_TYPE_GEOMETRY, getInput(0).c_str());

		domMesh *mesh = geometry->getMesh();
		if (mesh==NULL) continue;

		VertexSet vertexset;				// the set
		domUint vertexindex = 0;			// the next new index
//		domUint IndexMaxOffset = 0;

		domTriangles_Array & triangles_array = mesh->getTriangles_array();
		for (size_t i=0; i<triangles_array.getCount(); i++)
		{
			domTriangles * triangles = triangles_array[i];
			domInputLocalOffset_Array & input_array = triangles->getInput_array();
			domUint maxoffset = getMaxOffset(input_array);

			domP * p = triangles->getP();
			domPRef newp = (domP*) (domElement*) p->createElement("p");
			ParseInputAndP(input_array, p, newp, vertexset, vertexindex, maxoffset);

			// remove old p and add new p
			triangles->removeChildElement(p);
			triangles->placeElement(newp);
			SetAllInputToOffsetZero(input_array);
		}

		domTristrips_Array & tristrips_array = mesh->getTristrips_array();
		for (size_t i=0; i<tristrips_array.getCount(); i++)
		{
			domTristrips * tristrips = tristrips_array[i];
			domInputLocalOffset_Array & input_array = tristrips->getInput_array();
			domUint maxoffset = getMaxOffset(input_array);

			domP_Array & p_array = tristrips->getP_array();
			size_t p_array_count = p_array.getCount();
			for (size_t j=0; j<p_array_count; j++)
			{
				domP * p = p_array[0];
				domPRef newp = (domP*) (domElement*) p->createElement("p");
				ParseInputAndP(input_array, p, newp, vertexset, vertexindex, maxoffset);
				// remove old p and add new p
				tristrips->placeElement(newp);
				tristrips->removeChildElement(p);
			}
			SetAllInputToOffsetZero(input_array);
		}

		domTrifans_Array & trifans_array = mesh->getTrifans_array();
		for (size_t i=0; i<trifans_array.getCount(); i++)
		{
			domTrifans * trifans = trifans_array[i];
			domInputLocalOffset_Array & input_array = trifans->getInput_array();
			domUint maxoffset = getMaxOffset(input_array);

			domP_Array & p_array = trifans->getP_array();
			size_t p_array_count = p_array.getCount();
			for (size_t j=0; j<p_array_count; j++)
			{
				domP * p = p_array[0];
				domPRef newp = (domP*) (domElement*) p->createElement("p");
				ParseInputAndP(input_array, p, newp, vertexset, vertexindex, maxoffset);
				// remove old p and add new p
				trifans->placeElement(newp);
				trifans->removeChildElement(p);
			}
			SetAllInputToOffsetZero(input_array);
		}

		domPolygons_Array & polygons_array = mesh->getPolygons_array();
		for (size_t i=0; i<polygons_array.getCount(); i++)
		{
			domPolygons * polygons = polygons_array[i];
			domInputLocalOffset_Array & input_array = polygons->getInput_array();
			domUint maxoffset = getMaxOffset(input_array);

			domP_Array & p_array = polygons->getP_array();
			size_t p_array_count = p_array.getCount();
			for (size_t j=0; j<p_array_count; j++)
			{
				domP * p = p_array[0];
				domPRef newp = (domP*) (domElement*) p->createElement("p");
				ParseInputAndP(input_array, p, newp, vertexset, vertexindex, maxoffset);
				// remove old p and add new p
				polygons->placeElement(newp);
				polygons->removeChildElement(p);
			}
			SetAllInputToOffsetZero(input_array);
		}

		domPolylist_Array & polylist_array = mesh->getPolylist_array();
		for (size_t i=0; i<polylist_array.getCount(); i++)
		{
			domPolylist * polylist = polylist_array[i];
			domInputLocalOffset_Array & input_array = polylist->getInput_array();
			domUint maxoffset = getMaxOffset(input_array);

			domP * p = polylist->getP();
			domPRef newp = (domP*) (domElement*) p->createElement("p");
			ParseInputAndP(input_array, p, newp, vertexset, vertexindex, maxoffset);

			// remove old p and add new p
			polylist->removeChildElement(p);
			polylist->placeElement(newp);
			SetAllInputToOffsetZero(input_array);
		}

		domLines_Array & lines_array = mesh->getLines_array();
		for (size_t i=0; i<lines_array.getCount(); i++)
		{
			domLines * lines = lines_array[i];
			domInputLocalOffset_Array & input_array = lines->getInput_array();
			domUint maxoffset = getMaxOffset(input_array);

			domP * p = lines->getP();
			domPRef newp = (domP*) (domElement*) p->createElement("p");
			ParseInputAndP(input_array, p, newp, vertexset, vertexindex, maxoffset);

			// remove old p and add new p
			lines->removeChildElement(p);
			lines->placeElement(newp);
			SetAllInputToOffsetZero(input_array);
		}

		domLinestrips_Array & linestrips_array = mesh->getLinestrips_array();
		for (size_t i=0; i<linestrips_array.getCount(); i++)
		{
			domLinestrips * linestrips = linestrips_array[i];
			domInputLocalOffset_Array & input_array = linestrips->getInput_array();
			domUint maxoffset = getMaxOffset(input_array);

			domP_Array & p_array = linestrips->getP_array();
			size_t p_array_count = p_array.getCount();
			for (size_t j=0; j<p_array_count; j++)
			{
				domP * p = p_array[0];
				domPRef newp = (domP*) (domElement*) p->createElement("p");
				assert(newp);
				ParseInputAndP(input_array, p, newp, vertexset, vertexindex, maxoffset);
				// remove old p and add new p

				linestrips->placeElement(newp);
				linestrips->removeChildElement(p);
			}
			SetAllInputToOffsetZero(input_array);
		}

		domVertices * vertices = mesh->getVertices();
		if (vertices == 0)
		{
			printf("<vertices> not found int mesh, please run coherencytest\n");
			continue;
		}

		// setup map for new sources
		std::map<domSource*,domSource*> source_map;
		domSource_Array & source_array = mesh->getSource_array();
		domUint source_count = source_array.getCount();
		for(size_t i=0; i<source_count; i++)
		{	//make a map of old source and new duplicat source
			domSource* source = (domSource*) (domElement*) source_array[i];
			domSourceRef newsource = (domSource*) (domElement*) source->clone();
			assert(newsource);
			ResetSource(newsource, vertexindex);
			mesh->placeElement(newsource);
			source_map[source] = newsource;
		}

		// sort vertexset by new_index
		std::set<VertexIndexes*, LessInNewIndex> sortedvertexset;
		VertexSet::iterator setiter;
		for(setiter=vertexset.begin(); setiter!=vertexset.end(); setiter++)
		{
			sortedvertexset.insert((*setiter));
		}

		// rearrange all source data 
		std::set<VertexIndexes*, LessInNewIndex>::iterator sortedsetiter;
		for(sortedsetiter=sortedvertexset.begin(); sortedsetiter!=sortedvertexset.end(); sortedsetiter++)
		{
			VertexIndexes * index = (*sortedsetiter);
			IndexMap::iterator mapiter;
			for(mapiter=index->index_map.begin(); mapiter!=index->index_map.end(); mapiter++)
			{
				domElement * inputsource = mapiter->first;
				if (inputsource == vertices)
				{	// this inputsource is vertices, we need to rearrage all input sources in vertices
//					SetNewSource(index->new_index, 
					domInputLocal_Array & vertices_input_array = vertices->getInput_array();
					for(size_t i=0; i<vertices_input_array.getCount(); i++)
					{
						xsAnyURI & uri = vertices_input_array[i]->getSource();
						uri.resolveElement();
						domSource* source = (domSource*) (domElement*) uri.getElement();
						assert(source_map[source]);
						SetSource(source, source_map[source], mapiter->second, index->new_index);
					}
				} else
				{	// it is not vertices, some other source					
					domSource* source = (domSource*) inputsource;
					assert(source_map[source]);
					SetSource(source, source_map[source], mapiter->second, index->new_index);
				}
			}
		}

		// remove old sources
		std::map<domSource*,domSource*>::iterator iter;
		for(iter=source_map.begin(); iter!=source_map.end(); iter++)
		{
			domSource * source = iter->first;
			mesh->removeChildElement(source);
		}

		// parse skinning
		SkinInfo * skininfo = NULL;
		domUint skin_count = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_SKIN, getInput(0).c_str());
		for (daeUInt i=0; i < skin_count; i++)
		{
			domSkin * skin = 0;
			error = _dae->getDatabase()->getElement((daeElement**)&skin, i, NULL, COLLADA_TYPE_SKIN, getInput(0).c_str());

			xsAnyURI & uri = skin->getSource();
			uri.resolveElement();
			domGeometry * skin_geometry = (domGeometry*) (domElement*) uri.getElement();
			if (geometry == skin_geometry)
			{						// found match mesh, process skin
				domSkin::domVertex_weights * weights = skin->getVertex_weights();
//				domUint maxoffset = 0;


				domInputLocalOffset_Array &inputarray = weights->getInput_array();
/*				for (size_t j=0; j< inputarray.getCount(); j++)
				{
					domUint tmpoffset = inputarray[j]->getOffset();
					if (tmpoffset > maxoffset) maxoffset = tmpoffset;
				}
				domUint numbersofinput = maxoffset + 1; // offset is 0 start, not 1 start
*/
				domUint numbersofinput = getMaxOffset(inputarray) + 1;

				skininfo = new SkinInfo(weights, numbersofinput);
				break;
			}
		}

		if (skininfo==NULL)
		{
			for(sortedsetiter = sortedvertexset.begin(); sortedsetiter != sortedvertexset.end(); sortedsetiter++)
			{
				delete *sortedsetiter;
			}
			continue;
		}

		for (sortedsetiter=sortedvertexset.begin();sortedsetiter!=sortedvertexset.end();sortedsetiter++)
		{
			VertexIndexes * vertex_index = *sortedsetiter;
			domUint oldindex = vertex_index->index_map[vertices];
			skininfo->AppendIndex(oldindex);
			delete vertex_index;
		}
		sortedvertexset.clear();
		skininfo->SetWeightsCount(vertexindex);
		delete skininfo;
	}

	// resolve all a the inputs so that they will reference to the new domSource
	domUint input_count = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_INPUTLOCAL, getInput(0).c_str());
	for (daeUInt c=0; c<input_count; c++)
	{	// for each geometry
		domInputLocal *inputlocal;
		error = _dae->getDatabase()->getElement((daeElement**)&inputlocal,c, NULL, COLLADA_TYPE_INPUTLOCAL, getInput(0).c_str());
		inputlocal->getSource().resolveElement();
	}
	domUint inputlocaloffset_count = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_INPUTLOCALOFFSET, getInput(0).c_str());
	for (daeUInt c=0; c<inputlocaloffset_count; c++)
	{	// for each geometry
		domInputLocalOffset *inputlocaloffset;
		error = _dae->getDatabase()->getElement((daeElement**)&inputlocaloffset,c, NULL, COLLADA_TYPE_INPUTLOCALOFFSET, getInput(0).c_str());
		inputlocaloffset->getSource().resolveElement();
	}

	return 0;
}

Conditioner::Register< DeindexerProxy > deindexerProxy;
