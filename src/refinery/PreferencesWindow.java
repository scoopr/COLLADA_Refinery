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

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.util.HashMap;

import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JCheckBox;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;


public class PreferencesWindow extends JDialog {
	Preferences preferences;
	private JTabbedPane tabbedPane;
	private HashMap<String, JTextField> texts;
	private JCheckBox cb;
	private boolean initCBValue;

	public PreferencesWindow(JFrame parent){
		super(parent, true);
		preferences = Refinery.instance.preferences;
		texts = new HashMap<String, JTextField>();
		initCompenents();
		setLocationRelativeTo(parent);
	}
	
	public PreferencesWindow(JDialog parent){
		super(parent, true);
		preferences = Refinery.instance.preferences;
		texts = new HashMap<String, JTextField>();
		initCompenents();
		setLocationRelativeTo(parent);
	}
	
	public void initCompenents(){
		JPanel topPanel = new JPanel();
		
		// Directory panel
		JPanel dirPanel = new JPanel();
		dirPanel.setLayout(new GridLayout(0,2));
		addOption("DIR_USER_LIB", dirPanel);
		addOption("DIR_MACRO", dirPanel);
		//addOption("DIR_TEMP", dirPanel);
		
		// Remote viewer panel
		/*JPanel remotePanel = new JPanel();
		remotePanel.setLayout(new GridLayout(0,2));
		addOption("REMOTE_IP", remotePanel);
		addOption("REMOTE_PORT", remotePanel);
		addOption("REMOTE_DATADIR", remotePanel);*/

		JPanel updatePanel = new JPanel();
		updatePanel.setLayout(new GridLayout(0, 1));
		initCBValue = true;
		Preferences.Option o = preferences.getOption("UPDATE");
		if (o.value.equals("NO"))
		{
			initCBValue = false;
		}
		cb = new JCheckBox("Check for updates", initCBValue);
		cb.setToolTipText(o.description);
		updatePanel.add(cb);

		// Document defaults
		//TODO
		
		tabbedPane = new JTabbedPane();
		tabbedPane.addTab("Directories", dirPanel);
		//tabbedPane.addTab("Remote viewer", remotePanel);
		tabbedPane.addTab("Update", updatePanel);
		
		
		topPanel.add(tabbedPane);
		
		
		// Bottom panel with save and quit
		JPanel bottomPanel = new JPanel();
		bottomPanel.setLayout(new javax.swing.BoxLayout(bottomPanel, javax.swing.BoxLayout.Y_AXIS));
		JButton buttonSave = new JButton(new ActionSave());
		JButton buttonCancel = new JButton(new ActionQuit());
		getRootPane().setDefaultButton(buttonSave);
		bottomPanel.add(buttonSave);
		bottomPanel.add(buttonCancel);
		
		Container contentPane = getContentPane();
        contentPane.add(topPanel, BorderLayout.CENTER);
        contentPane.add(bottomPanel, BorderLayout.LINE_END);
		
		// Display the window.
		setTitle("Refinery preferences");
		//setPreferredSize(new Dimension(400,400));
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		
		
		
	}
	
	public void start(){
		pack();
		setVisible(true);
	}
	
	public void selectRemoteTab(){
		tabbedPane.setSelectedIndex(1);
	}
	
	private void addOption(String optionName, JPanel destination){
		Preferences.Option o = preferences.getOption(optionName);
		if (o == null) return;
		JLabel labelName = new JLabel(o.title);
		JTextField textName = new JTextField(o.value, 20);
		labelName.setToolTipText(o.description);
		textName.setToolTipText(o.description);
		destination.add(labelName);
		destination.add(textName);
		texts.put(optionName, textName);
	}
	
	class ActionSave extends AbstractAction{
		
		public ActionSave(){
			super("Save");
		}

		public void actionPerformed(ActionEvent e) {
			preferences.setValue("DIR_USER_LIB", texts.get("DIR_USER_LIB").getText());
			preferences.setValue("DIR_MACRO", texts.get("DIR_MACRO").getText());
			//preferences.setValue("DIR_TEMP", texts.get("DIR_TEMP").getText());
			//preferences.setValue("REMOTE_IP", texts.get("REMOTE_IP").getText());
			//preferences.setValue("REMOTE_PORT", texts.get("REMOTE_PORT").getText());
			//preferences.setValue("REMOTE_DATADIR", texts.get("REMOTE_DATADIR").getText());
			if ( initCBValue != cb.isSelected() )
			{
				if ( cb.isSelected() )
				{
					preferences.setValue("UPDATE", "YES");
				}
				else
				{
					preferences.setValue("UPDATE", "NO");
				}
			}
			try {
				preferences.save();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			dispose();
		}
		
	}
	
	class ActionQuit extends AbstractAction{

		public ActionQuit(){
			super("Quit");
		}
		public void actionPerformed(ActionEvent e) {
			dispose();
			
		}
		
	}
}
