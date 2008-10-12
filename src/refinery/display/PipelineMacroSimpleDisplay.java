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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.geom.AffineTransform;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JFrame;

import refinery.document.Anchor;
import refinery.document.PipelineMacro;
import refinery.document.Anchor.AnchorType;
import refinery.history.ActionHistory;

/**
 * The <code>PipelineMacroSimpleDisplay</code> class is the class implementing Display for Macros when rendered as a block only
 * in a document.
 * @author Philippe David
 *
 */

public class PipelineMacroSimpleDisplay extends Display {
	private PipelineMacro macro;
	private BufferedImage sprite;
	private JFrame detailWindow;
	
	private int x;
	private int y;
	private int height;
	private int width;
	private boolean selected;
	
	private int xClicked;
	private int yClicked;
	
	private Rectangle rectangle;
	private Vector<Ellipse2D.Double> inputAnchorDisplays;
	private Vector<Ellipse2D.Double> outputAnchorDisplays;
	private Anchor highlightedAnchor;
	
	private final int SPACE_BEETWEEN_ANCHORS = 20; 
	private final int ANCHORWIDTH = 11;
	private final int ANCHORHEIGHT = 11;

	public PipelineMacroSimpleDisplay(PipelineMacro _parent, ActionHistory h){
		super(Display.RefererType.MACRO, h);
		
		macro = _parent;
		detailWindow = null;
		
		x = 10;
		y = 10;
		width = 130;
		height = 80;
		selected = false;
		
		sprite = new BufferedImage(width+20, height+20, BufferedImage.TYPE_INT_ARGB);
		rectangle = new Rectangle(10,10,width,height-20);
//		inputAnchorDisplay = new Ellipse2D.Double(30-ANCHORWIDTH/2, 10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT);
//		outputAnchorDisplay = new Ellipse2D.Double(30-ANCHORWIDTH/2, height-10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT);
		
		// We should not change the number of inputs and output in the macro once it is loaded so we can create the anchor displays now
		inputAnchorDisplays = new Vector<Ellipse2D.Double>();
		int inputNumber = macro.getInputNumber();
		for(int i=0; i < inputNumber; i++){
			int xPosition = getAnchorMiddleX(i);
			inputAnchorDisplays.add(new Ellipse2D.Double(xPosition-ANCHORWIDTH/2, 10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT));
		}
		
		outputAnchorDisplays = new Vector<Ellipse2D.Double>();
		int outputNumber = macro.getOutputNumber();
		for(int i=0; i < outputNumber; i++){
			int xPosition = getAnchorMiddleX(i);
			outputAnchorDisplays.add(new Ellipse2D.Double(xPosition-ANCHORWIDTH/2, height-10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT));
		}
		
		repaint();
	}
	
	public Point getPosition() {
		return new Point(x, y);
	}
	
	public int getX(){
		return x;
	}
	
	public int getY(){
		return y;
	}

	public Dimension getSize() {
		return new Dimension(width, height);
	}
	
	public BufferedImage getSprite() {
		return sprite;
	}

	public void drawOn(Graphics2D big, ImageObserver io) {
		big.drawImage(getSprite(), x, y, io);
	}

	public void select() {
		if (selected == false){
			selected = true;
			repaint();
		}
	}

	public void select(int _x, int _y) {
		select();
		
		xClicked = _x - x;
		yClicked = _y - y;
	}

	@Override
	public void deSelect() {
		selected = false;
		repaint();
	}

