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
