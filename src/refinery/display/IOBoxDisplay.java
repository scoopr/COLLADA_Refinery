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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.JFrame;

import refinery.Refinery;
import refinery.document.Anchor;
import refinery.document.Box;
import refinery.document.IOBox;
import refinery.document.IOBox.IOBoxType;
import refinery.history.ActionHistory;

import java.io.*;
import java.util.*;
import java.util.zip.*;

/**
 * The <code>IOBoxDisplay</code> class is the class implementing Display for IOBox
 * 
 * @author Philippe David
 */
public class IOBoxDisplay extends Display{
	private IOBox io;
	BufferedImage sprite;
	
	private int x;
	private int y;
	private int height;
	private int width;
	private boolean selected;
	private int xClicked;
	private int yClicked;
	private Polygon triangle;
	private Ellipse2D.Double anchorDisplay;
	private Anchor anchor;
	private boolean isAnchorHighlighted;
	
	
//	private final int GRID_WIDTH = 30;
//	private final int GRID_HEIGHT = 30;
	
	private final int ANCHORWIDTH = 11;
	private final int ANCHORHEIGHT = 11;
	
	private ExtensionFileFilter filenameFilter;
	
	public IOBoxDisplay(IOBox _parent, ActionHistory h){
		super(Display.RefererType.IO, h);
		
		io = _parent;
		x = 10;
		y = 10;
		
		if(io.getType() == IOBox.IOBoxType.OUTPUTBOX){
			x = 10;
			y = 500;
		}
		width = 100;
		height = 90;
		selected = false;
		isAnchorHighlighted = false;
		
		sprite = new BufferedImage(width,height, BufferedImage.TYPE_INT_ARGB);
		
		triangle = new Polygon();
		if (io.getType() == IOBoxType.INPUTBOX){
			triangle.addPoint(0, 0);
			triangle.addPoint(width, 0);
			triangle.addPoint(width/2, height-11);
			anchorDisplay = new Ellipse2D.Double(width/2-4,height-14, ANCHORWIDTH, ANCHORHEIGHT);
		} else {
			triangle.addPoint(0, height-1);
			triangle.addPoint(width, height-1);
			triangle.addPoint(width/2, 10);
			anchorDisplay = new Ellipse2D.Double(width/2-4,6, ANCHORWIDTH, ANCHORHEIGHT);
		}
		
		if (io.getType() == IOBoxType.OUTPUTBOX){
			anchor = io.getInputAnchor(0);
		} else {
			anchor = io.getOutputAnchor(0);
		}
		
		filenameFilter = new ExtensionFileFilter("dae", "COLLADA file");
		
	}
	
	public Box getReferer(){
		return io;
	}
	
	public Dimension getSize() {
		return new Dimension(width,height);
	}
	
	public int getX(){
		return x;
	}
	
	public int getY(){
		return y;
	}
	
	public Dimension getDimension(){
		return new Dimension(width, height);
	}
	
	public void setPosition(Point pos){
		int oldX = x;
		int oldY = y;
		//TODO check we are still inside the box maybe
		x = pos.x;
		y = pos.y;
		history.addDisplayMove(this, oldX, oldY, x, y);
	}
	
	public BufferedImage getSprite() {
		return sprite;
	}
	
	public void repaint(){
		sprite = new BufferedImage(width,height, BufferedImage.TYPE_INT_ARGB);
		Graphics2D gg = sprite.createGraphics();
		
		gg.setRenderingHint(RenderingHints.KEY_ANTIALIASING,  RenderingHints.VALUE_ANTIALIAS_ON);
		
		gg.setColor(new Color(0,0,0,0));
		gg.fill(new Rectangle(0,0,width,height));
		
		gg.setColor(new Color(152, 220, 160, 150));
		gg.fill(triangle);
		if (selected) {
			gg.setColor(Color.red);
		} else {
			gg.setColor(Color.black);
		}
		gg.draw(triangle);
		
		gg.setColor(Color.black);
		if (io.getType() == IOBoxType.INPUTBOX){
			gg.drawString(io.toString(), 33, 20);
		} else {
			gg.drawString(io.toString(), 30, 75);
		}
		if (isAnchorHighlighted) {
			gg.setColor(Color.red);
		}
		gg.fill(anchorDisplay);
		
	}
	
	public boolean contains(int mx, int my){
		return triangle.contains(mx-x, my-y) || anchorDisplay.contains(mx-x, my-y);
	}
	
	public boolean intersects(Rectangle rect){
		Rectangle rectIntersect = (Rectangle) rect.clone();
		rectIntersect.translate(-x, -y);
		return triangle.intersects(rectIntersect);
	}

	@Override
	public Point getPosition() {
		return new Point(x, y);
	}

	@Override
	public void drawOn(Graphics2D big, ImageObserver io) {
		big.drawImage(getSprite(), x, y, io);
		
	}
	
	public Anchor getAnchorAt(int mx, int my){
		if(anchorDisplay.contains(mx-x, my-y)){
			return anchor;
		} else {
			return null;
		}
	}
	
