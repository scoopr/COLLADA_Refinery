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
package refinery;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;


public class MacroTemplateLoader extends DefaultHandler {
	private MacroTemplate mb;
	private String currentCharacterBuffer;
	
	public MacroTemplateLoader(MacroTemplate _mb){
		mb = _mb;
		currentCharacterBuffer = "";
	}
	
	public void startElement(String namespaceURI, String localName, String qName, Attributes atts) throws SAXException {
		currentCharacterBuffer = "";
		if (qName.equals("macro")){
			String name = atts.getValue("libname");
			mb.addRequiredMacro(name);
		} else if(qName.equals("conditioner")) {
			String name = atts.getValue("libname");
			mb.addRequiredLib(name);
		} else if(qName.equals("input")){
			mb.inputNumber++;
		} else if(qName.equals("output")){
			mb.outputNumber++;
		}
	}

	public void characters(char[] ch, int start, int length) throws SAXException {
		if (currentCharacterBuffer == null) return;
		
		char []buffer = new char[length];
		for (int i = 0; i <length; i++){
			buffer[i] = ch[start + i];
		}
		currentCharacterBuffer+= new String(buffer);
	}

	public void endElement(String uri, String localName, String qName) throws SAXException {
		if(qName.equals("fullname")){
			mb.setIDName(currentCharacterBuffer);
		 } else if (qName.equals("description")) {
			 mb.setDescription(currentCharacterBuffer);
		 }
	}
	  
	public void endDocument() throws SAXException {
	}
	
	public void error(SAXParseException exception) throws SAXException {
		  //System.out.println(exception.getMessage());
		//TODO report this error to user
		  fatalError(exception);
	  }
	  
}
