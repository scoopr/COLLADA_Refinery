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
