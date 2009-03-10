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
#include "compresstransform.h"
#include <dae.h>
#include <dae/daeDatabase.h>

#include "Crt/CrtTypes.h"
#include "Crt/CrtMatrix.h"

int CompressTransforms(DAE * _dae)
{
	int error = 0;

	// How many geometry elements are there?
	int count = (int)(_dae->getDatabase()->getElementCount(NULL, "node" ));

	for(int i = 0; i < count; i++)
	{
		// Find the next geometry element
		domNode *node;
		daeElement * element = 0;
		error = _dae->getDatabase()->getElement(&element,i, NULL, "node");
		node = (domNode *) element;

		daeElementRefArray children = node->getContents();

		CrtMatrix		LocalMatrix;
		CrtMatrixLoadIdentity( LocalMatrix ); 

		for (size_t j=0; j< (size_t) children.getCount(); j++)
		{
			domElement * element = children[j];

			COLLADA_TYPE::TypeEnum element_type = element->getElementType();
			switch(element_type)
			{
			case COLLADA_TYPE::TRANSLATE:
				{
					domTranslate * translate = (domTranslate * ) element;
					domFloat3 value = translate->getValue();
					CrtVec4f vec((float)value[0], (float)value[1], (float)value[2], 0);
					CrtMatrix4x4Translate( LocalMatrix, vec);									
				}
				break;
			case COLLADA_TYPE::ROTATE:
				{
					domRotate * rotate = (domRotate * ) element;
					domFloat4 value = rotate->getValue();
					CrtVec4f vec((float)value[0], (float)value[1], (float)value[2], (float)value[3]);
					CrtMatrix4x4RotateAngleAxis( LocalMatrix, vec);									
				}
				break;
			case COLLADA_TYPE::SCALE:
				{
					domScale * scale = (domScale * ) element;
					domFloat3 value = scale->getValue();
					CrtVec4f vec((float)value[0], (float)value[1], (float)value[2], 0);
					CrtMatrix4x4Scale( LocalMatrix, vec);									
				}
				break;
/*			case COLLADA_TYPE::MATRIX:
				{
					domMatrix * mat = (domMatrix * ) element;
					domFloat4x4 value = mat->getValue();
					CrtMatrix m((float)value[0], (float)value[1], (float)value[2], (float)value[3], 
								(float)value[4], (float)value[5], (float)value[6], (float)value[7], 
								(float)value[8], (float)value[9], (float)value[10], (float)value[11], 
								(float)value[12], (float)value[13], (float)value[14], (float)value[15]); 
					CrtMatrix4x4Mult( LocalMatrix, m);									
				}
				break;
*/			default:
				break;
			}
		}

		for (size_t j=0; j< (size_t) children.getCount(); j++)
		{
			domElement * element = children[j];

			COLLADA_TYPE::TypeEnum element_type = element->getElementType();
			switch(element_type)
			{
			case COLLADA_TYPE::TRANSLATE:
				break;
			case COLLADA_TYPE::ROTATE:
				break;
			case COLLADA_TYPE::SCALE:
				break;
			case COLLADA_TYPE::MATRIX:
				break;
			default:
				continue;
			}
			node->removeChildElement(element);
		}

		CrtMatrixTranspose(LocalMatrix, LocalMatrix);

		domMatrix * matrix = (domMatrix *) node->add("matrix");
		domFloat4x4 list;
		list.setCount(16);
		list[0] = LocalMatrix[0];
		list[1] = LocalMatrix[1];
		list[2] = LocalMatrix[2];
		list[3] = LocalMatrix[3];
		list[4] = LocalMatrix[4];
		list[5] = LocalMatrix[5];
		list[6] = LocalMatrix[6];
		list[7] = LocalMatrix[7];
		list[8] = LocalMatrix[8];
		list[9] = LocalMatrix[9];
		list[10] = LocalMatrix[10];
		list[11] = LocalMatrix[11];
		list[12] = LocalMatrix[12];
		list[13] = LocalMatrix[13];
		list[14] = LocalMatrix[14];
		list[15] = LocalMatrix[15];

		matrix->setValue(list);
	}

	return 0;
}

