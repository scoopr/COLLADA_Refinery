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
package refinery.display;

import java.awt.*;
import java.awt.event.*;
import java.util.Iterator;
import java.util.Vector;
import java.io.File;

import javax.swing.*;
import javax.swing.border.*;

import refinery.Refinery;
import refinery.Option;
import refinery.document.ConditionerWrapper;
import refinery.history.ActionEditConditioner;
import refinery.history.ActionHistory;


/**
 * The <code>ConditionerPropertyWindow</code> class is a JDialog used to edit properties of a
 * ConditionerWrapper. It reads possible options and display controls to edit them.
 * 
 * @author Philippe David
 */
public class ConditionerPropertyWindow extends JDialog {
	// This is to please Java compiler and remove a warning, but I doubt we ever use it
	private static final long serialVersionUID = 1950399361499178217L;

	private ConditionerWrapper cw;	// ConditionerWrapper we are displaying properties for
	private ActionHistory history;	// History class to which we report changes
	
	JTextField textName;
	Vector<Saveable> optionsToSave;
	
	public ConditionerPropertyWindow(JFrame parent, ConditionerWrapper _cw, ActionHistory _history) {
		super(parent, true);
		cw = _cw;
		history = _history;
		optionsToSave = new Vector<Saveable>();
		
		JPanel topPanel = new JPanel();
		topPanel.setLayout(new BoxLayout(topPanel, BoxLayout.PAGE_AXIS));

		JPanel panel = new JPanel();
		panel.setBorder(BorderFactory.createEmptyBorder(1,1,1,1));
		panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
		JLabel labelName = new JLabel("Name");
		textName = new JTextField(cw.getName(), 20);
		labelName.setHorizontalTextPosition(SwingConstants.LEFT);
		labelName.setAlignmentX(Component.LEFT_ALIGNMENT);
		labelName.setPreferredSize(new Dimension(100, 15));
		panel.add(labelName);
		panel.add(textName);
		topPanel.add(panel);
		
		// Add the option to modify the input number if needed
		if (cw.isVariableInputNumber()){
			panel = new JPanel();
			panel.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
			panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
			JLabel label = new JLabel("Input number");
			label.setHorizontalTextPosition(SwingConstants.LEFT);
			label.setAlignmentX(Component.LEFT_ALIGNMENT);
			label.setPreferredSize(new Dimension(100, 20));
			topPanel.add(label);
			InputNumberEdit inputEdit = new InputNumberEdit(cw);
			panel.add(inputEdit);
			optionsToSave.add(inputEdit);
			topPanel.add(panel);
		}
		
		Iterator<Option> optIt = cw.getOptionList().getOptions();
		while(optIt.hasNext()){
			Option o = optIt.next();
			switch(o.type){
			case BOOL:
				panel = new JPanel();
				panel.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
				panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
				JLabel label = new JLabel(o.fullName);
				label.setHorizontalTextPosition(SwingConstants.LEFT);
				label.setAlignmentX(Component.LEFT_ALIGNMENT);
				label.setPreferredSize(new Dimension(100, 20));
				panel.add(label);
				BooleanCombo combo = new BooleanCombo(o.name, cw);
				combo.setPreferredSize(label.getPreferredSize());
				optionsToSave.add(combo);
				panel.add(combo);
				topPanel.add(panel);
				break;
			case STRING:
				panel = new JPanel();
				panel.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
				panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
				label = new JLabel(o.fullName);
				label.setHorizontalTextPosition(SwingConstants.LEFT);
				label.setAlignmentX(Component.LEFT_ALIGNMENT);
				label.setPreferredSize(new Dimension(100, 20));
				panel.add(label);
				StringEdit edit = new StringEdit(o.name, cw);
				optionsToSave.add(edit);
				panel.add(edit);
				if (o.flag)
				{
					JButton browse = new JButton("Browse");
					browse.setSize(new Dimension(20, (int)edit.getSize().getHeight()));
					browse.addActionListener(new ActionChooseFile(this, edit));
					panel.add(browse);
				}
				topPanel.add(panel);				
				break;
			case FLOAT:
				panel = new JPanel();
				panel.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
				panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
				label = new JLabel(o.fullName);
				label.setHorizontalTextPosition(SwingConstants.LEFT);
				label.setAlignmentX(Component.LEFT_ALIGNMENT);
				label.setPreferredSize(new Dimension(100, 20));
				panel.add(label);
				FloatEdit fedit = new FloatEdit(o.name, cw);
				optionsToSave.add(fedit);
				panel.add(fedit);
				topPanel.add(panel);
				break;
			}
		}

		//output names
		for (int i = 0; i < cw.getOutputNumber(); i++)
		{
			if (cw.isOutputEditable(i))
			{
				panel = new JPanel();
				panel.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
				panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
				JLabel label = new JLabel("output" + i);
				label.setHorizontalTextPosition(SwingConstants.LEFT);
				label.setAlignmentX(Component.LEFT_ALIGNMENT);
				label.setPreferredSize(new Dimension(100, 20));
				panel.add(label);
				OutputEdit oe = new OutputEdit(i, cw);
				optionsToSave.add(oe);
				panel.add(oe);
				topPanel.add(panel);
			}
		}
		topPanel.add(Box.createVerticalGlue());
			
		// Action buttons
		JPanel bottomPanel = new JPanel();
		JButton buttonSave = new JButton("Save");
		JButton buttonCancel = new JButton("Cancel");
		buttonSave.addActionListener(new ActionSave());
		buttonCancel.addActionListener(new ActionQuit());
		getRootPane().setDefaultButton(buttonSave);
		bottomPanel.add(buttonSave);
		bottomPanel.add(buttonCancel);
		
		
		// Place objects in the panel
		Container contentPane = getContentPane();
        contentPane.add(topPanel, BorderLayout.CENTER);
        contentPane.add(bottomPanel, BorderLayout.PAGE_END);
		
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		
		pack();
		//setLocation(parent.getLocation());
		setLocationRelativeTo(parent);
		setVisible(true);
	}
	
