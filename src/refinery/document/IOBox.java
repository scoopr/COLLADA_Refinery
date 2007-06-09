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
package refinery.document;

import java.io.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.UnknownHostException;
import java.util.*;

import refinery.Refinery;
import refinery.EntryException;
import refinery.OptionList;
import refinery.XMLOutput;
import refinery.display.DisplayAreaInterface;
import refinery.display.IOBoxDisplay;
import refinery.document.Anchor.AnchorType;
import refinery.history.ActionHistory;
import refinery.history.IdentifiableImpl;
import refinery.LibLoader;


/**
 * The <code>IOBox</code> class represents the an input or an output in the conditioner
 * pipeline. It is a one anchor Box with a filename property which is the name of a file
 *  on the filesystem.
 *  
 * @author Philippe David
 */
public class IOBox extends IdentifiableImpl implements Box
{
	
	public enum IOBoxType {INPUTBOX, OUTPUTBOX};
	
	private IOBoxDisplay ioDisplay;
	private ActionHistory history;
	private IOBoxType type;
	private AnchorIO anchor;
	private String name;
	private String filename;
	private boolean remoteExecute;
	private OptionList options;
	private String _input;
	private Hashtable<String, String> cloneList;
	private int number;
	private String command;
	private String directory;
	
	public IOBox(IOBoxType _type, ActionHistory _history) {
		type = _type;
		history = _history;
		ioDisplay = null;;
		
		filename ="";
		remoteExecute = false;

		if (type == IOBoxType.INPUTBOX){
			name = "input";
			anchor = new AnchorIO(this, AnchorType.ANCHOROUT);
		} else {
			name = "output";
			anchor = new AnchorIO(this, AnchorType.ANCHORIN);
		}

		options = new OptionList();
		cloneList = new Hashtable<String, String>();
		command = "";
		directory = "";
	}
	
	public String getFilename() {
		return filename;
	}

	public void setFilename(String fname) {
		filename = "";
		if ( fname.length() > 0 && fname.charAt(0) != '/' )
			filename += "/";
		
		filename += fname.replace( '\\', '/' );

		if (ioDisplay != null){
			ioDisplay.repaint();
		}
	}

	public void setNumber(int number)
	{
		this.number = number;
		if (ioDisplay != null)
		{
			ioDisplay.repaint();
		}
	}

	public int getNumber()
	{
		return number;
	}
	
	public OptionList getOptionList(){
		return options;
	}
	
	public IOBoxType getType(){
		return type;
	}
	
	public Anchor getAnchor(){
		return anchor;
	}

	public Anchor getInputAnchor(int number) {
		if (type != IOBoxType.OUTPUTBOX) return null;
		
		if (number == 0){
			return anchor;
		} else {
			return null;
		}
	}
	
	public Iterator<Anchor> getInputAnchors(){
		Vector<Anchor> tmp = new Vector<Anchor>();
		if (type == IOBoxType.OUTPUTBOX){
			tmp.add(anchor);
		}
		return tmp.iterator();
	}
	
	public Anchor getOutputAnchor(int anchorNumber){
		if (type != IOBoxType.INPUTBOX) return null;

		if (anchorNumber == 0)
		{
			return anchor;
		}
		else
		{
			return null;
		}
	}
	
	public Iterator<Anchor> getOutputAnchors(){
		Vector<Anchor> tmp = new Vector<Anchor>();
		if (type == IOBoxType.INPUTBOX){
			tmp.add(anchor);
		}
		return tmp.iterator();
	}
	
	/**
	 * Returns the Anchor of the given type when a link is finishing on this Box.
	 */
	public Anchor getLinkEndAnchor(AnchorType anchorType){
		if (anchorType == AnchorType.ANCHORIN && type == IOBoxType.OUTPUTBOX){
			return anchor;
		}
		if (anchorType == AnchorType.ANCHOROUT && type == IOBoxType.INPUTBOX){
			return anchor;
		}
		return null;
	}
	