void CrtQuaternionToMatrix(CrtVec4f * LQ, CrtFloat * LMatrix)
{
	CrtFloat	LS, LXS, LYS, LZS, LWX, LWY, LWZ, LXX, LXY, LXZ, LYY, LYZ, LZZ;

	LS = 2.0f/(LQ->x*LQ->x+LQ->y*LQ->y+LQ->z*LQ->z+LQ->w*LQ->w);

	LXS = LQ->x*LS;LYS = LQ->y*LS;LZS = LQ->z*LS;
	LWX = LQ->w*LXS;LWY = LQ->w*LYS;LWZ = LQ->w*LZS;
	LXX = LQ->x*LXS;LXY = LQ->x*LYS;LXZ = LQ->x*LZS;
	LYY = LQ->y*LYS;LYZ = LQ->y*LZS;LZZ = LQ->z*LZS;

	LMatrix[M00] = 1.0f-(LYY+LZZ);
	LMatrix[M01] = LXY+LWZ;
	LMatrix[M02] = LXZ-LWY;

	LMatrix[M10] = LXY-LWZ;
	LMatrix[M11] = 1.0f-(LXX+LZZ);
	LMatrix[M12] = LYZ+LWX;

	LMatrix[M20] = LXZ+LWY;
	LMatrix[M21] = LYZ-LWX;
	LMatrix[M22] = 1.0f-(LXX+LYY);

	LMatrix[M03] = 0.0f;
	LMatrix[M13] = 0.0f;
	LMatrix[M23] = 0.0f;
	LMatrix[M30] = 0.0f;
	LMatrix[M31] = 0.0;LMatrix[M32] = 0.0;LMatrix[M33] = 1.0f;
}

void CrtMatrixTranspose( const CrtMatrix LSrcMtx, CrtMatrix LDstMtx )
{
	CrtMatrix outMat;
	CrtMatrixCopy( LSrcMtx, outMat ); 
	
	LDstMtx[M00] = outMat[M00];
	LDstMtx[M01] = outMat[M10];
	LDstMtx[M02] = outMat[M20];
	LDstMtx[M03] = outMat[M30];
		
	LDstMtx[M10] = outMat[M01];
	LDstMtx[M11] = outMat[M11];
	LDstMtx[M12] = outMat[M21];
	LDstMtx[M13] = outMat[M31];

	LDstMtx[M20] = outMat[M02];
	LDstMtx[M21] = outMat[M12];
	LDstMtx[M22] = outMat[M22];
	LDstMtx[M23] = outMat[M32];

	LDstMtx[M30] = outMat[M03];
	LDstMtx[M31] = outMat[M13];
	LDstMtx[M32] = outMat[M23];
	LDstMtx[M33] = outMat[M33];
}

void CrtMatrix4x4RotateAngleAxis(CrtMatrix LMatrix, CrtVec4f AxisRot)
{
	CrtMatrix	LRotMatrix;
	CrtVec4f	LQuaternion;

	// Build the rotation matix based off of the axis rotation passed in 
	CrtFloat	LAngleSin = (CrtFloat)sin(AxisRot.w*0.5f*CrtDEGREES_TO_RADIANS),
			LAngleCos = (CrtFloat)cos(AxisRot.w*0.5f*CrtDEGREES_TO_RADIANS);


	CrtMatrixLoadIdentity(LRotMatrix);
	LQuaternion.x = AxisRot.x*LAngleSin;
	LQuaternion.y = AxisRot.y*LAngleSin;
	LQuaternion.z = AxisRot.z*LAngleSin;
	LQuaternion.w = LAngleCos;
	CrtQuaternionToMatrix(&LQuaternion, LRotMatrix);

	// concate to previously passed in matrix 
	CrtMatrix4x4Mult(LRotMatrix, LMatrix);
}

void CrtMatrix4x4Translate(CrtMatrix LMatrix, CrtVec4f translate)
{
	CrtMatrix	TMatrix;
	
	// Build the translation matrix 
	CrtMatrixLoadIdentity(TMatrix);
	TMatrix[M30] = translate.x; 
	TMatrix[M31] = translate.y; 
	TMatrix[M32] = translate.z;

	// concatinate to previously passed in matrix 
	CrtMatrix4x4Mult(TMatrix, LMatrix);
}
void CrtMatrix4x4Scale(CrtMatrix LMatrix, CrtVec4f scale)
{
	CrtMatrix	TMatrix;
	
	// Build the translation matrix 
	CrtMatrixLoadIdentity(TMatrix);
	TMatrix[M00] = scale.x; 
	TMatrix[M11] = scale.y; 
	TMatrix[M22] = scale.z;

	// concatinate to previously passed in matrix 
	CrtMatrix4x4Mult(TMatrix, LMatrix);
}



