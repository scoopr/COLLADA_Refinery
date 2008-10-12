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