	public void setDisplayArea(DisplayAreaInterface area) {
		if (ioDisplay == null){
			ioDisplay = new IOBoxDisplay(this, history);
		}
		ioDisplay.repaint();
		area.addDisplay(ioDisplay);
		
	}
	
	public IOBoxDisplay getDisplay(){
		return ioDisplay;
	}
	
	public String toString(){
		if (type == IOBoxType.INPUTBOX){
			return "Input " + (number+1);
		} else {
			return "Output " + (number+1);
		}
	}

	public String getName() {
		return name;
	}

	public void setName(String _name) {
		name = _name;
	}
	
	public String getIDName(){
		if (type == IOBoxType.INPUTBOX){
			return "input"+getUID();
		} else {
			return "output"+getUID();
		}
	}
	
	/**
	 * @return Returns the remoteExecute.
	 */
	public boolean isRemoteExecute() {
		return remoteExecute;
	}

	/**
	 * @param remoteExecute The remoteExecute to set.
	 */
	public void setRemoteExecute(boolean remoteExecute) {
		this.remoteExecute = remoteExecute;
	}

	public String getCommand()
	{
		return command;
	}

	public void setCommand(String cmd)
	{
		command = cmd;
	}

	public String getDirectory()
	{
		return directory;
	}

	public void setDirectory(String dir)
	{
		directory = dir;
	}
	
	public void toXML(XMLOutput output){
		
		if (type == IOBoxType.INPUTBOX){
			output.startEntity("input");
			output.addAttribute("name", getIDName());
			output.addAttribute("number", String.valueOf(number));
			if (getFilename() != null && !getFilename().equals(""))
			{
				output.startEntity("file");
				output.addBodyText(getFilename());
				output.endEntity();
			}

		} else {
			output.startEntity("output");
			output.addAttribute("name", getIDName());
			output.addAttribute("number", String.valueOf(number));
			if (getFilename() != null && !getFilename().equals(""))
			{
				output.startEntity("file");
				output.addBodyText(getFilename());
				output.endEntity();
			}
			output.startEntity("execute");
			output.addBodyText(remoteExecute ? "yes" : "no");
			output.endEntity();
			output.startEntity("command");
			output.addBodyText(command);
			output.endEntity();
			output.startEntity("workingDir");
			output.addBodyText(directory);
			output.endEntity();
		}
		
		if(ioDisplay != null){
			output.startEntity("display");
				output.startEntity("x");
				output.addBodyText(String.valueOf(ioDisplay.getX()));
				output.endEntity();
				
				output.startEntity("y");
				output.addBodyText(String.valueOf(ioDisplay.getY()));
				output.endEntity();
			output.endEntity();
		}
		
		output.endEntity();
	}

	public int getOutputNumber()
	{
		if (type == IOBoxType.INPUTBOX)
			return 1;
		return 0;
	}

	public int getInputNumber()
	{
		if (type == IOBoxType.INPUTBOX)
			return 0;
		return 1;
	}

	public String getInput(int number)
	{
		if (number != 0)
			return null;

		if (type == IOBoxType.INPUTBOX)
			return filename;

		return _input;
	}

	public String getOutput(int number)
	{
		if (number != 0)
			return null;
		return filename;
	}

	public boolean setInput(int number, String str)
	{
		if (number != 0 || type == IOBoxType.INPUTBOX || str == null || str.equals(""))
			return false;

		_input = str;
		return true;
	}

	public boolean setOutput(int number, String str)
	{
		return false;
	}

	public void addCloneDocument(String fromDoc, String toDoc)
	{
		cloneList.put(fromDoc, toDoc);
	}
	