void CrtMatrix4x4Mult(CrtMatrix LSrcMtx, CrtMatrix LDestMtx)
{
	CrtFloat	L00, L01, L02, L03, L10, L11, L12, L13,
			L20, L21, L22, L23, L30, L31, L32, L33;

	// !!!GAC as an experiment, reorder the operations so the lifetime of the temps is limited
	// !!!GAC the compiler should do this, but I want to make sure it's doing its job.

	L00 = LDestMtx[M00];
	L10 = LDestMtx[M10];
	L20 = LDestMtx[M20];
	L30 = LDestMtx[M30];
	LDestMtx[M00] = LSrcMtx[M00]*L00+LSrcMtx[M01]*L10+LSrcMtx[M02]*L20+LSrcMtx[M03]*L30;
	LDestMtx[M10] = LSrcMtx[M10]*L00+LSrcMtx[M11]*L10+LSrcMtx[M12]*L20+LSrcMtx[M13]*L30;
	LDestMtx[M20] = LSrcMtx[M20]*L00+LSrcMtx[M21]*L10+LSrcMtx[M22]*L20+LSrcMtx[M23]*L30;
	LDestMtx[M30] = LSrcMtx[M30]*L00+LSrcMtx[M31]*L10+LSrcMtx[M32]*L20+LSrcMtx[M33]*L30;

	L01 = LDestMtx[M01];
	L11 = LDestMtx[M11];
	L21 = LDestMtx[M21];
	L31 = LDestMtx[M31];
	LDestMtx[M01] = LSrcMtx[M00]*L01+LSrcMtx[M01]*L11+LSrcMtx[M02]*L21+LSrcMtx[M03]*L31;
	LDestMtx[M11] = LSrcMtx[M10]*L01+LSrcMtx[M11]*L11+LSrcMtx[M12]*L21+LSrcMtx[M13]*L31;
	LDestMtx[M21] = LSrcMtx[M20]*L01+LSrcMtx[M21]*L11+LSrcMtx[M22]*L21+LSrcMtx[M23]*L31;
	LDestMtx[M31] = LSrcMtx[M30]*L01+LSrcMtx[M31]*L11+LSrcMtx[M32]*L21+LSrcMtx[M33]*L31;
	
	L02 = LDestMtx[M02];
	L12 = LDestMtx[M12];
	L22 = LDestMtx[M22];
	L32 = LDestMtx[M32];
	LDestMtx[M02] = LSrcMtx[M00]*L02+LSrcMtx[M01]*L12+LSrcMtx[M02]*L22+LSrcMtx[M03]*L32;
	LDestMtx[M12] = LSrcMtx[M10]*L02+LSrcMtx[M11]*L12+LSrcMtx[M12]*L22+LSrcMtx[M13]*L32;
	LDestMtx[M22] = LSrcMtx[M20]*L02+LSrcMtx[M21]*L12+LSrcMtx[M22]*L22+LSrcMtx[M23]*L32;
	LDestMtx[M32] = LSrcMtx[M30]*L02+LSrcMtx[M31]*L12+LSrcMtx[M32]*L22+LSrcMtx[M33]*L32;
	
	L03 = LDestMtx[M03];
	L13 = LDestMtx[M13];
	L23 = LDestMtx[M23];
	L33 = LDestMtx[M33];
	LDestMtx[M03] = LSrcMtx[M00]*L03+LSrcMtx[M01]*L13+LSrcMtx[M02]*L23+LSrcMtx[M03]*L33;
	LDestMtx[M13] = LSrcMtx[M10]*L03+LSrcMtx[M11]*L13+LSrcMtx[M12]*L23+LSrcMtx[M13]*L33;
	LDestMtx[M23] = LSrcMtx[M20]*L03+LSrcMtx[M21]*L13+LSrcMtx[M22]*L23+LSrcMtx[M23]*L33;
	LDestMtx[M33] = LSrcMtx[M30]*L03+LSrcMtx[M31]*L13+LSrcMtx[M32]*L23+LSrcMtx[M33]*L33;
}


bool CompressTransform::init()
{
	return true;
}

int CompressTransform::execute()
{
	int error = 0;

	CompressTransforms(_dae);

	return 0;
}


Conditioner::Register< CompressTransformProxy > CompressTransformProxy;

