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

import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import java.util.Vector;

import javax.xml.XMLConstants;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.xml.sax.SAXException;

public class MacroTemplate {
	private String IDName;
	private String description;
	private String category;
	private File file;
	private int count;
	private Vector<String> requiredMacros;
	private Vector<String> requiredLibs;
	public int inputNumber;
	public int outputNumber;
	
	
	public MacroTemplate(File _file) throws ParserConfigurationException, IOException, SAXException{
		file = _file;
		count = 0;
		inputNumber = 0;
		outputNumber = 0;
		
		requiredMacros = new Vector<String>();
		requiredLibs = new Vector<String>();
	
		MacroTemplateLoader contentHandler = new MacroTemplateLoader(this);

//		create a SchemaFactory capable of understanding W3C XML Schemas (WXS)
		SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
//		Set the error handler to receive any error during Schema Compilation
		//factory.setErrorHandler(new MyErrorHandler());
//		set the resource resolver to customize resource resolution
		//factory.setResourceResolver( new MyLSResourceResolver());
//		 load a WXS schema, represented by a Schema instance
		Schema schema = factory.newSchema(this.getClass().getResource("/savefiles.xsd"));
		SAXParserFactory spf = SAXParserFactory.newInstance();
//		Just set the Schema instance on SAXParserFactory
		spf.setSchema(schema);
//		Obtain the SAXParser instance
		SAXParser saxParser = spf.newSAXParser();
//		parser will parse the XML document but validate it using Schema instance
		saxParser.parse(file, contentHandler);
		
	}
	
	public String getIDName(){
		return IDName;
	}
		
	public String getNameWithCount() {
		// To prevent 2 boxes from having the same IDName at creation, we add a counter at the end
		// of the IDName
		count++;
		return IDName+"-"+count;
	}

	public String getDescription(){
		return description;
	}
	
	public File getFile(){
		return file;
	}
	
	public void setIDName(String _IDName){
		IDName = _IDName;
	}
	
	public void setDescription(String _d){
		description = _d;
	}
	
	public void addRequiredMacro(String r){
		requiredMacros.add(r);
	}
	
	public void addRequiredLib(String r){
		requiredLibs.add(r);
	}
	
	public Iterator<String> getRequiredMacros(){
		return requiredMacros.iterator();
	}
	
	public Iterator<String> getRequiredLibs(){
		return requiredLibs.iterator();
	}
	
	/**
	 * @return Returns the category.
	 */
	public String getCategory() {
		return category;
	}

	/**
	 * @param category The category to set.
	 */
	public void setCategory(String category) {
		this.category = category;
	}
}
