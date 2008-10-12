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
package refinery.display;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.Box;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;

import refinery.PreferencesWindow;
import refinery.Refinery;
import refinery.document.IOBox;
import refinery.history.ActionHistory;


/**
 * The <code>OutputBoxPropertyWindow</code> class is a JDialog used to edit properties of an
 * IObox. It reads possible options and display controls to edit them.
 * 
 * @author Philippe David
 */
public class OutputBoxPropertyWindow extends JDialog {
	private static final long serialVersionUID = -5936513103766361499L;
	private IOBox io;
	private ActionHistory history;
	
	//	 Variables declaration - do not modify
    private javax.swing.JButton buttonBrowse;
    private javax.swing.JButton buttonCancel;
    private javax.swing.JButton buttonOk;
    private javax.swing.JButton buttonSettings;
    private javax.swing.JCheckBox checkBoxExecute;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel panelExecute;
    private javax.swing.JPanel panelLeft;
    private javax.swing.JPanel panelQuit;
    private javax.swing.JTextField textFileName;
	private javax.swing.JTextField commandField;
	private javax.swing.JTextField dirField;
    // End of variables declaration

	public OutputBoxPropertyWindow(JFrame parent, IOBox _io, ActionHistory _history) {
		super(parent, true);
		io = _io;
		history = _history;
		
		initComponents();
		
		pack();
		setLocationRelativeTo(parent);
		setVisible(true);
	}
	
