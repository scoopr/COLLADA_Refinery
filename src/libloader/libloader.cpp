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
#include "libloader.h"

#include <windows.h>
#include <winbase.h>
#include <map>
#include <iostream>
#include "conditioner.h"

#include <dae.h>

std::map< std::string, const ConditionerCreator* > LibLoader::_availableConditioners;
std::map< std::string, Conditioner* > LibLoader::_conditioners;

//JNIEnv *LibLoader::_env = NULL;
//jobject LibLoader::_obj;
JavaVM *LibLoader::_jvm = NULL;
jint LibLoader::_envVer = 0;

DAE *_dae = NULL;

void LibLoader::init( JNIEnv *env, jobject obj )
{
	_envVer = env->GetVersion();
	if ( env->GetJavaVM( &_jvm ) != 0 )
	{
		fprintf(stderr,"Could not get current Java Virtual Machine!\n");
	}
	//_env = env;
	//_obj = obj;
	if ( _dae == NULL )
	{
		_dae = new DAE();
		_dae->setDatabase( NULL );
		_dae->setIOPlugin( NULL );
	}
}

void LibLoader::cleanup()
{
	if (_dae != NULL )
	{
		delete _dae;
		_dae = NULL;
	}
	DAE::cleanup();
}

void LibLoader::registerConditioner( const ConditionerCreator *cc )
{
	//printf( "registering a conditioner\n" );
	_availableConditioners.insert( std::make_pair( cc->getBaseName(), cc ) );
}

void LibLoader::unregisterConditioner( const ConditionerCreator *cc )
{
	//printf( "unregistering a conditioner\n" );
	_availableConditioners.erase( cc->getBaseName() );
}

unsigned int LibLoader::getNumAvailableConditioners()
{
	//printf( "num avail conds = %d\n", _availableConditioners.size() );
	return (unsigned int)_availableConditioners.size();
}

const ConditionerCreator *LibLoader::getAvailableConditioner( unsigned int idx )
{
	if ( idx >= getNumAvailableConditioners() )
		return NULL;

	std::map< std::string, const ConditionerCreator* >::iterator iter;
	iter = _availableConditioners.begin();
	for ( unsigned int x = 0; x < idx; ++x )
	{
		++iter;
	}
	return iter->second;
}

const ConditionerCreator *LibLoader::getAvailableConditioner( const std::string &key )
{
	std::map< std::string, const ConditionerCreator* >::iterator iter;
	iter = _availableConditioners.find( key );
	if ( iter == _availableConditioners.end() )
		return NULL;

	return iter->second;
}

bool LibLoader::addConditioner( const std::string &name, Conditioner *cond )
{
	//printf( "Added %s\n", name.c_str() );
	bool retval = _conditioners.insert( std::make_pair( name, cond ) ).second;
	if (retval)
	{
		cond->setDAE( _dae );
	}
	return retval;
}

Conditioner *LibLoader::getConditioner( const std::string &name )
{
	std::map< std::string, Conditioner* >::iterator iter;
	iter = _conditioners.find( name );
	if ( iter == _conditioners.end() )
		return NULL;
	
	return iter->second;
}

bool LibLoader::removeConditioner( const std::string &name )
{
	return _conditioners.erase( name ) > 0;
}

void LibLoader::printDebugMessage( const std::string &message )
{
	JNIEnv *_env;
	if ( _jvm->GetEnv( (void**)&_env, _envVer ) != 0 )
	{
		fprintf(stderr,"Could not get current Java Environment!\n");
	}
	jstring resultJString = _env->NewStringUTF( message.c_str() );
	jclass cls = _env->FindClass("refinery/Refinery");
	if ( cls == NULL )
	{
		fprintf(stderr,"Unable to find class LibLoader!\n");
	}
	jfieldID fid =  _env->GetStaticFieldID( cls, "instance", "Lrefinery/Refinery;");
	if ( fid == 0 )
	{
		fprintf(stderr,"Could not get Refinery instance!\n");
	}
	jobject obj = _env->GetStaticObjectField(cls, fid );
	fid = _env->GetFieldID( cls, "extLibs", "Lrefinery/LibLoader;");
	jobject _obj = _env->GetObjectField( obj, fid );

   	jmethodID mid = _env->GetMethodID( _env->GetObjectClass( _obj ), "debugMessage", "(Ljava/lang/String;)V" );

   	if ( mid == NULL ) {
   		printf( "debugMessage not found !!\n" );
        return;
    }
    jvalue args[1];
    args[0].l = resultJString;
    _env->CallObjectMethodA( _obj, mid, args );
}

