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
		addOption("DIR_LIB", dirPanel);
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
			preferences.setValue("DIR_LIB", texts.get("DIR_LIB").getText());
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