	private void initComponents() {
        panelLeft = new javax.swing.JPanel();
        jPanel1 = new javax.swing.JPanel();
        textFileName = new javax.swing.JTextField();
        buttonBrowse = new javax.swing.JButton();
        panelExecute = new javax.swing.JPanel();
        checkBoxExecute = new javax.swing.JCheckBox();
        buttonSettings = new javax.swing.JButton();
        panelQuit = new javax.swing.JPanel();
        buttonOk = new javax.swing.JButton();
        buttonCancel = new javax.swing.JButton();
		commandField = new javax.swing.JTextField();
		dirField = new javax.swing.JTextField();

        getContentPane().setLayout(new javax.swing.BoxLayout(getContentPane(), javax.swing.BoxLayout.X_AXIS));

        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setResizable(false);
        panelLeft.setLayout(new javax.swing.BoxLayout(panelLeft, javax.swing.BoxLayout.Y_AXIS));

        jPanel1.setLayout(new javax.swing.BoxLayout(jPanel1, javax.swing.BoxLayout.X_AXIS));

        jPanel1.setBorder(new javax.swing.border.TitledBorder("Destination"));
        //jPanel1.setAlignmentX(0.0F);
        jPanel1.setOpaque(false);
        jPanel1.setPreferredSize(new java.awt.Dimension(300, 52));
        textFileName.setText(io.getFilename());
        jPanel1.add(textFileName);

        buttonBrowse.setText("Browse");
        buttonBrowse.addActionListener(new ActionChooseFile(this));
        jPanel1.add(buttonBrowse);

        panelLeft.add(jPanel1);

		checkBoxExecute.setText("Execute Command");
		checkBoxExecute.setSelected(io.isRemoteExecute());
		checkBoxExecute.setAlignmentX(1.0f);
		panelLeft.add(checkBoxExecute);

        panelExecute.setLayout(new javax.swing.BoxLayout(panelExecute, javax.swing.BoxLayout.Y_AXIS));

        panelExecute.setBorder(new javax.swing.border.TitledBorder("Command"));
        panelExecute.setAutoscrolls(true);
        panelExecute.setMaximumSize(new java.awt.Dimension(2147483647, 2147483647));
        panelExecute.setPreferredSize(new java.awt.Dimension(150, 64));
        //checkBoxExecute.setText("Execute on remote COLLADA viewer (the destination must be the DATA_DIR where the Collada viewer is expecting to find the file");
        //checkBoxExecute.setText("Launch COLLADA RT viewer");
        //checkBoxExecute.setSelected(io.isRemoteExecute());
        //panelExecute.add(checkBoxExecute);

        //buttonSettings.setText("Settings");
        //buttonSettings.addActionListener(new ActionSettings(this));
        //panelExecute.add(buttonSettings);

		panelExecute.add(new javax.swing.JLabel("use %f for filename; %u for URI"));
		commandField.setText(io.getCommand());
		panelExecute.add(commandField);
		panelLeft.add(panelExecute);

		javax.swing.JPanel panelWD = new javax.swing.JPanel();
		//panelWD.setLayout(new javax.swing.BoxLayout(panelWD, javax.swing.BoxLayout.Y_AXIS));
		panelWD.setLayout(new javax.swing.BoxLayout(panelWD, javax.swing.BoxLayout.X_AXIS));

		panelWD.setBorder(new javax.swing.border.TitledBorder("Working Directory"));
		panelWD.setAutoscrolls(true);
		panelWD.setMaximumSize(new java.awt.Dimension(2147483647, 2147483647));
		panelWD.setPreferredSize(new java.awt.Dimension(150, 52));
		dirField.setText(io.getDirectory());
		panelWD.add(dirField);

		javax.swing.JButton wdButtonBrowse = new javax.swing.JButton();
		wdButtonBrowse.setText("Browse");
		wdButtonBrowse.addActionListener(new ActionChooseWD(this));
		panelWD.add(wdButtonBrowse);

		panelLeft.add(panelWD);

        getContentPane().add(panelLeft);

        panelQuit.setLayout(new javax.swing.BoxLayout(panelQuit, javax.swing.BoxLayout.Y_AXIS));

        buttonOk.setText("Ok");
        buttonOk.setToolTipText("null");
        buttonOk.setAlignmentY(0.0F);
        buttonOk.setMaximumSize(new java.awt.Dimension(65, 23));
        buttonOk.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonOkActionPerformed(evt);
            }
        });
        getRootPane().setDefaultButton(buttonOk);



        buttonCancel.setText("Cancel");
        buttonCancel.setAlignmentY(0.0F);
        buttonCancel.addActionListener(new ActionListener() {
        	public void actionPerformed(java.awt.event.ActionEvent evt) {
        		buttonQuitActionPerformed(evt);
        	}
        });
        
        panelQuit.add(buttonOk);
        panelQuit.add(buttonCancel);
        panelQuit.add(Box.createVerticalGlue());

        getContentPane().add(panelQuit);

        pack();
    }
	

	
	public void buttonOkActionPerformed(ActionEvent arg0) {
		String oldFileName = io.getFilename();
		
		String newFileName = textFileName.getText();
		
		history.addEditIO(io, io.getName(), oldFileName, io.getName(), newFileName);
		
		io.setFilename(textFileName.getText());
		io.setRemoteExecute(checkBoxExecute.getModel().isSelected());
		io.setCommand(commandField.getText());
		io.setDirectory(dirField.getText());
		dispose();
	}
	
		
	public void buttonQuitActionPerformed(ActionEvent arg0) {
		dispose();
	}
	
	public class ActionChooseFile implements ActionListener{
		JDialog parentFrame;
		ExtensionFileFilter filenameFilter;
		
		public ActionChooseFile(JDialog _parentFrame){
			parentFrame = _parentFrame;
			filenameFilter = new ExtensionFileFilter("dae", "COLLADA file");
		}
		
		public void actionPerformed(ActionEvent arg0) {
			
			JFileChooser fc = new JFileChooser();
			fc.setFileFilter(filenameFilter);
			File currentFileChooserDirectory = new File(Refinery.instance.preferences.getValue("GUI_DAEDIR"));
			if(currentFileChooserDirectory.exists()){
				fc.setCurrentDirectory(currentFileChooserDirectory);
			}
			fc.setSelectedFile(new File(io.getFilename()));
			int returnVal = fc.showSaveDialog(parentFrame);
			if (returnVal == JFileChooser.APPROVE_OPTION) {
	            File file = fc.getSelectedFile();
	            //io.setFilename(file.getPath());
	            //textFileName.setText(io.getFilename());
				textFileName.setText(file.getPath());
	            Refinery.instance.preferences.setValue("GUI_DAEDIR", fc.getCurrentDirectory().getPath());
			}
			
		}
		
	}

	public class ActionChooseWD implements ActionListener
	{
		JDialog parentFrame;
		
		public ActionChooseWD(JDialog _parentFrame)
		{
			parentFrame = _parentFrame;
		}

		public void actionPerformed(ActionEvent arg0)
		{

			JFileChooser fc = new JFileChooser();
			File currentFileChooserDirectory = new File(Refinery.instance.preferences.getValue("GUI_DAEDIR"));
			fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			if (currentFileChooserDirectory.exists())
			{
				fc.setCurrentDirectory(currentFileChooserDirectory);
			}
			fc.setSelectedFile(new File(io.getDirectory()));
			int returnVal = fc.showDialog(parentFrame, "Select Directory");
			if (returnVal == JFileChooser.APPROVE_OPTION)
			{
				File file = fc.getSelectedFile();
				//io.setFilename(file.getPath());
				//textFileName.setText(io.getFilename());
				dirField.setText(file.getPath());
				//Refinery.instance.preferences.setValue("GUI_DAEDIR", fc.getCurrentDirectory().getPath());
			}

		}

	}
	
	private class ActionSettings implements ActionListener{
		JDialog parent;
    	public ActionSettings(JDialog _parent){
    		parent = _parent;
    	}
    	
		public void actionPerformed(ActionEvent e) {
			PreferencesWindow pref = new PreferencesWindow(parent); 
			pref.selectRemoteTab();
			pref.start();
		}
    }
}
