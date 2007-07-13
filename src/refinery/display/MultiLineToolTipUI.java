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
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Toolkit;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.Enumeration;
import java.util.Vector;

import javax.swing.JComponent;
import javax.swing.JToolTip;
import javax.swing.SwingUtilities;
import javax.swing.plaf.metal.MetalToolTipUI;

public class MultiLineToolTipUI extends MetalToolTipUI {
	private String[] strs;
	  private int maxWidth = 0;

	  public void paint(Graphics g, JComponent c) {

	    FontMetrics metrics = c.getFontMetrics(g.getFont());
	    Dimension size = c.getSize();
	    g.setColor(c.getBackground());
	    g.fillRect(0, 0, size.width, size.height);
	    g.setColor(c.getForeground());
	    if (strs != null) {
	      for (int i=0;i<strs.length;i++) {
		g.drawString(strs[i], 3, (metrics.getHeight()) * (i+1));
	      }
	    }
	  }

	  public Dimension getPreferredSize(JComponent c) {

	    FontMetrics metrics = c.getFontMetrics(c.getFont());
	    String tipText = ((JToolTip)c).getTipText();
	    if (tipText == null) {
	      tipText = "";
	    }
	    BufferedReader br = new BufferedReader(new StringReader(tipText));
	    String line;
	    int maxWidth = 0;
	    Vector<String> v = new Vector<String>();
	    try {
	      while ((line = br.readLine()) != null) {
		int width = SwingUtilities.computeStringWidth(metrics,line);
		maxWidth = (maxWidth < width) ? width : maxWidth;
		v.addElement(line);
	      }
	    } catch (IOException ex) {
	      ex.printStackTrace();
	    }   
	    int lines = v.size();
	    if (lines < 1) {
	      strs = null;
	      lines = 1;
	    } else {
	      strs = new String[lines];
	      int i=0;
	      for (Enumeration e = v.elements(); e.hasMoreElements() ;i++) {
	        strs[i] = (String)e.nextElement();
	      }
	    }
	    int height = metrics.getHeight() * lines;
	    this.maxWidth = maxWidth;
	    return new Dimension(maxWidth + 6, height + 4);
	  }
}
