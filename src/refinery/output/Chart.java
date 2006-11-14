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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

/** The <code>Chart</code> class provides a simple Chart construction for plugins to output their 
 * results as charts. It is displayed with profiles on several tab, each one having several groups
 * and several Values for each file in each group.
 * The Chart is first built internally with methods startProfile, startGroup and addElement. 
 * Then it is displayed with display.
 * 
 * @author PDavid
 *
 */
public class Chart extends JFrame {
	private static final long serialVersionUID = -5479771670723787865L;
	private String name;
	private Vector<Profile> profiles;
	private Profile currentProfile = null;
	private Group currentGroup = null;

	public Chart(String _name){
		name = _name;
		profiles = new Vector<Profile> ();
		setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
	}
	
	public void startProfile(String profileName){
		currentProfile = new Profile(profileName);
		profiles.add(currentProfile);
	}
	
	/**
	 * Start a group in the current profile
	 */
	public void startGroup(String groupName){
		currentGroup = new Group(groupName);
		currentProfile.addGroup(currentGroup);
	}
	
	/**
	 * Add an element in the current group
	 */
	public void addElement(String name, int [] values){
		Element element = new Element(name);
		currentGroup.elements.add(element);
		for (int i = 0; i < values.length; i++){
			element.addValue(values[i]);
		}
	}
	
	/**
	 *  Display the chart built so far.
	 */ 
	public void display(){
		setTitle(name);
		JTabbedPane tabbedPane = new JTabbedPane();

		Iterator<Profile> profilesIt = profiles.iterator();
		while(profilesIt.hasNext()){
			Profile profile = profilesIt.next();
			JPanel profilePanel = new JPanel();
			profilePanel.setLayout(new BoxLayout(profilePanel, BoxLayout.PAGE_AXIS));
			
			Iterator<Group> groupsIt = profile.getGroups();
			while(groupsIt.hasNext()){
				Group group = groupsIt.next();
				JPanel groupPanel = new JPanel();
				groupPanel.setBorder(BorderFactory.createTitledBorder(group.name));
				
				groupPanel.add(new ChartCanvas(group));
				profilePanel.add(groupPanel);
			}
			
			
			tabbedPane.add(profile.name, profilePanel);
		}
		
		add(tabbedPane);
		pack();
		setVisible(true);
	}
	
	/**
	 * The Profile nested class keeps its name and current groups it has.
	 */
	private class Profile{
		String name;
		Vector<Group> groups;
		
		public Profile(String _name){
			name = _name;
			groups = new Vector<Group>();
		}
		
		public void addGroup(Group newGroup){
			groups.add(newGroup);
		}
		
		public Iterator<Group> getGroups(){
			return groups.iterator();
		}
	}
	
	
	/**
	 * A Group nested class keeps its name and current elements it has. 
	 */
	private class Group{
		String name;
		Vector<Element> elements;
		
		public Group(String _name){
			name = _name;
			elements = new Vector<Element>();
		}
		
		public Iterator<Element> getElements(){
			return elements.iterator();
		}
	}
	
	private class Element{
		String name;
		Vector<Integer> values;
		
		public Element(String _name){
			name = _name;
			values = new Vector<Integer>();
		}
		
		public void addValue(Integer newVal){
			values.add(newVal);
		}
		
		public void addValue(int newVal){
			addValue(new Integer(newVal));
		}
		
		public String getName(){
			return name;
		}
		
		public Iterator<Integer> getValues(){
			return values.iterator();
		}
		
		public int getMaxValue(){
			int max = 0;
			Iterator<Integer> valuesIt = values.iterator();
			while(valuesIt.hasNext()){
				max = Math.max(max, valuesIt.next().intValue());
			}
			return max;
		}
	}
	
	/**
	 * The ChartCanvas class is the area on which the graphical result is displayed
	 */
	private class ChartCanvas extends JPanel {
		private static final long serialVersionUID = -318972987417566703L;
		Group group;
		
		public ChartCanvas(Group _group){
			group = _group;
			setPreferredSize(new Dimension(400, 200));
		}

		@Override
		public void paintComponent (Graphics g) {
			Iterator<Element> elementsIt = group.getElements();
			int x = 10;
			while(elementsIt.hasNext()){
				Element element = elementsIt.next();
				g.drawString(element.name, x, 130);
				float scale = 100 / (float) element.getMaxValue();
				
				for (int i = 0; i < element.values.size(); i++){
					int height = (int) (element.values.get(i) * scale);
					g.setColor(Color.RED);
					g.fillRect(x, 115 - height, 20, height );
					g.setColor(Color.BLACK);
					g.drawRect(x, 115 - height, 20, height );
					g.drawString(element.values.get(i).toString(), x, 110-height);
					
					x += 20;
				}
				
				x += 40;
			}
			
			
		}

		@Override
		public void update(Graphics g) {
			paint(g);
		}
		
		
	}
}
