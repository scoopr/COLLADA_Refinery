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

import java.util.Iterator;
import java.util.Vector;


/**
 * The <code>CommandLineParser</code> class parses the command line according to what we expect and sets
 * some values. This is a little more handful than a general commmand line parser in our case, e.g. to
 * treat the end of the command line options as a conditioner options and not ours.  
 * @author Philippe David
 */

//TODO Maybe we can add options to debug behaviors of libs and macros. Like listing those which are loaded, print debug info about those which are not loaded, etc.
public class CommandLineParser {
	private Refinery.Mode mode = Refinery.Mode.Normal;
	private String libName;
	private String [] libArgs;
	private String macroName;
	private String fileName;
	private Vector<String> inFiles;
	private Vector<String> outFiles;

	public CommandLineParser(String[] args) throws ParsingErrorException
	{
		libName = "";
		macroName = "";
		fileName = "";
		inFiles = new Vector<String>();
		outFiles = new Vector<String>();


		// Let's parse the command line
		int argPos;
		boolean finished = false;
		for (argPos = 0; argPos < args.length && !finished; argPos++)
		{

			// User wants to execute a conditioner :
			if (currentArgument(args, argPos, "--execute-lib", "-x", 1))
			{
				argPos++;
				libName = args[argPos];

				libArgs = new String[args.length-(argPos+1)];
				for (int i = 0; i < libArgs.length; i++)
				{
					libArgs[i] = args[argPos + i + 1];
				}
				if (mode != Refinery.Mode.Normal)
				{
					error("You can only choose one mode between lib, macro or file");
				}
				mode = Refinery.Mode.ExLib;
				//return;

			}
			else if (currentArgument(args, argPos, "--execute-macro", "-m", 1))
			{
				// User wants to execute a macro:
				//System.out.println("CLP 1");
				argPos++;
				macroName = args[argPos];

				if (mode != Refinery.Mode.Normal)
				{
					error("You can only choose one mode between lib, macro or file");
				}
				mode = Refinery.Mode.ExMacro;
				libArgs = new String[args.length-(argPos+1)];
				for (int i = 0; i < libArgs.length; i++)
				{
					libArgs[i] = args[argPos + i + 1];
				}

			}
			else if (currentArgument(args, argPos, "--execute-file", "-f", 1))
			{
				// User wants to execute a file:
				//System.out.println("CLP 5");
				argPos++;
				fileName = args[argPos];
				if (mode != Refinery.Mode.Normal)
				{
					error("You can only choose one mode between lib, macro or file");
				}
				mode = Refinery.Mode.ExFile;


			}
			else if (currentArgument(args, argPos, "-i", 1))
			{
				//System.out.println("CLP 2");
				argPos++;
				inFiles.add(args[argPos]);

			}
			else if (currentArgument(args, argPos, "-o", 1))
			{
				//System.out.println("CLP 3");
				argPos++;
				outFiles.add(args[argPos]);


			}
			else if (currentArgument(args, argPos, "--help"))
			{
				if (mode != Refinery.Mode.Normal)
				{
					error("You can only choose one mode");
				}
				mode = Refinery.Mode.Help;
				printUsage();

			}
			else if (currentArgument(args, argPos, "--help-lib", 1))
			{
				if (mode != Refinery.Mode.Normal)
				{
					error("You can only choose one mode");
				}
				mode = Refinery.Mode.HelpLib;
				argPos++;
				libName = args[argPos];
			}
			else if (currentArgument(args, argPos, "--list"))
			{
				if (mode != Refinery.Mode.Normal)
				{
					error("You can only choose one mode");
				}
				mode = Refinery.Mode.ListLibs;
				argPos++;
			}
			else if (mode != Refinery.Mode.ExLib)
			{
				//				System.out.println("CLP 4");
				//				error("Unknown option :" + args[argPos]);
				System.err.println("Unknown option :" + args[argPos]);
				/*if (mode != Refinery.Mode.Normal)
				{
					error("You can only choose one mode");
				}*/
				mode = Refinery.Mode.Help;
				printUsage();
			}
		}
	}
	
	private void error(String message) throws ParsingErrorException{
		mode = Refinery.Mode.Error;
		throw new ParsingErrorException(message);
	}
	
	private boolean currentArgument(String []args, int pos, String optionName){
		if (args[pos].equals(optionName)){
			return true;
		} else {
			return false;
		}
	}
	
	private boolean currentArgument(String []args, int pos, String optionName, int nbArgAfter) throws ParsingErrorException{
		if (args[pos].equals(optionName)){
			if(pos+nbArgAfter >= args.length){
				error("You must specify a file to call after "+optionName);
			}
			return true;
		}
		return false;
	}
	
	private boolean currentArgument(String []args, int pos, String optionName1, String optionName2, int nbArgAfter) throws ParsingErrorException{
		if (args[pos].equals(optionName1) || args[pos].equals(optionName2)){
			if(pos+nbArgAfter >= args.length){
				error("You must specify a file to call after "+optionName1 +" or "+ optionName2);
			}
			return true;
		}
		return false;
	}
	
	public Refinery.Mode getMode(){
		return mode;
	}
	
	public String getLibName(){
		return libName;
	}
	
	public String getFileName(){
		return fileName;
	}
	
	public String getMacroName(){
		return macroName;
	}
	
	public String [] getLibArgs(){
		return libArgs;
	}
	
	public int getInputFileNumber(){
		return inFiles.size();
	}
	
	public Iterator<String> getInputFiles(){
		return inFiles.iterator();
	}
	
	public Iterator<String> getOutputFiles(){
		return outFiles.iterator();
	}
	
	public void printUsage(){
		String usage = "Refinery command line usage:\n";
		usage += "\trefinery [options] (--execute-lib|-x) <libname> [liboptions]\n";
		usage += "\t\tExecute the contionner called <libname> in the lib directory\n\t\tand pass all following options to this conditionner\n\n";

		usage += "\trefinery [options] (--execute-macro|-m) <macroname> -i <input filename> -o <output filename>\n";
		usage += "\t\tExecute the macro called <macroname> in the macro directory\n\n";

		usage += "\trefinery [options] (--execute-file|-f) <filename>\n";
		usage += "\t\tExecute the pipeline saved into <filename>\n\n";

		usage += "\trefinery --list\n";
		usage += "\t\tList all conditioners and macros found.\n\n";

		usage += "\trefinery --help\n";
		usage += "\t\tPrint this message.\n\n";

		usage += "\trefinery --help-lib <libname>\n";
		usage += "\t\tPrint description and options for a particular lib.\n\n";
		
		System.out.println(usage);
	}
}
