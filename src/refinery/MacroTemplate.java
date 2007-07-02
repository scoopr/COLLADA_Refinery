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
