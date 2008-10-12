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
