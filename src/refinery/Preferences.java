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
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;

import javax.xml.XMLConstants;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.validation.SchemaFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class Preferences {
	private HashMap<String, Option> preferences;
	
	public final String preferenceFile = Refinery.RefineryDirectory + File.separator + "preferences.xml";
	
	public Preferences(){
		preferences = new HashMap<String, Option>();
		initialFeed();
		setDefaultValues();
	}
	
	private void initialFeed(){
		preferences.put("GUI_POSITION", new Option("GUI_POSITION", "GUI position", "Initial window position", "100,100"));
		preferences.put("GUI_SIZE", new Option("GUI_SIZE", "GUI size", "Initial window size", "1000,800"));
		preferences.put("GUI_PIPDIR", new Option("GUI_PIPDIR", "Default pipeline files directory", "", "savefiles"));
		preferences.put("GUI_DAEDIR", new Option("GUI_DAEDIR", "Default dae files directory", "", "Collada_data"));
		
		preferences.put("DIR_LIB", new Option("DIR_LIB", "Conditioner directory", "Directory where conditioners are stored and loaded at launch", "conditioners"));
		preferences.put("DIR_MACRO", new Option("DIR_MACRO", "Macro directory", "Directory where macros are stored and loaded at launch", "macros"));
		//preferences.put("DIR_TEMP", new Option("DIR_TEMP", "Temporary directory", "Directory where temporary execution files are stored", "tmp"));
		
		//preferences.put("REMOTE_IP",  new Option("REMOTE_IP", "Remote COLLADA server IP", "IP of the host running COLLADA remote", "0.0.0.0"));
		//preferences.put("REMOTE_PORT",  new Option("REMOTE_PORT", "Remote COLLADA server port", "Port binded by COLLADA remote (default is 2345)", "2345"));
		//preferences.put("REMOTE_DATADIR", new Option("REMOTE_DATADIR", "Data dir root", "Local bind of the root directory in which COLLADA remote in expecting files", "."));
		preferences.put("UPDATE", new Option("UPDATE", "Check for Updates", "Value that specifies whether or not to check for updates at start up", "Yes"));
	}

	public void setDefaultValues(){
		preferences.get("GUI_POSITION").setDefaultValue();
		preferences.get("GUI_SIZE").setDefaultValue();
		preferences.get("GUI_PIPDIR").setDefaultValue();
		preferences.get("GUI_DAEDIR").setDefaultValue();
		
		preferences.get("DIR_LIB").setDefaultValue();
		preferences.get("DIR_MACRO").setDefaultValue();
		//preferences.get("DIR_TEMP").setDefaultValue();
		
		//preferences.get("REMOTE_IP").setDefaultValue();
		//preferences.get("REMOTE_PORT").setDefaultValue();
		//preferences.get("REMOTE_DATADIR").setDefaultValue();
		preferences.get("UPDATE").setDefaultValue();
	}
	
	public Option getOption(String optionName){
		return preferences.get(optionName);
	}
	
	public String getValue(String optionName){
		Option o = preferences.get(optionName);
		if(o!= null){
			return o.value;
		} else {
			return null;
		}
	}
	
	public void setValue(String optionName, String value){
		Option o = preferences.get(optionName);
		if (o != null){
			o.value = value;
		}
	}
	
	public void load(){
		try {
			PrefLoader loader = new PrefLoader();
			SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
			SAXParserFactory spf = SAXParserFactory.newInstance();
			SAXParser saxParser = spf.newSAXParser();
			saxParser.parse(new File(preferenceFile), loader);
		} catch (Exception e){
			Refinery.instance.addDebugMessage(preferenceFile + " not found or failed to load, default preferences loaded");
		}
	}
	
	public void save() throws IOException{
		XMLOutput output = new XMLOutput();
		
		FileOutputStream fos = new FileOutputStream(new File(preferenceFile));
		
		// Feed with data
		
		output.startEntity("Preferences");
			addOption(output, "GUI_POSITION");
			addOption(output, "GUI_SIZE");
			addOption(output, "GUI_PIPDIR");
			addOption(output, "GUI_DAEDIR");
			addOption(output, "DIR_LIB");
			addOption(output, "DIR_MACRO");
			//addOption(output, "DIR_TEMP");
			//addOption(output, "REMOTE_IP");
			//addOption(output, "REMOTE_PORT");
			//addOption(output, "REMOTE_DATADIR");
			addOption(output, "UPDATE");
		output.endEntity();
		//
		fos.write(output.toString().getBytes());
		
		fos.close();
		
		//System.out.println("Saving preferences");
	}
	
	private void addOption(XMLOutput output, String name){
		output.startEntity("option");
		output.addAttribute("name", name);
		output.addAttribute("value", preferences.get(name).value);
		output.endEntity();
	}
	
	public class Option{
		String name;
		String title;
		String description;
		String defaultValue;
		String value;
		public Option(String _name, String _title, String _description, String _defaultValue){
			name = _name;
			title = _title;
			description = _description;
			defaultValue = _defaultValue;
			value = null;
		}
		
		public void setDefaultValue(){
			value = defaultValue;
		}
	}
	
	private class PrefLoader extends DefaultHandler {
		public void startElement(String namespaceURI, String localName, String qName, Attributes atts) throws SAXException{
			if(qName.equals("option")){
				Option o = preferences.get(atts.getValue("name"));
				if (o != null){
					o.value = atts.getValue("value");
				}
			}
		}
	}
}