	public Point getAnchorPosition(Anchor a){
		if (io.getType() == IOBoxType.INPUTBOX){
			if (a == io.getOutputAnchor(0)){
				return new Point (x + width/2, y + height-10);
			}
		} else {
			if (a == io.getInputAnchor(0)){
				return new Point (x+ width/2, y + 10);
			}
		}
		return null;
	}
	
	
	public void select(){
		if (selected == false){
			selected = true;
			repaint();
		}
	}
	
	public void select(int _x, int _y){
		select();
		
		xClicked = _x - x;
		yClicked = _y - y;
	}

	public void deSelect() {
		selected = false;
		repaint();
	}

	public void mouseMove(int _x, int _y, Dimension box) {
		int oldX = x;
		int oldY = y;
		
		int newX = _x - xClicked;
		int newY = _y - yClicked;
		
		// Move on the grid only
		//newX -= newX%GRID_WIDTH;
		//newY -= newY%GRID_HEIGHT;

		if ( newX >= 0 && newX + width <= box.width){
			x = newX;
		}
		
		if ( newY >= 0 && newY + height <= box.height){
			y = newY;
		}
		
		history.addDisplayMove(this, oldX, oldY, newX, newY);
	}

	public void displayPropertyWindows(JFrame parentWindow) {
		if (io.getType() == IOBoxType.INPUTBOX){
			JFileChooser fc = new JFileChooser();
			File currentFileChooserDirectory = new File(Refinery.instance.preferences.getValue("GUI_DAEDIR"));
			if(currentFileChooserDirectory.exists()){
				fc.setCurrentDirectory(currentFileChooserDirectory);
			}
			fc.setFileFilter(filenameFilter);
			fc.setSelectedFile(new File(io.getFilename()));
			int returnVal = fc.showOpenDialog(parentWindow);
			if (returnVal == JFileChooser.APPROVE_OPTION) {
	            File file = fc.getSelectedFile();
	            String name = file.getPath();
	            if (name.endsWith(".kmz") == true) {
	            	// unzip the kmz file here
	            	io.setFilename(unzipFile(file.getPath()));
	            } else 
	            	io.setFilename(file.getPath());
	            Refinery.instance.preferences.setValue("GUI_DAEDIR", fc.getCurrentDirectory().getPath());
			}
			
			
		} else { 
			new OutputBoxPropertyWindow(parentWindow, io, history);
		}
		
	}

	@Override
	public void highlightLinkEnd(Anchor linkStart, MouseEvent event) {
		if(linkStart.getType() != anchor.getType()){
			if(!isAnchorHighlighted){
				isAnchorHighlighted = true;
				repaint();
			}
		}
		
	}

	@Override
	public void unHighlight() {
		if(isAnchorHighlighted){
			isAnchorHighlighted = false;
			repaint();
		}
	}
	
    public void copyInputStream(InputStream in, OutputStream out) throws IOException
	{
	    byte[] buffer = new byte[1024];
	    int len;
	
	    while((len = in.read(buffer)) >= 0)
	      out.write(buffer, 0, len);
	
	    in.close();
	    out.close();
	}
    
    public String unzipFile(String FileName) {
        Enumeration entries;
        ZipFile zipFile;
        String dataFile = null;
        String newFile = null;
        
        //System.out.println("unzip file name: " + FileName);
        File file1 = new File(FileName);
        String parent = file1.getParent();
        //System.out.println("unzip file path: " + parent);
        
        try {
          zipFile = new ZipFile(FileName);

          entries = zipFile.entries();

          while(entries.hasMoreElements()) {
            ZipEntry entry = (ZipEntry)entries.nextElement();

            if(entry.isDirectory()) {
              // Assume directories are stored parents first then children.
              System.err.println("Extracting directory: " + entry.getName());
              // This is not robust, just for demonstration purposes.
              (new File(entry.getName())).mkdir();
              continue;
            }

            System.err.println("Extracting file: " + entry.getName());
 
            File file = new File(entry.getName());
            String parentPath = file.getParent();
    	    System.err.println("parentpath: " + parentPath);
    	    if (parentPath != null) 
    		{
    			//String curDir = System.getProperty("user.dir");
    			//System.err.println("current dir: " + curDir);
    			//String newPath = curDir + File.separator + "dae_data" + File.separator + parentPath;
    	    	String newPath = parent + File.separator + parentPath;
    	    	System.err.println("new path: " + newPath);
    			(new File(newPath)).mkdir();
    			//newFile = curDir + File.separator + "dae_data" + File.separator + entry.getName();
    		}
    		newFile = parent + File.separator + entry.getName();
    		System.err.println("new file: " + newFile);
    	    copyInputStream(zipFile.getInputStream(entry), new BufferedOutputStream(new FileOutputStream(newFile)));
 
            if (entry.getName().endsWith(".dae") == true) {
                dataFile = new String(newFile);
            }
          }

          zipFile.close();
        } catch (IOException ioe) {
          System.err.println("Unhandled exception:");
          ioe.printStackTrace();
          return dataFile;
        }
        //System.out.println("return dae file: " + dataFile);
        return dataFile;
      }   

}
