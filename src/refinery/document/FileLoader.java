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
package refinery.document;

import java.awt.Point;
import java.util.HashMap;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;

import refinery.ConditionerTemplate;
import refinery.MacroTemplate;
import refinery.OptionList;
import refinery.Refinery;
import refinery.display.Display;
import refinery.display.DisplayAreaInterface;

/**
 * The <code>FileLoader</code> class is a SAX handler to load a file and recreate the workspace from it.
 * 
 * @author Philippe David
 */
public class FileLoader extends DefaultHandler {
	private Refinery refinery;
	Pipeline document;
	
	private Box currentBox;
	private ConditionerWrapper currentConditioner;
	private Display currentDisplay;
	private String currentOptionName;
	private OptionList currentOptionList;
	private IOBox currentIOBox;
	private DisplayAreaInterface displayArea;
	private int x;
	private int y;
	private HashMap<String, Box> boxes;
	
	private String currentCharacterBuffer;

	public FileLoader(Refinery _refinery, Pipeline _document, DisplayAreaInterface _displayArea){
		refinery = _refinery;
		displayArea = _displayArea;
		document = _document;
		currentIOBox = null;
		currentBox = null;
		boxes = new HashMap<String, Box>();
		//boxes.put("input", document.getInputBox());
		//boxes.put("output", document.getOutputBox());
	}
	
	  public void startElement(String namespaceURI, String localName, String qName, Attributes atts) throws SAXException{
		  try{
			  currentCharacterBuffer = "";
			  if(qName.equals("conditioner")){		// Conditionner
				  String libName = atts.getValue("libname");
				  ConditionerTemplate cb = refinery.getAvailableCondList().getConditionerBase(libName);
				  if(cb == null) throw new SAXException("The conditioner lib " + libName + "could not be found");
				  currentConditioner = document.addConditioner(cb);
				  
				  String name = atts.getValue("name");
				  currentConditioner.setName(name);
				  
				  String idname = atts.getValue("idname");
				  boxes.put(idname, currentConditioner);
				  
				  if(currentConditioner.isVariableInputNumber() ){
					  String inputValue = atts.getValue("inputnumber");
					  if (inputValue != null){ // It should be null but who knows
						  currentConditioner.setInputNumber(Integer.valueOf(inputValue));
					  }
					  
				  }
				  
				  currentConditioner.setDisplayArea(displayArea);
				  currentBox = currentConditioner;
				  
			  } else if(qName.equals("option")){	// Option
				  currentOptionName = atts.getValue("name");
				  currentOptionList = currentConditioner.getOptionList();
	
			  } else if(qName.equals("macro")){		// Macro
				  String libName = atts.getValue("libname");
				  MacroTemplate mb = refinery.getAvailableMacroList().getMacroBaseFromLibName(libName);
				  if (mb == null) throw new SAXException("Macro "+libName+" not found");
				  PipelineMacro newMacro = document.addMacro(mb);
				  newMacro.setName(atts.getValue("name"));
				  String idname = atts.getValue("idname");
				  boxes.put(idname, newMacro);
				  newMacro.setSimpleDisplayArea(displayArea);
				  currentBox = newMacro;
				  
			  } else if(qName.equals("display")){	// Display
				  x = 0;
				  y = 0;
				  currentDisplay = currentBox.getDisplay();  
				  
			  } else if(qName.equals("link")){		// Link
				  String start = atts.getValue("start");
				  Box startBox = boxes.get(start);
				  String end = atts.getValue("end");
				  Box endBox = boxes.get(end);
				  
				  int startNb = Integer.parseInt(atts.getValue("startNb"));
				  int endNb = Integer.parseInt(atts.getValue("endNb"));
				  if(startBox == null || endBox == null) throw new SAXException("Error while creating link from "+start+" to "+end);
				  document.addLink(new Link(startBox.getOutputAnchor(startNb), endBox.getInputAnchor(endNb)));
				  //System.out.println("found a new link from " + startBox + " : " + startNb + " to " + endBox + " : " + endNb);
				  
			  } else if(qName.equals("input")){
				  currentIOBox = document.addInputBox();
				  boxes.put(atts.getValue("name"), currentIOBox);
				  currentIOBox.setNumber(Integer.parseInt(atts.getValue("number")));
				  //System.out.println("input loaded with number " + Integer.parseInt(atts.getValue("number")));
				  currentBox = currentIOBox;
				  
			  } else if(qName.equals("output")){
				  currentIOBox = document.addOuputBox();
				  boxes.put(atts.getValue("name"), currentIOBox);
				  currentIOBox.setNumber(Integer.parseInt(atts.getValue("number")));
				  currentBox = currentIOBox;
			  }
		  } catch(Exception e){
			  throw new SAXException(e.getMessage());
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
	  
	public void endElement(String namespaceURI, String localName, String qName) throws SAXException {
		if (qName.equals("fullname")){
			document.setFullName(currentCharacterBuffer);
		} else if (qName.equals("description")){
			document.setDescription(currentCharacterBuffer);
		} else if (qName.equals("option")){
			if(currentOptionList == null || currentOptionName == null){
				Refinery.instance.addDebugMessage("Could not find the option "+currentOptionName+" for the conditioner "+currentConditioner.getName()+". Ignored");
			} else {
				currentOptionList.setValue(currentOptionName, currentCharacterBuffer);
			}

		} else if(qName.equals("file")){
			currentIOBox.setFilename(currentCharacterBuffer);
			//currentIOBox = null;
		} else if(qName.equals("x")){
			x = Integer.parseInt(currentCharacterBuffer);
		} else if(qName.equals("y")){
			y = Integer.parseInt(currentCharacterBuffer);
		} else if(qName.equals("display")) {
			if (currentDisplay != null){
				currentDisplay.setPosition(new Point(x, y));
			}
		} else if(qName.equals("execute")){
			if(currentCharacterBuffer.equals("yes")){
				currentIOBox.setRemoteExecute(true);
			} else if (currentCharacterBuffer.equals("no")){
				currentIOBox.setRemoteExecute(false);
			}
		}
		else if (qName.equals("command"))
		{
			currentIOBox.setCommand(currentCharacterBuffer);
		}
		else if (qName.equals("workingDir"))
		{
			currentIOBox.setDirectory(currentCharacterBuffer);
		}
		
		currentCharacterBuffer = "";
	 }
	
	public void endDocument() throws SAXException {
		//document.setSimpleDisplayArea(displayArea);
	  }
	
	
	public void error(SAXParseException exception) throws SAXException {
		  //System.out.println(exception.getMessage());
		  fatalError(exception);
	  }

}
