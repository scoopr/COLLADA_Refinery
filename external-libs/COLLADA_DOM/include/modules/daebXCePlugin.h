/* --- DO NOT REMOVE THIS LINE --- SNIP_SDK_COPYRIGHT_C_BEGIN
 * --- DO NOT REMOVE THIS LINE --- SNIP_SDK_COPYRIGHT_C_END */

#ifndef __DAEBXCEPLUGIN__
#define __DAEBXCEPLUGIN__

#include <dae/daeElement.h>
#include <dae/daeMetaAttribute.h>
#include <dae/daeIOPlugin.h>

#include <stdio.h>
#include <vector>

class daeIntegrationObject;
class daeMetaElement;
class daeDocument;

/**
 * The @c bXcePlugin class derives from @c daeIOPlugin and implements an XML
 * input/output backend using bXce as a parser. When using this plugin, daeInterface::load() expects
 * an rfc 2396 compliant URI. Only filename paths are supported by bXce. If the URI contains a 
 * fragment it will be ignored and the entire referenced document will be loaded.  
 * daeInterface::saveAs will only handle a filename path at present (ie: no scheme or authority).
 */
class daebXCePlugin : public daeIOPlugin
{
public:
	// Constructor / destructor
	/**
	 * Constructor.
	 */
	daebXCePlugin();
	/**
	 * Destructor.
	 */
	virtual ~daebXCePlugin();
	virtual daeInt setMeta(daeMetaElement *topMeta);

	// Database setup	
	virtual void setDatabase(daeDatabase* database);

	// Operations
	virtual daeInt read(daeURI& uri, daeString docBuffer);
	virtual daeInt write(daeURI *name, daeDocument *document, daeBool replace);//{ return DAE_ERR_NOT_IMPLEMENTED;}

	// Stats	
	virtual void getProgress(daeInt* bytesParsed,
		daeInt* lineNumber,
		daeInt* totalBytes,
		daeBool reset = false );

	daeInt setOption( daeString option, daeString value ) 
	{
		(void)option;
		(void)value;
		return DAE_OK;
	}

	daeString getOption( daeString option )
	{
		(void)option;
		return NULL;
	}
	
private:

	typedef struct
	{
		daeElement* element;
		daeIntegrationObject* intObject;
	} INTEGRATION_ITEM;
	
	daeMetaElement* topMeta;

	daeDatabase* database;
	
	void postProcessDom(daeDocument *document, daeElement* element, std::vector<INTEGRATION_ITEM> &intItems);

	void writeElement( FILE *fp, daeElement* element, daeString indentString = "", daeInt indentLevel = 0 ); 
	void writeAttribute( FILE* fp, daeMetaAttribute* attr, daeElement* element );
};

#endif //__DAEBXCEPLUGIN__
