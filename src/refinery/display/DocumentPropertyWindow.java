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

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.SpinnerNumberModel;
import javax.swing.border.Border;

import refinery.document.Pipeline;

public class DocumentPropertyWindow extends JFrame {
	private static final long serialVersionUID = 8501891588561352332L;
	
	private Pipeline document;
	
	private javax.swing.JSpinner HeightSpinner;
    private javax.swing.JPanel buttonPanel;
    private javax.swing.JButton cancelButton;
    private javax.swing.JLabel descriptionLabel;
    private javax.swing.JPanel descriptionPanel;
    private javax.swing.JEditorPane descriptionText;
    private javax.swing.JLabel heightLabel;
    private javax.swing.JPanel leftPanel;
    private javax.swing.JLabel nameLabel;
    private javax.swing.JPanel namePanel;
    private javax.swing.JTextField nameText;
    private javax.swing.JButton okButton;
    private javax.swing.JPanel sizePanel;
    private javax.swing.JLabel widthLabel;
    private javax.swing.JSpinner widthSpinner;
    private Border border;
    private SpinnerNumberModel widthSpinModel;
    private SpinnerNumberModel heightSpinModel;
    
    public DocumentPropertyWindow(Pipeline _document){
    	document = _document;
    	leftPanel = new javax.swing.JPanel();
        namePanel = new javax.swing.JPanel();
        nameLabel = new javax.swing.JLabel();
        nameText = new javax.swing.JTextField();
        descriptionPanel = new javax.swing.JPanel();
        descriptionLabel = new javax.swing.JLabel();
        descriptionText = new javax.swing.JEditorPane();
        sizePanel = new javax.swing.JPanel();
        widthLabel = new javax.swing.JLabel();
        widthSpinModel = new SpinnerNumberModel(new Double(document.getDisplayArea().getSize().getWidth()).intValue(), new Integer(0), null, new Integer(10)); 
        heightSpinModel = new SpinnerNumberModel(new Double(document.getDisplayArea().getSize().getHeight()).intValue(), new Integer(0), null, new Integer(10));
        widthSpinner = new javax.swing.JSpinner(widthSpinModel);
        heightLabel = new javax.swing.JLabel();
        HeightSpinner = new javax.swing.JSpinner(heightSpinModel);
        buttonPanel = new javax.swing.JPanel();
        okButton = new javax.swing.JButton();
        cancelButton = new javax.swing.JButton();
        border = BorderFactory.createEmptyBorder(5,5,5,5);

        setTitle("Document properties");
        getContentPane().setLayout(new javax.swing.BoxLayout(getContentPane(), javax.swing.BoxLayout.X_AXIS));

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        leftPanel.setLayout(new javax.swing.BoxLayout(leftPanel, javax.swing.BoxLayout.Y_AXIS));
        leftPanel.setBorder(border);

        namePanel.setLayout(new java.awt.BorderLayout());
        namePanel.setBorder(border);
        
        nameLabel.setText("Name");
        namePanel.add(nameLabel, java.awt.BorderLayout.LINE_START);

        nameText.setText(document.getFullName());
        namePanel.add(nameText, java.awt.BorderLayout.CENTER);

        leftPanel.add(namePanel);

        descriptionPanel.setLayout(new java.awt.BorderLayout());
        descriptionPanel.setBorder(border);
        descriptionLabel.setText("Description");
        descriptionPanel.add(descriptionLabel, java.awt.BorderLayout.LINE_START);

        descriptionText.setText(document.getDescription());
        descriptionText.setPreferredSize(new Dimension(100, 75));
        descriptionText.setMaximumSize(new Dimension(100, 75));
        descriptionPanel.add(descriptionText, java.awt.BorderLayout.CENTER);

        leftPanel.add(descriptionPanel);

        sizePanel.setBorder(border);
        widthLabel.setText("Width");
        sizePanel.add(widthLabel);
        widthSpinner.setPreferredSize(new java.awt.Dimension(50, 18));
        sizePanel.add(widthSpinner);

        heightLabel.setText("Height");
        sizePanel.add(heightLabel);
        HeightSpinner.setPreferredSize(new java.awt.Dimension(50, 18));
        sizePanel.add(HeightSpinner);

        leftPanel.add(sizePanel);
        leftPanel.add(javax.swing.Box.createVerticalGlue());

        getContentPane().add(leftPanel);

        buttonPanel.setLayout(new javax.swing.BoxLayout(buttonPanel, javax.swing.BoxLayout.Y_AXIS));
        buttonPanel.setBorder(border);
        
        okButton.setText("Ok");
        okButton.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				save();
				dispose();
			}
        });
        buttonPanel.add(okButton);
        getRootPane().setDefaultButton(okButton);

        cancelButton.setText("Cancel");
        cancelButton.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				dispose();
			}
        	
        });
        buttonPanel.add(cancelButton);

        getContentPane().add(buttonPanel);
        setMinimumSize(new Dimension(350, 125));
        setPreferredSize(new Dimension(450, 170));
        setResizable(false);

        pack();
        setVisible(true);
    }
    
    private void save(){
    	document.setFullName(nameText.getText());
    	document.setDescription(descriptionText.getText());
    	document.getDisplayArea().setDisplaySize(new Dimension(widthSpinModel.getNumber().intValue(), heightSpinModel.getNumber().intValue()));
    }
}