	/**
	 * Returns true if the point (mx, my) is inside the display, false otherwise
	 */
	public boolean contains(int mx, int my){
		if(rectangle.contains(mx-x, my-y)){
			return true;
		}
		for(int i = 0; i < inputAnchorDisplays.size(); i++){
			if(inputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return true;
			}
		}
		for(int i = 0; i < outputAnchorDisplays.size(); i++){
			if(outputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Returns true if the rectangle rect intersects with this diplay, false otherwise
	 */
	public boolean intersects(Rectangle rect){
		Rectangle rectIntersect = (Rectangle) rect.clone();
		rectIntersect.translate(-x, -y);
		return rectangle.intersects(rectIntersect);
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

	public void setPosition(Point pos){
		int oldX = x;
		int oldY = y;
		//TODO check we are still inside the box maybe
		x = pos.x;
		y = pos.y;
		history.addDisplayMove(this, oldX, oldY, x, y);
	}

	public Anchor getAnchorAt(int mx, int my){
		// top anchors
		for(int i= 0; i < inputAnchorDisplays.size(); i++){
			if (inputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return macro.getInputAnchor(i);
			}
		}
		
		// bottom anchors
		for(int i= 0; i < outputAnchorDisplays.size(); i++){
			if (outputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return macro.getOutputAnchor(i);
			}
		}
		
		return null;
	}
	
	public Point getAnchorPosition(Anchor a){
		if(a.type == AnchorType.ANCHORIN){
			for(int i = 0; i < macro.getInputNumber(); i++){
				if(macro.getInputAnchor(i) == a) return new Point(x + getAnchorMiddleX(i), y + 10);
			}
		} else {
			for(int i = 0; i < macro.getOutputNumber(); i++){
				if(macro.getOutputAnchor(i) == a) return new Point(x + getAnchorMiddleX(i), y + height - 10);
			}
		}
		
		return null;
	}
	
	public void repaint(){

		sprite = new BufferedImage(width+20, height+20, BufferedImage.TYPE_INT_ARGB);
		Graphics2D gg = sprite.createGraphics();
		gg.setRenderingHint(RenderingHints.KEY_ANTIALIASING,  RenderingHints.VALUE_ANTIALIAS_ON);
		
		gg.setColor(new Color(0,0,0,0));
		gg.fill(new Rectangle(0,0,width+20,height+20));
		
		gg.setColor(new Color(223, 186, 149, 150));
		gg.fill(rectangle);
		if (selected) {
			gg.setColor(Color.red);
		} else {
			gg.setColor(Color.black);
		}
		gg.draw(rectangle);
		
		gg.setColor(Color.black);
		
		gg.drawString("MACRO", 50, 35);
		gg.drawString(macro.getFullName(), 50, 55);
		
		// Top anchors
		for(int i= 0; i < inputAnchorDisplays.size(); i++){
			
			if (macro.getInputAnchor(i) == highlightedAnchor){
				gg.setColor(Color.RED);
			}
			gg.fill(inputAnchorDisplays.get(i));
			gg.setColor(Color.black);
		}
		
		// Bottom anchor
		for(int i= 0; i < outputAnchorDisplays.size(); i++){
			
			if (macro.getOutputAnchor(i) == highlightedAnchor){
				gg.setColor(Color.RED);
			}
			gg.fill(outputAnchorDisplays.get(i));
			gg.setColor(Color.black);
		}
		
		// Add a cool icon
		Image inImage = new ImageIcon("add.png").getImage();
		AffineTransform tx =  new AffineTransform();
		tx.translate(20, 30);
		gg.drawImage(inImage, tx, null);
		
	}
	
	/**
	 * @return The X component of the center where the Anchor anchorNum is placed taking in account 
	 * the total number of Input Anchors.
	 */
	private int getAnchorMiddleX(int anchorNum){
		return (30 + anchorNum*SPACE_BEETWEEN_ANCHORS);
	}

	@Override
	public void displayPropertyWindows(JFrame parentWindow) {
		if(detailWindow == null){
			detailWindow = new JFrame("MACRO : " + macro.getFullName());
			detailWindow.add(new DisplayAreaNonEditable(macro, detailWindow, macro.getRefinery(), macro.getRefinery().getGUI()));
			detailWindow.pack();
		}
		detailWindow.setVisible(true);
	}

	@Override
	public Object getReferer() {
		return macro;
	}

	@Override
	public void highlightLinkEnd(Anchor linkStart, MouseEvent event) {
		Anchor newHighlightedAnchor = null;
		Anchor a = getAnchorAt(event.getX(), event.getY());
		if (a != null && a.getType() != linkStart.getType() && a.getReferer() != linkStart.getReferer()) {
			newHighlightedAnchor = a;
		} else {
			AnchorType endType = linkStart.getType() == AnchorType.ANCHORIN ? AnchorType.ANCHOROUT : AnchorType.ANCHORIN;
			newHighlightedAnchor = macro.getLinkEndAnchor(endType);
		}
		if (newHighlightedAnchor != highlightedAnchor){
			highlightedAnchor = newHighlightedAnchor;
			repaint();
		}
		
	}

	@Override
	public void unHighlight() {
		if (highlightedAnchor != null){
			highlightedAnchor = null;
			repaint();
		}
	}

}
