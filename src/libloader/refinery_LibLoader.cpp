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
#include "refinery_LibLoader.h"
#include "libloader.h"
#include "conditioner.h"

JNIEXPORT void JNICALL Java_refinery_LibLoader_init
  (JNIEnv *env, jobject obj )
{
	//printf( "in Init\n" );
	LibLoader::init( env, obj );
}

JNIEXPORT jint JNICALL Java_refinery_LibLoader_getNumAvailableConditioners
  (JNIEnv *, jobject )
{
	return LibLoader::getNumAvailableConditioners();
}

JNIEXPORT jobject JNICALL Java_refinery_LibLoader_getConditioner
  (JNIEnv *env, jobject obj, jint idx)
{
	const ConditionerCreator *cc = LibLoader::getAvailableConditioner( idx );
	if ( cc == NULL )
		return NULL;

	jstring name = env->NewStringUTF( cc->getBaseName().c_str() );
	jstring desc = env->NewStringUTF( cc->getDescription().c_str() );
	jstring cat = env->NewStringUTF( cc->getCategory().c_str() );

	jmethodID mid = env->GetMethodID( env->GetObjectClass(obj), 
		"newConditionerBase", 
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lrefinery/ConditionerTemplate;");

   	if (mid == 0) {
   		printf("newConditionerBase not found !!\n");
        return NULL;
    }

    jvalue args[3];
    args[0].l = name;
    args[1].l = desc;
	args[2].l = cat;
    return env->CallObjectMethodA( obj, mid, args );
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_loadConditioner
  (JNIEnv *env, jobject obj, jstring baseName, jstring name, jobject cw )
{
	//Look for conditioner
	const char *nm = env->GetStringUTFChars( baseName, 0 );
	std::string nmStr = std::string( nm );
	env->ReleaseStringUTFChars( baseName, nm );

	const ConditionerCreator *cc = LibLoader::getAvailableConditioner( nmStr );
	if ( cc == NULL ) 
	{
		return false;
	}
	//create conditioner
	Conditioner *cond = cc->create();

	//add conditioner
	nm = env->GetStringUTFChars( name, 0 );
	nmStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm );

	cond->setName( nmStr );

	if ( !LibLoader::addConditioner( nmStr, cond ) )
	{
		delete cond;
		return false;
	}

	//set up conditioner
	cond->init();
	//set up ConditionerWrapper
	//set number of inputs and outputs
	jclass cls = env->GetObjectClass( cw );
	jmethodID mid = env->GetMethodID( cls, "init", "(IIZ[Ljava/lang/String;[Z)V");

   	if (mid == 0) {
   		printf("init not found !!\n");
        return false;
    }

	unsigned int numOuts = cond->getNumOutputs();
	jboolean *ba = new jboolean[numOuts*2];
	jbooleanArray barray= env->NewBooleanArray( numOuts *2 );

	jobjectArray oa = env->NewObjectArray( numOuts, env->GetObjectClass( name ), env->NewStringUTF( "" ) );

	for ( unsigned int i = 0; i < numOuts; i++ )
	{
		env->SetObjectArrayElement( oa, i, env->NewStringUTF( cond->getOutput(i).c_str() ) );
		ba[i*2   ] = cond->isOutputLinked(i);
		ba[i*2 +1] = cond->isOutputEditable(i);
	}
	env->SetBooleanArrayRegion( barray, 0, numOuts*2, ba );
	delete[] ba;

    jvalue args[5];
    args[0].i = cond->getNumInputs();
    args[1].i = numOuts;
	args[2].z = cond->hasVariableNumInputs();
	args[3].l = oa;
	args[4].l = barray;

	env->CallVoidMethodA( cw, mid, args );

	return true;
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_removeConditioner
  (JNIEnv *env, jobject, jstring name)
{
	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nmStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm );

	return LibLoader::removeConditioner( nmStr );
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_loadBoolOptions
  (JNIEnv *env, jobject obj, jstring name, jobject cw )
{
	//add other options
	jclass cls = env->GetObjectClass( cw );
	jmethodID mid = env->GetMethodID( cls, "addBoolOption", 
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
	
   	if (mid == 0) {
   		printf("addBoolOption not found !!\n");
        return false;
    }

	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nmStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm );

	Conditioner *cond = LibLoader::getConditioner( nmStr );
	if ( cond == NULL )
	{
		printf("unable to find Conditioner named %s!!\n", nmStr.c_str() );
		return false;
	}

	Conditioner::BoolOptionMap::const_iterator bIter = cond->getBoolOptionsMap().begin();
	while ( bIter != cond->getBoolOptionsMap().end() )
	{
		jstring name = env->NewStringUTF( bIter->first.c_str() );
		jstring fName = env->NewStringUTF( bIter->second.fullName.c_str() );
		jstring desc = env->NewStringUTF( bIter->second.description.c_str() );

		jvalue bArgs[4];
		bArgs[0].l = name;
		bArgs[1].l = fName;
		bArgs[2].l = desc;
		bArgs[3].z = bIter->second.value;

		env->CallVoidMethodA( cw, mid, bArgs );

		++bIter;
	}
	return true;
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_loadStringOptions
  (JNIEnv *env, jobject obj, jstring name, jobject cw )
{
	jclass cls = env->GetObjectClass( cw );
	jmethodID mid = env->GetMethodID( cls, "addStringOption", 
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");

   	if (mid == 0) {
   		printf("addStringOption not found !!\n");
        return false;
    }

	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nmStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm );

	Conditioner *cond = LibLoader::getConditioner( nmStr );
	if ( cond == NULL )
	{
		printf("unable to find Conditioner named %s!!\n", nmStr.c_str() );
		return false;
	}

	Conditioner::StringOptionMap::const_iterator sIter = cond->getStringOptionsMap().begin();
	while ( sIter != cond->getStringOptionsMap().end() )
	{
		jstring name = env->NewStringUTF( sIter->first.c_str() );
		jstring fName = env->NewStringUTF( sIter->second.fullName.c_str() );
		jstring desc = env->NewStringUTF( sIter->second.description.c_str() );
		jstring val = env->NewStringUTF( sIter->second.value.c_str() );
		jboolean flag = sIter->second.flag;

		jvalue sArgs[5];
		sArgs[0].l = name;
		sArgs[1].l = fName;
		sArgs[2].l = desc;
		sArgs[3].l = val;
		sArgs[4].z = flag;

		env->CallVoidMethodA( cw, mid, sArgs );

		++sIter;
	}
	return true;
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_loadFloatOptions
  (JNIEnv *env, jobject obj, jstring name, jobject cw )
{
	jclass cls = env->GetObjectClass( cw );
	jmethodID mid = env->GetMethodID( cls, "addFloatOption", 
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;F)V");

   	if (mid == 0) {
   		printf("addFloatOption not found !!\n");
        return false;
    }

	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nmStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm );

	Conditioner *cond = LibLoader::getConditioner( nmStr );
	if ( cond == NULL )
	{
		printf("unable to find Conditioner named %s!!\n", nmStr.c_str() );
		return false;
	}

	Conditioner::FloatOptionMap::const_iterator fIter = cond->getFloatOptionsMap().begin();
	while ( fIter != cond->getFloatOptionsMap().end() )
	{
		jstring name = env->NewStringUTF( fIter->first.c_str() );
		jstring fName = env->NewStringUTF( fIter->second.fullName.c_str() );
		jstring desc = env->NewStringUTF( fIter->second.description.c_str() );
		
		jvalue fArgs[4];
		fArgs[0].l = name;
		fArgs[1].l = fName;
		fArgs[2].l = desc;
		fArgs[3].f = fIter->second.value;

		env->CallVoidMethodA( cw, mid, fArgs );

		++fIter;
	}

	return true;
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_changeConditionerName
  (JNIEnv *env, jobject obj, jstring oldName, jstring newName )
{
	const char *nm = env->GetStringUTFChars( oldName, 0 );
	std::string oStr = std::string( nm );
	env->ReleaseStringUTFChars( oldName, nm );

	Conditioner *cond = LibLoader::getConditioner( oStr );
	if ( cond == NULL ) {
		printf( "unable to find conditioner: %s\n", oStr.c_str() );
		return false;
	}

	nm = env->GetStringUTFChars( newName, 0 );
	std::string nStr = std::string( nm );
	env->ReleaseStringUTFChars( newName, nm );

	cond->setName( nStr );

	if ( !LibLoader::addConditioner( nStr, cond ) )
	{
		printf( "unable to add conditioner: %s\n", nStr.c_str() );
		return false;
	}

	return LibLoader::removeConditioner( oStr );
}

JNIEXPORT jint JNICALL Java_refinery_LibLoader_execute
  (JNIEnv *env, jobject obj, jstring name )
{
	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nmStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm ); 	
	
	//printf("entering execute libName is %s\n", nmStr.c_str() );
	Conditioner *cond = LibLoader::getConditioner( nmStr );
	if ( cond == NULL )
	{
		printf( "failed to find conditioner named %s!\n", nmStr.c_str() );
		return -1;
	}
	/*for ( unsigned int i = 0; i < cond->getNumInputs(); i++ )
	{
		printf("input%d is %s\n", i, cond->getInput( i ).c_str() );
	}*/

	return cond->execute();
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_loadDocument
  (JNIEnv *env, jclass cls, jstring doc)
{
	const char *nm = env->GetStringUTFChars( doc, 0 );
	std::string nmStr = std::string( nm );
	env->ReleaseStringUTFChars( doc, nm ); 

	return LibLoader::loadDocument( nmStr );	
}

JNIEXPORT jboolean JNICALL Java_refinery_LibLoader_saveDocument
  (JNIEnv *env, jclass cls, jstring doc, jstring toFile )
{
	const char *nm = env->GetStringUTFChars( doc, 0 );
	std::string docStr = std::string( nm );
	env->ReleaseStringUTFChars( doc, nm ); 

	nm = env->GetStringUTFChars( toFile, 0 );
	std::string toStr = std::string( nm );
	env->ReleaseStringUTFChars( toFile, nm ); 

	return LibLoader::saveDocument( docStr, toStr );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_setInputNumber
  (JNIEnv *env, jobject, jstring name, jint num)
{
	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nameStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm ); 

	Conditioner *cond = LibLoader::getConditioner( nameStr );
	if ( cond == NULL )
	{
		return;
	}
	cond->setNumInputs( num, true );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_setInput
  (JNIEnv *env, jobject, jstring name, jint num, jstring doc)
{
	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nameStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm ); 

	nm = env->GetStringUTFChars( doc, 0 );
	std::string docStr = std::string( nm );
	env->ReleaseStringUTFChars( doc, nm ); 

	Conditioner *cond = LibLoader::getConditioner( nameStr );
	if ( cond == NULL )
	{
		return;
	}
	cond->setInput( num, docStr );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_setOutput
  (JNIEnv *env, jobject, jstring name, jint num, jstring doc)
{
	const char *nm = env->GetStringUTFChars( name, 0 );
	std::string nameStr = std::string( nm );
	env->ReleaseStringUTFChars( name, nm ); 

	nm = env->GetStringUTFChars( doc, 0 );
	std::string docStr = std::string( nm );
	env->ReleaseStringUTFChars( doc, nm ); 

	Conditioner *cond = LibLoader::getConditioner( nameStr );
	if ( cond == NULL )
	{
		return;
	}
	cond->setOutput( num, docStr );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_setBoolOption
  (JNIEnv *env, jobject, jstring cName, jstring oName, jboolean val )
{
	const char *nm = env->GetStringUTFChars( cName, 0 );
	std::string nameStr = std::string( nm );
	env->ReleaseStringUTFChars( cName, nm ); 

	nm = env->GetStringUTFChars( oName, 0 );
	std::string optionStr = std::string( nm );
	env->ReleaseStringUTFChars( oName, nm ); 

	Conditioner *cond = LibLoader::getConditioner( nameStr );
	if ( cond == NULL )
	{
		return;
	}
	cond->setBoolOption( optionStr, (val != 0)  );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_setStringOption
  (JNIEnv *env, jobject, jstring cName, jstring oName, jstring val )
{
	const char *nm = env->GetStringUTFChars( cName, 0 );
	std::string nameStr = std::string( nm );
	env->ReleaseStringUTFChars( cName, nm ); 

	nm = env->GetStringUTFChars( oName, 0 );
	std::string optionStr = std::string( nm );
	env->ReleaseStringUTFChars( oName, nm ); 

	nm = env->GetStringUTFChars( val, 0 );
	std::string valStr = std::string( nm );
	env->ReleaseStringUTFChars( val, nm );

	Conditioner *cond = LibLoader::getConditioner( nameStr );
	if ( cond == NULL )
	{
		return;
	}
	cond->setStringOption( optionStr, valStr );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_setFloatOption
  (JNIEnv *env, jobject, jstring cName, jstring oName, jfloat val)
{
	const char *nm = env->GetStringUTFChars( cName, 0 );
	std::string nameStr = std::string( nm );
	env->ReleaseStringUTFChars( cName, nm ); 

	nm = env->GetStringUTFChars( oName, 0 );
	std::string optionStr = std::string( nm );
	env->ReleaseStringUTFChars( oName, nm ); 

	Conditioner *cond = LibLoader::getConditioner( nameStr );
	if ( cond == NULL )
	{
		return;
	}
	cond->setFloatOption( optionStr, val );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_cloneDocument
  (JNIEnv *env, jclass, jstring from, jstring to)
{
	const char *nm = env->GetStringUTFChars( from, 0 );
	std::string fromStr = std::string( nm );
	env->ReleaseStringUTFChars( from, nm ); 

	nm = env->GetStringUTFChars( to, 0 );
	std::string toStr = std::string( nm );
	env->ReleaseStringUTFChars( to, nm );

	LibLoader::cloneDocument( fromStr, toStr );
}

JNIEXPORT void JNICALL Java_refinery_LibLoader_clearDOM
  (JNIEnv *, jclass)
{
	LibLoader::clearDOM();
}

