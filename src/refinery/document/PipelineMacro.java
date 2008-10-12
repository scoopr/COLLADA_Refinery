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

import java.io.File;
import java.io.IOException;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.UUID;
import java.util.Vector;
import java.util.Map;

import javax.xml.XMLConstants;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.xml.sax.SAXException;

import refinery.MacroTemplate;
import refinery.OptionList;
import refinery.ParsingErrorException;
import refinery.Refinery;
import refinery.XMLOutput;
import refinery.display.DisplayAreaInterface;
import refinery.display.PipelineMacroSimpleDisplay;
import refinery.document.Anchor.AnchorType;
import refinery.history.Identifiable;
import refinery.LibLoader;
import refinery.EntryException;

public class PipelineMacro extends Pipeline implements Box, Identifiable {
	// Instance variables
	//private String name;
	private String libName;
	
	// Run time variables
	private Vector<Anchor> anchorsIn;
	private Vector<Anchor> anchorsOut;
	private PipelineMacroSimpleDisplay simpleDisplay;
	private OptionList options;
	private UUID id;
	
	// Execution variables
	private static HashSet<String> loadingMacros = null;
	protected String prefix;

	private Hashtable<String, String> cloneList;
	private Vector<Box> callList;
	
	public PipelineMacro(Refinery _refinery, MacroTemplate macroBase) throws SAXException, ParserConfigurationException, IOException, GraphException {
		super(_refinery);   
		autoArrange = false;

		try {
			fullName = macroBase.getIDName();
			description = macroBase.getDescription();
			libName = macroBase.getIDName();
			id = UUID.randomUUID();
			
			if(loadingMacros == null){
				loadingMacros = new HashSet<String>();
				loadingMacros.add(libName);
			} else {
				if(loadingMacros.contains(libName)){
					throw new GraphException("I detected a circular call between macros.\nAborting");
				} else {
					loadingMacros.add(libName);
				}
			}

			options = new OptionList();
			
			// Create anchor lists
			anchorsIn = new Vector<Anchor>();
			anchorsOut = new Vector<Anchor>();
			
			loadFromFile(macroBase.getFile());
			
			loadingMacros.remove(libName);
		} catch (GraphException e) {
			loadingMacros = null;
			throw e;
		} catch (SAXException e) {
			loadingMacros = null;
			throw e;
		} catch (ParserConfigurationException e) {
			loadingMacros = null;
			throw e;
		} catch (IOException e) {
			loadingMacros = null;
			throw e;
		}
		
		
		if (loadingMacros.size() == 0){
			loadingMacros = null;
		}
		
		for(int i = 0; i <macroBase.inputNumber; i++){
			anchorsIn.add(new AnchorMacro(this, AnchorType.ANCHORIN, i));
		}
		
		for(int i = 0; i <macroBase.outputNumber; i++){
			anchorsOut.add(new AnchorMacro(this, AnchorType.ANCHOROUT, i));
		}
		
		autoArrange = true;

		cloneList = new Hashtable<String, String>();
	}

	public void destroy()
	{
		Iterator<ConditionerWrapper> cwIter = conditionersList.iterator();
		while (cwIter.hasNext())
		{
			refinery.getLibLoader().removeConditioner(cwIter.next().getIDName());
		}
		Iterator<PipelineMacro> mIter = macroList.iterator();
		while (mIter.hasNext())
		{
			mIter.next().destroy();
		}

	}

	public void setSimpleDisplayArea(DisplayAreaInterface area) {
		if (simpleDisplay == null){
			simpleDisplay = new PipelineMacroSimpleDisplay(this, history);
		}
		simpleDisplay.repaint();
		area.addDisplayAtRandomPosition(simpleDisplay);
		
	}
	
	

	/*************** Implementation of the Box interface ****************/
	public Anchor getLinkEndAnchor(AnchorType anchorType) {
		if (anchorType == AnchorType.ANCHORIN ){
			switch (anchorsIn.size()){
			case 0:
				return null;
			case 1:
				return getInputAnchor(0);
			default:
				//select the first free Anchor
				Iterator<Anchor> ancs = getInputAnchors();
				while (ancs.hasNext()){
					Anchor anc = ancs.next();
					if (! Refinery.instance.getDocument().getLinksFromAnchor(anc).hasNext()){
						return anc;
					}
				}
				// We found no free Anchor, return null
				return null;
			}
		} else if (anchorType == AnchorType.ANCHOROUT ){
			switch (anchorsOut.size()){
			case 0:
				return null;
			case 1:
				return getOutputAnchor(0);
			default:
				//select the first free Anchor
				Iterator<Anchor> ancs = getOutputAnchors();
				while (ancs.hasNext()){
					Anchor anc = ancs.next();
					if (! Refinery.instance.getDocument().getLinksFromAnchor(anc).hasNext()){
						return anc;
					}
				}
				// We found no free Anchor, return null
				return null;
			}
		}
		return null;
	}

	public Iterator<Anchor> getInputAnchors() {
		return anchorsIn.iterator();
	}

	public Anchor getInputAnchor(int number) {
		if (number >= 0 && number < anchorsIn.size()){
			return anchorsIn.get(number);
		} else {
			return null;
		}
	}
	
	public int getInputNumber(){
		return inputBoxes.size();
	}
	
	/*public IOBox addInputBox(){
		IOBox inputBox = new IOBox(IOBox.IOBoxType.INPUTBOX, history);
		inputBoxes.add(inputBox);
		inputBox.setDisplayArea(displayArea);
		return inputBox;
	}
	
	public IOBox addOuputBox(){
		IOBox outputBox = new IOBox(IOBox.IOBoxType.OUTPUTBOX, history);
		outputBoxes.add(outputBox);
		outputBox.setDisplayArea(displayArea);
		return outputBox;
	}*/
	
	
	
