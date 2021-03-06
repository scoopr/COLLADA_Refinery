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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.Box;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JTextArea;

import refinery.Refinery;

public class ExecuteWindow extends JFrame implements WindowListener, LogOutput{
	private static final long serialVersionUID = 2077443792918583988L;
	private Refinery refinery;
	private JPanel panel;
	private JButton buttonStopClose;
	private JProgressBar jProgressBar1;
	private JTextArea log;
	private JLabel title;
	private Dimension verticalSpace;
	private Dimension borderSpace;
	
	private int totalStepNumber;
	private int currentStep;
	private boolean closeOnFinish;
	
	public boolean stop;
	
	public ExecuteWindow(Refinery _refinery, JFrame parent, int _totalStepNumber){
		refinery = _refinery;
		totalStepNumber = _totalStepNumber;
		currentStep = 0;
		stop = false;
		closeOnFinish = false;
		panel = new JPanel();
		title = new javax.swing.JLabel();
		log = new javax.swing.JTextArea();
		jProgressBar1 = new javax.swing.JProgressBar();
		//buttonStopClose = new javax.swing.JButton(new ImageIcon("icons/Stop16.gif"));
		buttonStopClose = new javax.swing.JButton(new ImageIcon(refinery.getClass().getResource("/icons/Stop16.gif")));
		verticalSpace = new Dimension(0, 10);
		borderSpace = new Dimension(10,10);
		
		setTitle("Pipeline execution");
		
		panel.setLayout(new javax.swing.BoxLayout(panel, javax.swing.BoxLayout.Y_AXIS));
		
		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
		title.setText("Executing...");
		title.setAlignmentX(Component.CENTER_ALIGNMENT);
		panel.add(title);
		
		panel.add(Box.createRigidArea(verticalSpace));
		
		log.setEditable(false);
		log.setLineWrap(true);
		log.setBorder(new javax.swing.border.LineBorder(new java.awt.Color(0, 0, 0)));
		panel.add(log);
		
		panel.add(Box.createRigidArea(verticalSpace));
		
		jProgressBar1.setMaximum(_totalStepNumber);;
		panel.add(jProgressBar1);
		
		panel.add(Box.createRigidArea(verticalSpace));
		
		buttonStopClose.setText("Stop");
		buttonStopClose.setAlignmentX(Component.CENTER_ALIGNMENT);
		buttonStopClose.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				stopClose();
			}
		}
		);
		panel.add(buttonStopClose);
		
		add(Box.createRigidArea(borderSpace), BorderLayout.PAGE_START);
		add(Box.createRigidArea(borderSpace), BorderLayout.PAGE_END);
		add(Box.createRigidArea(borderSpace), BorderLayout.LINE_START);
		add(Box.createRigidArea(borderSpace), BorderLayout.LINE_END);
		add(panel, BorderLayout.CENTER);
		
		addWindowListener(this);
		setPreferredSize(new Dimension(500, 300));
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		pack();
		setVisible(true);
		
		refinery.setTempLoger(this);
	}
	
	public void addStep(){
		if (currentStep < totalStepNumber){
			currentStep++;
			jProgressBar1.setValue(currentStep);
		}
		if (currentStep == totalStepNumber){
			buttonStopClose.setText("Close");
			if (closeOnFinish) dispose();
		}
	}
	
	public void addLog(String newLog){
		log.append(newLog+"\n");
		log.setCaretPosition(log.getDocument().getLength());
	}
	
	private void stopClose(){
		if (currentStep == totalStepNumber){
			dispose();
			return;
		}
		int answer = JOptionPane.showConfirmDialog(
			    this,
			    "Do you confirm you want to stop execution ?",
			    "Confirm",
			    JOptionPane.YES_NO_OPTION);
		if (answer == JOptionPane.YES_OPTION){
			stop = true;
			addLog("Stopping...");
		}
	}
	
	public void finish(){
		currentStep = totalStepNumber;
		jProgressBar1.setValue(currentStep);
		buttonStopClose.setText("Close");
	}

	/* (non-Javadoc)
	 * @see java.awt.Window#dispose()
	 */
	@Override
	public void dispose() {
		super.dispose();
		refinery.unsetTempLoger();
	}

	public void windowOpened(WindowEvent e) {}

	public void windowClosing(WindowEvent e) {
		if (currentStep == totalStepNumber){
			dispose();
			return;
		}
		int answer = JOptionPane.showConfirmDialog(
			    this,
			    "Do you confirm you want to stop execution ?",
			    "Confirm",
			    JOptionPane.YES_NO_OPTION);
		if (answer == JOptionPane.YES_OPTION){
			stop = true;
			closeOnFinish = true;
			addLog("Stopping...");
		}
		
	}

	public void windowClosed(WindowEvent e) {}

	public void windowIconified(WindowEvent e) {}

	public void windowDeiconified(WindowEvent e) {}

	public void windowActivated(WindowEvent e) {}

	public void windowDeactivated(WindowEvent e) {}
	
	
}