void LibLoader::printExecutionMessage( const std::string &message )
{
	JNIEnv *_env;
	if ( _jvm->GetEnv( (void**)&_env, _envVer ) != 0 )
	{
		fprintf(stderr,"Could not get current Java Environment!\n");
	}
	jstring resultJString = _env->NewStringUTF( message.c_str() );
	jclass cls = _env->FindClass("refinery/Refinery");
	if ( cls == NULL )
	{
		fprintf(stderr,"Unable to find class LibLoader!\n");
	}
	jfieldID fid =  _env->GetStaticFieldID( cls, "instance", "Lrefinery/Refinery;");
	if ( fid == 0 )
	{
		fprintf(stderr,"Could not get Refinery instance!\n");
	}
	jobject obj = _env->GetStaticObjectField(cls, fid );
	fid = _env->GetFieldID( cls, "extLibs", "Lrefinery/LibLoader;");
	jobject _obj = _env->GetObjectField( obj, fid );

   	jmethodID mid = _env->GetMethodID( _env->GetObjectClass( _obj ),"executionMessage", "(Ljava/lang/String;)V" );

   	if ( mid == NULL ) {
   		printf( "executionMessage not found !!\n" );
        return;
    }
    jvalue args[1];
    args[0].l = resultJString;
    _env->CallObjectMethodA( _obj, mid, args );
}

void LibLoader::printErrorMessage( const std::string &message )
{
	JNIEnv *_env;
	if ( _jvm->GetEnv( (void**)&_env, _envVer ) != 0 )
	{
		fprintf(stderr,"Could not get current Java Environment!\n");
	}
	jstring resultJString = _env->NewStringUTF( message.c_str() );
	jclass cls = _env->FindClass("refinery/Refinery");
	if ( cls == NULL )
	{
		fprintf(stderr,"Unable to find class LibLoader!\n");
	}
	jfieldID fid =  _env->GetStaticFieldID( cls, "instance", "Lrefinery/Refinery;");
	if ( fid == 0 )
	{
		fprintf(stderr,"Could not get Refinery instance!\n");
	}
	jobject obj = _env->GetStaticObjectField(cls, fid );
	fid = _env->GetFieldID( cls, "extLibs", "Lrefinery/LibLoader;");
	jobject _obj = _env->GetObjectField( obj, fid );

   	jmethodID mid = _env->GetMethodID( _env->GetObjectClass( _obj ),"setErrorMessage", "(Ljava/lang/String;)V");

   	if ( mid == NULL ) {
   		printf( "setErrorMessage not found !!\n" );
        return;
    }
    jvalue args[1];
    args[0].l = resultJString;
    _env->CallObjectMethodA( _obj, mid, args );
}

bool LibLoader::loadDocument( const std::string &doc )
{
	return _dae->load( doc.c_str() ) == DAE_OK;
}

bool LibLoader::saveDocument( const std::string &doc, const std::string &toFile )
{
	return _dae->saveAs( toFile.c_str(), doc.c_str() ) == DAE_OK;
}

bool LibLoader::cloneDocument( const std::string &from, const std::string &to )
{
	daeDocument *fromDoc = _dae->getDatabase()->getDocument( from.c_str() );
	daeDocument *toDoc = _dae->getDatabase()->getDocument( to.c_str() );
	if ( fromDoc == NULL || toDoc != NULL )
		return false;

	_dae->getDatabase()->createDocument( to.c_str(), &toDoc );
	if ( toDoc == NULL )
		return false;

	toDoc->setDomRoot( fromDoc->getDomRoot()->clone() );
	daeElement::resolveAll();
	return true;
}

void LibLoader::clearDOM()
{
	_dae->clear();
}
