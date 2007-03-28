/* --- DO NOT REMOVE THIS LINE --- SNIP_SDK_COPYRIGHT_C_BEGIN
 * --- DO NOT REMOVE THIS LINE --- SNIP_SDK_COPYRIGHT_C_END */

#ifndef __DAE_BXCERESOLVER__
#define __DAE_BXCERESOVLER__

#include "dae/daeURI.h"
class daeIOPlugin;
class daeDatabase;

/**
 * The @c daebXCeResolver class derives from @c daeURIResolver and implements
 * the default XML backend resolver.
 */
class daebXCeResolver : public daeURIResolver
{
public:
	/**
	 * Constructor.
	 * @param database The @c daeDatabase used.
	 * @param plugin The @c daeIOPlugin used.
	 */
	daebXCeResolver(daeDatabase* database, daeIOPlugin* plugin);
	/**
	 * Destructor.
	 */
	~daebXCeResolver();

protected:
	daeDatabase* _database;
	daeIOPlugin* _plugin;
public:
public: // Abstract Interface
	virtual daeBool resolveElement(daeURI& uri, daeString typeNameHint = NULL);
	virtual daeBool resolveURI(daeURI& uri);
	virtual daeString getName();
	virtual daeBool isProtocolSupported(daeString protocol);
	virtual daeBool isExtensionSupported(daeString extension);
};

#endif //__DAE_XMLRESOLVER__