	private interface Saveable{
		public abstract void save(ActionEditConditioner cwHistory) throws Exception;
	}
	
	/**
	 * <code>BooleanCombo</code> is a <code>JComboBox</code> that directly reports changes
	 * to a ConditionerWrapper
	 */
	private class BooleanCombo extends JComboBox implements Saveable
	{
		private static final long serialVersionUID = -3933888412772783461L;
		private String name;
		private ConditionerWrapper cw;
		private Boolean initialValue;
		
		public BooleanCombo(String _name, ConditionerWrapper _cw)
		{
			name = _name;
			cw = _cw;
			Option o = cw.getOptionList().getOption(name);
			initialValue = (Boolean)o.getValue();
			addItem(Boolean.FALSE);
			addItem(Boolean.TRUE);
			setSelectedItem(o.getValue());
		}
		
		public void save(ActionEditConditioner cwHistory)
		{
			Boolean newValue = (Boolean) this.getSelectedItem();
			if ( !initialValue.equals(newValue))
			{
				Option o = cw.getOptionList().getOption(name);
				o.setValue(newValue);
				cwHistory.addBoleanOptionChange(name, initialValue, newValue);
			}
		}
	}
	
	private class StringEdit extends JTextField implements Saveable
	{
		private static final long serialVersionUID = -2049323226731418745L;
		private String name;
		private ConditionerWrapper cw;
		private String initialValue;

		public StringEdit(String _name, ConditionerWrapper _cw)
		{
			super(20);
			name = _name;
			cw = _cw;
			initialValue = cw.getOptionList().getValue(name);
			setText(initialValue);
		}
		
		public void save(ActionEditConditioner cwHistory){
			String newValue = getText();
			if ( !initialValue.equals(newValue)){
				cw.getOptionList().setValue(name, newValue);
				cwHistory.addStringOptionChange(name, initialValue, newValue);
			}
		}
	}

	private class FloatEdit extends JTextField implements Saveable
	{
		private static final long serialVersionUID = -3846323212345418745L;
		private String name;
		private ConditionerWrapper cw;
		private Float initialValue;

		public FloatEdit(String _name, ConditionerWrapper _cw)
		{
			super(20);
			name = _name;
			cw = _cw;
			Option o = cw.getOptionList().getOption(name);
			initialValue = (Float)o.getValue();
			setText(String.valueOf(initialValue));
		}