	public void checkEntriesForExecution(Pipeline doc) throws Exception {
		if (type == IOBoxType.INPUTBOX){
			// Check input file is readable
			File destination = new File(filename);
			if(filename == null || filename.equals("")){
				throw new EntryException("Error in "+getName()+": you must set an input file");
			}
			if(!destination.canRead()){
				throw new EntryException("Error in "+getName()+": cannot read file "+filename);
			}
		} else {
			// Check destination is writable
			File destination = new File(filename);
			if (destination.exists()){
				if(!destination.canWrite()){
					throw new EntryException("Error in "+getName()+": cannot write to output file "+destination.getPath());
				}
			} else {
				File parentDir = destination.getParentFile();
				if(parentDir != null){
					if(!parentDir.canWrite()){
						throw new EntryException("Error in "+getName()+": cannot write in directory "+parentDir.getPath());
					}
				} else {
					throw new EntryException("Error in "+getName()+": fix the file name or delete the output box if it isn't necessary" + filename);
				}
			}
		}

		if (remoteExecute) 
		{
			if (command.length() == 0)
			{
				throw new EntryException("Error: " + toString() + " No command specified to execute.");
			}
		}

		if (!doc.getLinksFromAnchor(anchor).hasNext())
		{
			throw new EntryException("Error: " + toString() + " is not connected to graph.");
		}
	}
	
	public int execute() throws ExecuteException  {
		if (type == IOBoxType.INPUTBOX)
		{
			LibLoader.loadDocument(filename);
		}
		else
		{
			//clone appropriate documents
			Iterator<Map.Entry<String, String>> iter = cloneList.entrySet().iterator();
			while (iter.hasNext())
			{
				Map.Entry<String, String> entry = iter.next();
				LibLoader.cloneDocument(entry.getKey(), entry.getValue());
			}
			cloneList.clear();
			LibLoader.saveDocument(_input, filename);
		}
		 

		if (type == IOBoxType.OUTPUTBOX && remoteExecute) 
		{
			
			File dir = null;
			if (directory.length() > 0)
			{
				dir = new File(directory);
				if (!dir.isDirectory())
				{
					throw new ExecuteException("Error: " + toString() + " is not a valid directory.");
				}
			}

			try
			{
				java.util.StringTokenizer st = new java.util.StringTokenizer(command);
				List<String> cmd = new LinkedList<String>();

				while (st.hasMoreTokens())
				{
					String tmp = st.nextToken();
					
					String os = System.getProperty("os.name");
					String sysFile = filename;
					if (os == null) //default to windows for the hell of it
					{
						sysFile =  filename.substring(1).replace('/', '\\');
					}
					else if (os.toLowerCase().indexOf("win") != -1)
					{
						sysFile = filename.substring(1).replace('/', '\\');
					}
					
					tmp = tmp.replace( "%f", sysFile );
					tmp = tmp.replace( "%u", "file://" + filename );

					if (cmd.size() == 0 && dir != null)
					{
						tmp = dir.getAbsolutePath() + File.separator + tmp;
					}

					cmd.add(tmp);
				}
				
				//Process p = Runtime.getRuntime().exec(execName, null, dir);
				ProcessBuilder pb = new ProcessBuilder();
				pb.directory(dir);
				pb.command(cmd);
				Process p = pb.start();
				//Process p = rt.exec("COLLADA_RT_VIEWER.exe /c:/svn/Refinery22/sample_data/COLLADA/demo.dae");
			}
			catch(NullPointerException npe) 
			{
				System.err.println("NullPointerException: " + npe.getMessage());
			}
			catch(IndexOutOfBoundsException iobe) 
			{
				System.err.println("IndexOutOfBoundsException: " + iobe.getMessage());
			}
			catch(SecurityException se) 
			{
				System.err.println("SecurityException: " + se.getMessage());
			}
			catch (IOException ioe)
			{
				System.err.println("IOException: " + ioe.getMessage());
			}
			catch(Exception e) 
			{
				System.err.println(e.getMessage());
			}
		}
		return 0;
	}
}
