/* --- DO NOT REMOVE THIS LINE --- SNIP_SDK_COPYRIGHT_C_BEGIN
 * --- DO NOT REMOVE THIS LINE --- SNIP_SDK_COPYRIGHT_C_END */

#ifndef	__DAEBXCEPROCESSOR__
#define	__DAEBXCEPROCESSOR__

#include <bXCeXMLConfig.h>
#include <bXCeXMLProcessor.h>
#include <stdio.h>
#include <dae/daeElement.h>
#include <dae/daeMetaElement.h>

class daebXCeProcessor	: public bXCeXMLProcessor
{
public: //inherited from base
	// Start document event
	int startDocument(void* userData);

	// Start element event
	int startElement(void* userData, const char* name, int length, bXCeAttrList& list);

	// Characters event
	int characters(void* userData, const char* ch, int length);

	// End element event
	int endElement(void* userData, const char* name, int length);

	// End document event
	int endDocument(void* userData);

	// Escaped event
	int skippedText(void* userData, int tokenType, const char* text, int length);
	int entityData(void* userData, int tokenType, const char* name, int length);

	// Error handling
	void fatalError(void* userData, int errCode);

public: //constructor and destrcutor
	daebXCeProcessor();
	~daebXCeProcessor();

public:
	daeElementRef getRoot() { return domRoot;}
	void setTopMeta( daeMetaElement *tm ) { topMeta = tm; }

private:
	bool escapeTextStart;
	daeMetaElement *topMeta;
	daeElementRef domRoot;
	daeElementRef currentElement;

	daeBool skipTillEnd;
	daeChar skipTill[256];
	
	void readAttribute( daeString name, daeString value );
	void readValue();
	daeChar * valueBuf;
	size_t valueBufLength;
};

#endif	/* __DAEBXCEPROCESSOR__ */