		public void save(ActionEditConditioner cwHistory) throws Exception
		{
			try
			{
				Float newValue = Float.parseFloat(getText());
				if (!initialValue.equals(newValue))
				{
					Option o = cw.getOptionList().getOption(name);
					o.setValue(newValue);
					cwHistory.addFloatOptionChange(name, initialValue, newValue);
				}
			}
			catch (Exception e)
			{
				throw new Exception("Invalid Floating point number");
			}
		}
	}
	
	private class InputNumberEdit extends JTextField implements Saveable{
		private static final long serialVersionUID = -316641578589906652L;
		private ConditionerWrapper cw;
		
		public InputNumberEdit(ConditionerWrapper _cw){
			cw = _cw;
			setText(String.valueOf(cw.getInputNumber()));
		}

		public void save(ActionEditConditioner cwHistory) throws Exception {
			int newValue = Integer.valueOf(getText());
			if (newValue <0 ) throw new Exception ("Input number must be >= 0");
			int oldValue = cw.getInputNumber();
			
			if (newValue < oldValue){
				cwHistory.willLowerInputNumber(oldValue, newValue);
				
				// This is a bit tricky but if we delete some links, we want the delete actions placed before the
				// conditioner edition in the history, while we create them after (and there are added at this time)
				history.reverseCurrentPack();
			} 
			
			cw.setInputNumber(newValue);
			
			if (newValue > oldValue){
				cwHistory.raisedInputNumber(oldValue, newValue);
			}
		}
	}

	private class OutputEdit extends JTextField implements Saveable
	{
		private static final long serialVersionUID = -4358261596731418745L;
		private int num;
		private ConditionerWrapper cw;
		private String initialValue;

		public OutputEdit(int _num, ConditionerWrapper _cw)
		{
			num = _num;
			cw = _cw;
			initialValue = cw.getOutput(num);
			if (initialValue == null)
			{
				setText("");
			}
			else
			{
				setText(initialValue);
			}
		}

		public void save(ActionEditConditioner cwHistory)
		{
			String newValue = getText();
			if (!newValue.endsWith(".dae"))
			{
				newValue += ".dae";
			}
			if (!initialValue.equals(newValue))
			{
				cw.setOutput(num, newValue);
				cwHistory.addFileOptionChange(num, initialValue, newValue);
			}
		}
	}
	
	public class ActionQuit implements ActionListener{
		public void actionPerformed(ActionEvent arg0) {
			dispose();
		}
		
	}
	
	/**
	 * The <code>ActionSave</code> is responsible of checking and saving the new setting to
	 * the related ConditionerWrapper.
	 * 
	 * @author Philippe David
	 *
	 */
	public class ActionSave implements ActionListener{

		public void actionPerformed(ActionEvent arg0) {
			String oldName = cw.getName();
			
			String newName = textName.getText();
			try{
				//TODO check the name matches the format we expect
				if (newName.contains(" ")){
					throw new Exception("Please do not use space in the name");
				}
				
				
				cw.setName(newName);
				history.startPackAll();
				ActionEditConditioner cwHistory = history.addEditConditioner(cw, oldName, newName);
				if (cwHistory == null){
					throw new Exception("Unable to save some options to the history");
				} else {
					Iterator<Saveable> saveIt = optionsToSave.iterator();
					while (saveIt.hasNext()){
						saveIt.next().save(cwHistory);
					}
				}
				history.endPackAll();
			} catch (Exception e){
				history.endPackAll();
				JOptionPane.showMessageDialog(null, e.getMessage(),"Error !",
						JOptionPane.ERROR_MESSAGE);
				return;
			}
			
			dispose();
		}		
	}

	public class ActionChooseFile implements ActionListener
	{
		JDialog parentFrame;
		JTextField tf;
		public ActionChooseFile(JDialog _parentFrame, JTextField text)
		{
			parentFrame = _parentFrame;
			tf = text;
		}

		public void actionPerformed(ActionEvent arg0)
		{

			JFileChooser fc = new JFileChooser();
			File currentFileChooserDirectory = new File(Refinery.instance.preferences.getValue("GUI_DAEDIR"));
			if (currentFileChooserDirectory.exists())
			{
				fc.setCurrentDirectory(currentFileChooserDirectory);
			}
			fc.setSelectedFile(new File(tf.getText()));
			int returnVal = fc.showSaveDialog(parentFrame);
			if (returnVal == JFileChooser.APPROVE_OPTION)
			{
				File file = fc.getSelectedFile();
				tf.setText(file.getPath());
			}
		}
	}
}
