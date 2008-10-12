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
package refinery.output;

import java.util.Iterator;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JOptionPane;


/**
 * The <code>ErrorAndWarningReport</code> is a simple class to store errors and warnings during a process and report
 * them to the user at the end with a dialog window.
 * @author pdavid
 *
 */
public class ErrorAndWarningReport {
	Vector<String> errors;
	Vector<String> warnings;
	
	public ErrorAndWarningReport(){
		errors = new Vector<String>();
		warnings = new Vector<String>();
	}
	
	public void addError(String message){
		errors.add(message);
	}
	
	public void addWarning(String message){
		warnings.add(message);
	}
	
	public boolean hasErrors(){
		return errors.size() > 0;
	}
	
	public boolean hasWarnings(){
		return warnings.size() > 0;
	}
	
	public void popupWindow(){
		popupWindow(null);
	}
	
	public void popupWindow(JFrame parent){
		popupWindow(parent, null);
	}
	
	public void popupWindow(JFrame parent, String additionalMessage){
		String message = "";
		String title = "";
		int icon = JOptionPane.INFORMATION_MESSAGE;
		
		if (hasErrors()){
			icon = JOptionPane.ERROR_MESSAGE;
			title = "Error";
			if (errors.size() > 1){
				message = errors.size() + " errors occured :\n";
				Iterator<String> errMessage = errors.iterator();
				while(errMessage.hasNext()){
					message += " - " + errMessage.next() + "\n";
				}
			} else {
				message = "An error occured : \n - " + errors.get(0)+ "\n";
			}
			
			if (hasWarnings()){ // Errors + warning(s)
				message += "\n";
				if (warnings.size() > 1){
					message += "And " + warnings.size() + "warnings \n";
					Iterator<String> warnMessage = warnings.iterator();
					while (warnMessage.hasNext()){
						message += " - " + warnMessage.next() + "\n";
					}
				} else {
					message += "And one warning :\n" + warnings.get(0) + "\n";
				}
			}
			
		} else if (hasWarnings()){ // Only warnings
			icon = JOptionPane.WARNING_MESSAGE;
			title = "Warning";
			
			if (warnings.size() > 1){
				message += warnings.size() + "warnings \n";
				Iterator<String> warnMessage = warnings.iterator();
				while (warnMessage.hasNext()){
					message += " - " + warnMessage.next() + "\n";
				}
			} else {
				message += "Warning :\n" + warnings.get(0) + "\n";
			}
			
		} else { // Just an information windows
			if (additionalMessage == null) {return;}
			
			icon = JOptionPane.INFORMATION_MESSAGE;
			title = "Info";
			
		}
		
		if (additionalMessage != null){
			message += "\n" + additionalMessage;
		}
		
		
		JOptionPane.showMessageDialog(parent, message, title, icon);

	}
}
