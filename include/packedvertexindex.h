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
#ifndef __PACKED_VERTEX_INDEX_H__
#define __PACKED_VERTEX_INDEX_H__

#include <memory.h>

class packedVertexIndex
{
public:
	packedVertexIndex();
	packedVertexIndex(int size);
	packedVertexIndex(const packedVertexIndex& rhs);
	packedVertexIndex& operator=(const packedVertexIndex& rhs);
	virtual ~packedVertexIndex();
	
	inline bool operator==(const packedVertexIndex& rhs) const
	{return( memcmp(indices,rhs.indices, index_count*sizeof(int)) == 0);}
	inline bool operator!=(const packedVertexIndex & rhs) const
	{return( !operator==(rhs));}
	inline bool operator<(const packedVertexIndex & rhs) const
	{return( memcmp(indices,rhs.indices, index_count*sizeof(int)) < 0);}
	inline bool operator>(const packedVertexIndex & rhs) const
	{return( memcmp(indices,rhs.indices, index_count*sizeof(int)) > 0);}
	inline bool operator<=(const packedVertexIndex & rhs) const
	{return( !(*this > rhs));}
	inline bool operator>=(const packedVertexIndex & rhs) const
	{return( !(*this < rhs));}
	

	mutable int references;			// Number of times this packedVertex is used
	mutable int new_index_number;   // new index for this "de-indexed" vertex
	int index_count;		// number of indices in this packedVertex
	int* indices;			// Storage for the indices
};
#endif //__PACKED_VERTEX_INDEX_H__
