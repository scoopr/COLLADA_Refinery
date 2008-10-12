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
