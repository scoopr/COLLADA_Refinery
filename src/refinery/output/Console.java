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

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import refinery.Refinery;

/**
 * The <code>Console</code> class displays a windows in which new text is added at the end and
 * the text area is automatically scrolled to see the last entry.
 * @author Philippe David
 */
public class Console extends JFrame implements WindowListener, LogOutput {
	private static final long serialVersionUID = 145348936939222162L;
	private JTextArea textArea;
	private Refinery refinery;
	
	public Console(Refinery _refinery){
		refinery = _refinery;
		setTitle("Console");
		
		textArea = new JTextArea("Refinery console\n", 15, 50);
		textArea.setWrapStyleWord(true);
		
		JScrollPane scrollPane = new JScrollPane(textArea);
		setPreferredSize(new Dimension(600, 300));
		add(scrollPane, BorderLayout.CENTER);
		
		addWindowListener(this);
		
		pack();
	}
	
	/**
	 * Empty the text area
	 *
	 */
	public void flush(){
		textArea.setText("");
	}
	
	/**
	 * Add text at the end of the buffer
	 * @param text Text to add
	 */
	public void addLog(String text){
		textArea.append(text+"\n");
        textArea.setCaretPosition(textArea.getDocument().getLength());
	}
	
	public void putOnTop(){
		setVisible(true);
	}

	public void windowOpened(WindowEvent e) {}

	public void windowClosing(WindowEvent e) {
		refinery.getGUI().consoleViewCheckBox.setState(false);
		setVisible(false);
	}

	public void windowClosed(WindowEvent e) {}

	public void windowIconified(WindowEvent e) {}

	public void windowDeiconified(WindowEvent e) {}

	public void windowActivated(WindowEvent e) {}

	public void windowDeactivated(WindowEvent e) {}
}