	public Iterator<Anchor> getOutputAnchors(){
		return anchorsOut.iterator();
	}

	public Anchor getOutputAnchor(int anchorNumber) {
		if (anchorNumber >= 0 && anchorNumber < anchorsOut.size()){
			return anchorsOut.get(anchorNumber);
		} else {
			return null;
		}
	}
	
	public int getOutputNumber(){
		return outputBoxes.size();
	}

	public PipelineMacroSimpleDisplay getDisplay() {
		return simpleDisplay;
	}

	public String getName() {
		return fullName;
	}
	
	public void setName(String newName){
		fullName = newName;
	}
	
	public String getIDName(){
		return fullName + getUID();
	}
	
	public String getLibName(){
		return libName;
	}
	
	public UUID getUID() {
		return id;
	}

	public void setUID(UUID _id) {
		id = _id;
	}
	
	public OptionList getOptionList(){
		return options;
	}

	public void setExecutionPrefix(String _prefix){
		prefix = _prefix;
	}
	
	public String getOutput(int number)
	{
		if (number >= getOutputNumber())
			return null;

		Iterator<IOBox> boxiter = outputBoxes.iterator();
		while (boxiter.hasNext())
		{
			IOBox box = boxiter.next();
			if (box.getNumber() == number)
			{
				return box.getInput(0);
			}
		}
		return null;
	}

	public boolean setInput(int number, String str)
	{
		//System.out.println(libName + "input num " + number + "=" + str);
		if (number >= getInputNumber() || str == null || str.equals("") )
			return false;
		//inputBoxes.get(number).setFilename(str);
		Iterator<IOBox> boxiter = inputBoxes.iterator();
		while (boxiter.hasNext())
		{
			IOBox box = boxiter.next();
			if (box.getNumber() == number)
			{
				//System.out.println("setting input box filename");
				box.setFilename(str);
				return true;
			}
		}
		return false;
	}

	public boolean setOutput(int number, String str)
	{
		return false;
	}

	public void addCloneDocument(String fromDoc, String toDoc)
	{
		cloneList.put(fromDoc, toDoc);
	}
	
	/**
	 * executeSolo executes this macro as the single element in a pipeline with the options in args
	 * @param args Options to parse before execution
	 * @throws ParsingErrorException
	 * @throws ExecuteException 
	 */
	public void executeSolo(String [] args) throws ParsingErrorException, ExecuteException{
		//System.out.println("execute solo");
		options.parse(args);
		try
		{
			generateCallList();
		}
		catch (GraphException e)
		{
			throw new ExecuteException(e.getMessage());
		}
		Iterator<Box> executeIt = callList.iterator();
		while (executeIt.hasNext())
		{
			Box executingBox = executeIt.next();
			if (!(executingBox instanceof IOBox))
			{
				if (executingBox.execute() != 0)
				{
					throw new ExecuteException(executingBox.getName() + " did not execute successfully. Aborting execution");
				}
			}
		}
	}

	public void generateCallList() throws GraphException 
	{
		//System.out.println("Macro " + libName + " makeCallList" );
		callList = makeCallList();
	}

	public int execute() throws ExecuteException 
	{
		Iterator<Box> executeIt = callList.iterator();
		while(executeIt.hasNext()){
			Box executingBox = executeIt.next();
			//dont execute inputs or outputs.
			if (!(executingBox instanceof IOBox))
			{
				if (executingBox.execute() != 0)
				{
					throw new ExecuteException(executingBox.getName() + " did not execute successfully. Aborting execution");
				}
			}
		}
		
		return 0;
	}
	
	private String addFilePrefix(String filename){
		int separationOffset = filename.lastIndexOf('/');
		String result = filename.substring(0, separationOffset+1);
		//result += prefix;
		result += filename.substring(separationOffset+1);
		return result;
	}
	
	public void toSimpleXML(XMLOutput output){
		output.startEntity("macro");
		output.addAttribute("name", fullName);
		output.addAttribute("idname", getIDName());
		output.addAttribute("libname", libName);
		
		
		if (simpleDisplay != null){
			output.startEntity("display");
			output.startEntity("x");
			output.addBodyText(String.valueOf(simpleDisplay.getX()));
			output.endEntity();
			
			output.startEntity("y");
			output.addBodyText(String.valueOf(simpleDisplay.getY()));
			output.endEntity();
			output.endEntity();
		}
		
		output.endEntity();
	}
	
	private void loadFromFile(File file) throws SAXException, ParserConfigurationException, IOException{
		FileLoader contentHandler = new FileLoader(refinery, this, displayArea);
		SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);		
		Schema schema = factory.newSchema(refinery.getClass().getResource("/savefiles.xsd"));
		SAXParserFactory spf = SAXParserFactory.newInstance();
		spf.setSchema(schema);
		SAXParser saxParser = spf.newSAXParser();
		saxParser.parse(file, contentHandler);
	}

	public void checkEntriesForExecution(Pipeline doc) throws Exception {
		Iterator<Anchor> ancIt = anchorsIn.iterator();
		while (ancIt.hasNext())
		{
			if (!doc.getLinksFromAnchor(ancIt.next()).hasNext())
			{
				throw new EntryException("Error in " + libName + ": not all inputs are plugged");
			}
		}

		Iterator<Box> boxIt = callList.iterator();
		while (boxIt.hasNext())
		{
			Box box = boxIt.next();
			try
			{
				if (!(box instanceof IOBox))
				{
					box.checkEntriesForExecution(this);
				}
			}
			catch (Exception e)
			{
				throw new EntryException("Error in Macro: " + libName + ": " + e.getMessage());
			}
		}
	}

}
