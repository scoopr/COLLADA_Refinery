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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.border.Border;

import org.xml.sax.SAXParseException;

import refinery.AvailableConditionersList;
import refinery.AvailableMacrosList;
import refinery.ConditionerTemplate;
import refinery.ExecuteDocumentThread;
import refinery.MacroTemplate;
import refinery.PreferencesWindow;
import refinery.Refinery;
import refinery.document.ConditionerWrapper;
import refinery.document.ExecuteException;
import refinery.document.IOBox;
import refinery.document.Link;
import refinery.document.Pipeline;
import refinery.document.PipelineMacro;
import refinery.history.HistoryException;
import refinery.history.Mark;

/**
 * The <code>MainGUI</code> class draws the main gui windows with buttons, menus and add
 * a new BoxArea inside.
 * 
 * @author Philippe David
 */
public class MainGUI {
	private Refinery main;
	private Pipeline document;
	private AvailableConditionersList clist;
	private AvailableMacrosList mlist;
	private DisplayArea boxes;
	private JFrame window;
	private JLabel documentInfoLabel;
	private Border border;
	private JLabel statusBarLabel;
	
	private ActionUndo actionUndo;
	private ActionRedo actionRedo;
	private ActionDelete actionDelete;
	private ActionNew actionNew;
	private ActionOpen actionOpen;
	private ActionSave actionSave;
	private ActionSaveAs actionSaveAs;
	private ActionExecute actionExecute;
	private ActionZoomIn actionZoomIn;
	private ActionZoomOut actionZoomOut;
	private ActionPreferences actionPreferences; 
	
	private JFileChooser pipelineFileChooser;
	
	public JCheckBoxMenuItem consoleViewCheckBox;

	final private int LEFT_PANEL_MAX_X = 150;
	final private int LEFT_PANEL_MAX_Y = 350;
	
	public MainGUI(Refinery _main, AvailableConditionersList _clist, AvailableMacrosList _mlist, Pipeline _document) {
		main = _main;
		document = _document;
		clist = _clist;
		mlist = _mlist;
		
		// First we create our window
		window = new JFrame("Refinery");
		//window.setIconImage((new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/collada.gif")).getImage());
		window.setIconImage((new ImageIcon(_main.getClass().getResource("/icons/collada.gif"))).getImage());
		
		
		// Set of actions
		actionUndo = new ActionUndo(window);
		actionRedo = new ActionRedo(window);
		actionDelete = new ActionDelete();
		actionNew = new ActionNew();
		actionOpen = new ActionOpen(window);
		actionSave = new ActionSave(window);
		actionSaveAs = new ActionSaveAs(window);
		actionExecute = new ActionExecute();
		actionZoomIn = new ActionZoomIn();
		actionZoomOut = new ActionZoomOut();
		actionPreferences = new ActionPreferences(window);
		
		// The file chooser
		pipelineFileChooser = new JFileChooser();
		pipelineFileChooser.setFileFilter(new ExtensionFileFilter("pip", "Pipeline file"));
		File currentFileChooserDirectory = new File(Refinery.instance.preferences.getValue("GUI_PIPDIR"));
		if(currentFileChooserDirectory.exists()){
			pipelineFileChooser.setCurrentDirectory(currentFileChooserDirectory);
		}
		
		border = BorderFactory.createEmptyBorder(10,10,10,10);
		
		// Set of panels
		//This is the old left panel stuff
		/*JPanel leftPannel = new JPanel();
		leftPannel.setLayout(new BoxLayout(leftPannel, BoxLayout.PAGE_AXIS));
		
		JPanel panelAddConditioners = buildLibAddPanel();
		panelAddConditioners.setPreferredSize(new Dimension(200, 100));
		JScrollPane addConditionerScrollPane = new JScrollPane(panelAddConditioners);
		
		JPanel panelAddMacros = buildMacroAddPanel();
		panelAddMacros.setPreferredSize(new Dimension(200, 100));
		JScrollPane addMacrosSrollPane = new JScrollPane(panelAddMacros);
		
		JPanel panelAddIO = new JPanel();
		JButton addInputButton = new JButton("Input");
		addInputButton.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				document.addInputBox();
			}
		});
		JButton addOutputButton = new JButton("Output");
		addOutputButton.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				document.addOuputBox();
			}
		});
		panelAddIO.add(addInputButton);
		panelAddIO.add(addOutputButton);
		
		JTabbedPane tabbedPane = new JTabbedPane();

		tabbedPane.addTab("Conditioners", addConditionerScrollPane);
		tabbedPane.addTab("Macros", addMacrosSrollPane);
		tabbedPane.addTab("IO", panelAddIO);
		
		leftPannel.add(tabbedPane);*/

		//new left panel
		JPanel leftPanel = new JPanel();
		leftPanel.setPreferredSize(new Dimension(LEFT_PANEL_MAX_X, LEFT_PANEL_MAX_Y));

		leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.PAGE_AXIS));

		JPanel IOPanel = new JPanel();
		IOPanel.setLayout(new BoxLayout(IOPanel, BoxLayout.PAGE_AXIS));
		IOPanel.setBorder(BorderFactory.createTitledBorder("IO"));
		String[] IOChoices = {"IO", "Input", "Output" };
		JComboBox IOCombo = new JComboBox(IOChoices);
		IOCombo.addActionListener(new ActionAddIO());
		IOCombo.setMaximumSize(new Dimension(LEFT_PANEL_MAX_X, 1));
		IOPanel.add(IOCombo);
		IOPanel.add(Box.createVerticalGlue());
		IOPanel.setMaximumSize(new Dimension(LEFT_PANEL_MAX_X, LEFT_PANEL_MAX_Y));

		leftPanel.add(IOPanel);
		leftPanel.add(buildMacroAddPanel());
		leftPanel.add(buildLibAddPanel());
		//leftPanel.add(Box.createVerticalGlue());

		window.add(leftPanel, BorderLayout.LINE_START);
				
		JPanel centerPanel = new JPanel();
		centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.PAGE_AXIS));
		
		
		// Build the toolbar
		JToolBar toolBar = new JToolBar("Area toolbar");
		toolBar.setFloatable(false);
		//toolBar.setRollover(true);
		toolBar.setAlignmentX(Component.LEFT_ALIGNMENT);
		// Add Buttons to the tool bar
		JButton toolNew = new JButton(actionNew);
		toolNew.setToolTipText("New document");
		toolNew.setText("");
		JButton toolOpen = new JButton(actionOpen);
		toolOpen.setToolTipText("Load a pipeline from a file");
		toolOpen.setText("");
		JButton toolSave = new JButton(actionSave);
		toolSave.setToolTipText("Save this pipeline to a file");
		toolSave.setText("");
		JButton toolUndo = new JButton(actionUndo);
		toolUndo.setToolTipText("Undo last action");
		toolUndo.setText("");
		JButton toolRedo = new JButton(actionRedo);
		toolRedo.setToolTipText("Redo last action");
		toolRedo.setText("");
		JButton toolDelete = new JButton(actionDelete);
		toolDelete.setToolTipText("Delete selected element(s)");
		toolDelete.setText("");
		JButton toolExecute = new JButton(actionExecute);
		toolExecute.setToolTipText("Execute this pipeline");
		toolExecute.setText("");
		
		JButton toolZommIn = new JButton(actionZoomIn);
		
		
		toolBar.add(toolNew);
		toolBar.add(toolOpen);
		toolBar.add(toolSave);
		toolBar.addSeparator();
		toolBar.add(toolUndo);
		toolBar.add(toolRedo);
		toolBar.addSeparator();
		toolBar.add(toolDelete);
		toolBar.addSeparator();
		toolBar.add(toolExecute);
		//toolBar.add(toolZommIn);
		//toolBar.add(actionZoomOut);
		
		centerPanel.add(toolBar);
		
		
		// We add the DisplayArea
		boxes = new DisplayArea(document, window, main, this);
		JScrollPane scrollPane = new JScrollPane(boxes);
		scrollPane.setViewport(new DisplayAreaViewPort(boxes));
		scrollPane.setPreferredSize(new Dimension(750,750));
		centerPanel.add(scrollPane);
		
		// Info panel under the display area
		JPanel bottomInfosPanel = new JPanel();
		bottomInfosPanel.setLayout(new BoxLayout(bottomInfosPanel, BoxLayout.X_AXIS));
		documentInfoLabel = new JLabel(document.getFullName());
		JButton editDocumentButton = new JButton("Edit properties");
		editDocumentButton.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				new DocumentPropertyWindow(document);
			}
			
		});
		JButton executeButton = new JButton(actionExecute);
		bottomInfosPanel.add(documentInfoLabel);
		bottomInfosPanel.add(Box.createGlue());
		bottomInfosPanel.add(editDocumentButton);
		bottomInfosPanel.add(executeButton);
		bottomInfosPanel.setBorder(border);
		centerPanel.add(bottomInfosPanel);
		
		window.add(centerPanel, BorderLayout.CENTER);	
		
		// Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		// Build the first menu.
		JMenu fileMenu = new JMenu("File");
		menuBar.add(fileMenu);
		JMenuItem menuItemNew = new JMenuItem(actionNew);
		JMenuItem menuItemLoad = new JMenuItem(actionOpen);
		JMenuItem menuItemSave = new JMenuItem(actionSave);
		JMenuItem menuItemSaveAs = new JMenuItem(actionSaveAs);
		JMenuItem menuItemPreferences = new JMenuItem(actionPreferences);
		JMenuItem menuItemQuit = new JMenuItem("Quit");
		menuItemQuit.addActionListener(new ActionQuit());
		fileMenu.add(menuItemNew);
		fileMenu.add(menuItemLoad);
		fileMenu.add(menuItemSave);
		fileMenu.add(menuItemSaveAs);
		fileMenu.addSeparator();
		fileMenu.add(menuItemPreferences);
		fileMenu.addSeparator();
		fileMenu.add(menuItemQuit);
		
		// Build Edit menu
		JMenu editMenu = new JMenu("Edit");
		menuBar.add(editMenu);
		JMenuItem menuItemUndo = new JMenuItem(actionUndo);
		JMenuItem menuItemRedo = new JMenuItem(actionRedo);
		JMenuItem menuItemDelete = new JMenuItem(actionDelete);
		editMenu.add(menuItemUndo);
		editMenu.add(menuItemRedo);
		editMenu.addSeparator();
		editMenu.add(menuItemDelete);
		
		
		// Build the view menu
		JMenu viewMenu = new JMenu("View");
		menuBar.add(viewMenu);
		consoleViewCheckBox = new JCheckBoxMenuItem("Console", false);
		consoleViewCheckBox.setMnemonic(KeyEvent.VK_C);
		consoleViewCheckBox.addActionListener(new ActionShowConsole());
		
		viewMenu.add(consoleViewCheckBox);
		
		window.setJMenuBar(menuBar);
		
		statusBarLabel = new JLabel("Refinery started");
		statusBarLabel.setMinimumSize(new Dimension(34,25));
		statusBarLabel.setBorder(BorderFactory.createEtchedBorder());
		window.add(statusBarLabel, BorderLayout.PAGE_END);
		
		// Get properties from preferences
		Pattern p = Pattern.compile("^(\\d*),(\\d*)$");
		// Position
		Matcher m = p.matcher(Refinery.instance.preferences.getValue("GUI_POSITION"));
		if(m.matches()){
			window.setLocation(Integer.valueOf(m.group(1)), Integer.valueOf(m.group(2)));
		}
		
		// Display the window.
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.addWindowListener(new MainGUIWindowListener(this));
		window.pack();
		window.setVisible(true);
	}
	
	private JPanel buildLibAddPanel(){
		/*HashMap<String, JPanel> categories = new HashMap<String, JPanel>();
		JPanel panelAddConditioners = new JPanel();
		panelAddConditioners.setLayout(new BoxLayout(panelAddConditioners, BoxLayout.PAGE_AXIS));
		
		Iterator<ConditionerTemplate> cit = clist.iterator();
		while(cit.hasNext()){
			ConditionerTemplate cb = cit.next();
			String name = cb.getIDName();
			JButton cButton = new JButton(cb.getIDName());
			cButton.setToolTipText(cb.getDescription());
			cButton.addActionListener(new ActionAddConditioner(cb));
				
			String cat = cb.getCategory();
			if (cat == null || cat.equals("")) {
				cat = "No category";
			}
			JPanel pan = categories.get(cat);
			if (pan == null){
				pan = new JPanel(new FlowLayout());
				pan.setBorder(BorderFactory.createTitledBorder(cat));
				panelAddConditioners.add(pan);
				categories.put(cat, pan);
			}
			pan.setPreferredSize(new Dimension(200, 100));
			pan.add(cButton);
		}
		
		//panelAddConditioners.add(Box.createVerticalGlue());
		
		return panelAddConditioners;*/

		JPanel CPanel = new JPanel();
		CPanel.setLayout(new BoxLayout(CPanel, BoxLayout.PAGE_AXIS));
		CPanel.setBorder(BorderFactory.createTitledBorder("Conditioners"));

		HashMap<String, JComboBox> categories = new HashMap<String, JComboBox>();
		Iterator<ConditionerTemplate> cit = clist.iterator();
		while(cit.hasNext()){
			ConditionerTemplate cb = cit.next();
			String name = cb.getIDName();
			String cat = cb.getCategory();
			if (cat == null || cat.equals(""))
			{
				cat = "No category";
			}
			JComboBox box = categories.get(cat);
			if (box == null)
			{
				box = new JComboBox();
				box.addItem(cat);
				box.addActionListener(new ActionAddConditioner());
				box.setMaximumSize(new Dimension(LEFT_PANEL_MAX_X, 1));
				CPanel.add(box);
				categories.put(cat, box);
				CPanel.add(Box.createRigidArea(new Dimension(LEFT_PANEL_MAX_X, 7)));
			}
			box.addItem(name);
		}
		//CPanel.setMaximumSize(new Dimension(LEFT_PANEL_MAX_X, categories.size() *LEFT_PANEL_MAX_Y ));
		CPanel.add(Box.createVerticalGlue());
		return CPanel;
	}
	
	private JPanel buildMacroAddPanel(){
		/*HashMap<String, JPanel> categories = new HashMap<String, JPanel>();
		JPanel panelAddMacros = new JPanel();
		panelAddMacros.setLayout(new BoxLayout(panelAddMacros, BoxLayout.PAGE_AXIS));
		
		Iterator<MacroTemplate> mit = mlist.iterator();
		while(mit.hasNext()){
			MacroTemplate mb = mit.next();
			JButton mButton = new JButton(mb.getIDName());
			mButton.setToolTipText(mb.getDescription());
			mButton.addActionListener(new ActionAddMacro(mb, window));
			
			String cat = mb.getCategory();
			if (cat == null || cat.equals("")){
				cat = "No category";
			}
			JPanel pan = categories.get(cat);
			if(pan == null){
				pan = new JPanel(new FlowLayout());
				pan.setBorder(BorderFactory.createTitledBorder(cat));
				panelAddMacros.add(pan);
				categories.put(cat, pan);
			}
			pan.setPreferredSize(new Dimension(200, 100));
			pan.add(mButton);
			//generalPanel.add(mButton);
		}
		
		//panelAddMacros.add(Box.createVerticalGlue());

		return panelAddMacros;*/

		JPanel MPanel = new JPanel();
		MPanel.setLayout(new BoxLayout(MPanel, BoxLayout.PAGE_AXIS));
		MPanel.setBorder(BorderFactory.createTitledBorder("Macros"));
		JComboBox MCombo = new JComboBox();
		MCombo.addActionListener(new ActionAddMacro(window));
		MCombo.setMaximumSize(new Dimension(LEFT_PANEL_MAX_X, 1));
		MCombo.addItem("Macros");
		MPanel.add(MCombo);
		MPanel.setMaximumSize(new Dimension(LEFT_PANEL_MAX_X, LEFT_PANEL_MAX_Y));
		
		Iterator<MacroTemplate> mit = mlist.iterator();
		while (mit.hasNext())
		{
			MacroTemplate mb = mit.next();
			MCombo.addItem(mb.getIDName());
		}
		MPanel.add(Box.createVerticalGlue());
		return MPanel;
	}
	
	public void setDocument(Pipeline _document){
		document = _document;
		documentInfoLabel.setText(document.getFullName());
	}
	
	public void setStatusBarError(String text){
		statusBarLabel.setForeground(Color.red);
		statusBarLabel.setText(text);
	}
	
	public void cleanStatusBar(){
		statusBarLabel.setText(" ");
	}
	
	public JFrame getWindow(){
		return window;
	}
	
	public DisplayAreaInterface getBoxesArea()
	{
		return boxes;
	}
	
	public void updateDocumentInfos(){
		documentInfoLabel.setText(document.getFullName());
		getWindow().setTitle("Refinery - "+document.getFullName());
	}
	
	public ActionUndo getActionUndo(){
		return actionUndo;
	}
	
	public ActionRedo getActionRedo(){
		return actionRedo;
	}
	
	public ActionExecute getActionExecute(){
		return actionExecute;
	}
	
	public ActionDelete getActionDelete(){
		return actionDelete;
	}

	/**
	 * Action to execute the conditioner pipeline
	 */
	class ActionExecute extends AbstractAction{
		private static final long serialVersionUID = -3680705883192824329L;

		public ActionExecute(){
			//super("Execute", new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/Play16.gif"));
			super("Execute", new ImageIcon(main.getClass().getResource("/icons/Play16.gif")));
		}
		
		public void actionPerformed(ActionEvent arg0) {
			ExecuteDocumentThread exec = new ExecuteDocumentThread(document);
			exec.start();
			
		}
		
	}
	
	/**
	 * Action to load a pipeline file
	 */
	class ActionOpen extends AbstractAction {
		private static final long serialVersionUID = -4456274469282770981L;
		JFrame parentFrame;
		
		public ActionOpen(JFrame _parentFrame) {
			//super("Open", new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/Open16.gif"));
			super("Open", new ImageIcon(main.getClass().getResource("/icons/Open16.gif")));
			parentFrame = _parentFrame;
		}

		public void actionPerformed(ActionEvent arg0) {
			if (pipelineFileChooser.showOpenDialog(parentFrame) == JFileChooser.APPROVE_OPTION) {
				try {
					main.loadFile(pipelineFileChooser.getSelectedFile().getPath());
				} catch (SAXParseException spe) {
					//	Error generated by the parser
					JOptionPane.showMessageDialog(null, "Parsing error" + ", line " +
			                spe.getLineNumber() + ", uri " + spe.getSystemId()+"\n" + spe.getMessage(),"Error",
							    JOptionPane.ERROR_MESSAGE);

					main.newWorkSpace();
				} catch (Exception e){
					JOptionPane.showMessageDialog(null, "Loading error : "+e.getMessage(),"Error",
							    JOptionPane.ERROR_MESSAGE);
					main.newWorkSpace();
				}
			}
		}
		
	}
	
	/**
	 * Action to save the current pipeline to its current file, or ask the user to choose a file otherwise
	 */
	class ActionSave extends AbstractAction{
		private static final long serialVersionUID = 1396561529596847904L;
		JFrame parentFrame;
		
		public ActionSave(JFrame _parentFrame) {
//			super("Save", new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/Save16.gif"));
			super("Save", new ImageIcon(main.getClass().getResource("/icons/Save16.gif")));
			parentFrame = _parentFrame;
			putValue(ACCELERATOR_KEY, KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_MASK));
		}
		
		public void actionPerformed(ActionEvent arg0) {
			if(document.getSaveFileName() != null){
				try {
					document.saveToFile(document.getSaveFileName());
				} catch (Exception e) {
					e.printStackTrace();
					JOptionPane.showMessageDialog(parentFrame, "An error occured:\n" + e.getMessage(),"Error",
					    JOptionPane.ERROR_MESSAGE);
				}
			} else {
				(new ActionSaveAs(parentFrame)).actionPerformed(arg0);
			}
		}
	}
	
	/**
	 * Action to save a pipeline to a new file
	 */
	class ActionSaveAs extends AbstractAction {
		private static final long serialVersionUID = -3006198610906852701L;
		JFrame parentFrame;
		
		public ActionSaveAs(JFrame _parentFrame) {
			super("Save As");
			parentFrame = _parentFrame;
		}

		public void actionPerformed(ActionEvent arg0) {
			if (pipelineFileChooser.showSaveDialog(parentFrame) == JFileChooser.APPROVE_OPTION) {
				try {
					String path = pipelineFileChooser.getSelectedFile().getPath();
					if (!path.matches(".*\\.pip")){
						path += ".pip";
					}
					document.saveToFile(path);
				} catch (Exception e) {
					e.printStackTrace();
					JOptionPane.showMessageDialog(parentFrame, "An error occured:\n" + e.getMessage(),"Error",
					    JOptionPane.ERROR_MESSAGE);

				}
			}

		}
		
	}
	
	/**
	 * Action to delete an item in the workspace
	 */
	class ActionDelete extends AbstractAction {
		private static final long serialVersionUID = -5048682940897487824L;

		public ActionDelete(){
			//super("Delete", new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/Delete16.gif"));
			super("Delete", new ImageIcon(main.getClass().getResource("/icons/Delete16.gif")));
			putValue(ACCELERATOR_KEY, KeyStroke.getKeyStroke("DELETE"));
		}

		public void actionPerformed(ActionEvent arg0) {
			Mark historyMark = main.getDocument().getHistory().addMark();
			Iterator<Display> it = boxes.getSelectionIterator();
			
			while (it.hasNext()){
				Display d = it.next();
				switch (d.getRefererType()){
				case COND:
					document.removeConditioner((ConditionerWrapper) d.getReferer() );
					it = boxes.getSelectionIterator();
					break;
				case MACRO:
					document.removeMacro((PipelineMacro) d.getReferer() );
					it = boxes.getSelectionIterator();
					break;
				case LINK:
					document.removeLink((Link) d.getReferer());
					it = boxes.getSelectionIterator();
					break;
				case IO:
					IOBox io = (IOBox) d.getReferer();
					if (io.getType() == IOBox.IOBoxType.INPUTBOX){
						document.removeInputBox((IOBox) d.getReferer());
					} else {
						document.removeOutputBox((IOBox) d.getReferer());
					}
					it = boxes.getSelectionIterator();
					break;
				}
			}
			
			main.getDocument().getHistory().packSinceMark(historyMark);
			
		}
	}
	
	/**
	 * Action to empty the workspace
	 */
	class ActionNew extends AbstractAction {
		private static final long serialVersionUID = -578642069071954800L;
		public ActionNew(){
			//super("New", new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/New16.gif"));
			super("New", new ImageIcon(main.getClass().getResource("/icons/New16.gif")));
		}
		public void actionPerformed(ActionEvent arg0) {
			main.newWorkSpace();
			boxes.repaint();
		}
		
	}
	
	
	/**
	 * Action to refresh
	 */
	class ActionRefresh implements ActionListener {

		public void actionPerformed(ActionEvent arg0) {
			boxes.repaint();
		}
		
	}
	
	/**
	 * Action to show/hide console
	 */
	class ActionShowConsole implements ActionListener {

		public void actionPerformed(ActionEvent arg0) {
			main.setConsoleVisible(consoleViewCheckBox.getState());
		}
		
	}
	
	
	/**
	 * Action to quit application
	*/
	class ActionQuit implements ActionListener {

		public void actionPerformed(ActionEvent arg0) {
			System.exit(0);
		}
		
	}
	
	/**
	 * Action to undo last change
	*/
	class ActionUndo extends AbstractAction {
		private static final long serialVersionUID = 918305483833425182L;
		JFrame parentFrame;
		
		public ActionUndo(JFrame _parent){
//			super("Undo", new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/Undo16.gif"));
			super("Undo", new ImageIcon(main.getClass().getResource("/icons/Undo16.gif")));
			parentFrame = _parent;
			putValue(ACCELERATOR_KEY, KeyStroke.getKeyStroke(KeyEvent.VK_Z, InputEvent.CTRL_MASK));
		}

		public void actionPerformed(ActionEvent arg0) {
			try {
				document.getHistory().undo();
				boxes.repaint();
			} catch (HistoryException e) {
				JOptionPane.showMessageDialog(parentFrame, "An error occured:\n" + e.getMessage(),"Error",
					    JOptionPane.ERROR_MESSAGE);
			}
		}
		
	}
	
	/**
	 * Action to redo last undo
	*/
	class ActionRedo extends AbstractAction {
		private static final long serialVersionUID = -879145972663806935L;
		JFrame parentFrame;
		
		public ActionRedo(JFrame _parent){
			//super("Redo", new ImageIcon(Refinery.RefineryDirectory + File.separator + "icons/Redo16.gif"));
			super("Redo", new ImageIcon(main.getClass().getResource("/icons/Redo16.gif")));
			parentFrame = _parent;
			putValue(ACCELERATOR_KEY, KeyStroke.getKeyStroke(KeyEvent.VK_Y, InputEvent.CTRL_MASK));
		}

		public void actionPerformed(ActionEvent arg0) {
			try {
				document.getHistory().redo();
				boxes.repaint();
			} catch (HistoryException e) {
				JOptionPane.showMessageDialog(parentFrame, "An error occured:\n" + e.getMessage(),"Error",
					    JOptionPane.ERROR_MESSAGE);
			}
		}
		
	}
	
	/**
	 * Action to increase zoom in the display area
	 */
	class ActionZoomIn extends AbstractAction{

		public ActionZoomIn(){
			super("Zoom in");
		}
		public void actionPerformed(ActionEvent e) {
			boxes.zoomIn();
			
		}
		
	}
	
	/**
	 * Action to decrease zoom in the display area
	 */
	class ActionZoomOut extends AbstractAction{

		public ActionZoomOut(){
			super("Zoom out");
		}
		public void actionPerformed(ActionEvent e) {
			boxes.zoomOut();
			
		}
		
	}
	
	/**
	 * Action to open the preferences window
	 */
	
	class ActionPreferences extends AbstractAction{
		JFrame parent;
		
		public ActionPreferences(JFrame _parent){
			super("Preferences");
			parent = _parent;
		}
		public void actionPerformed(ActionEvent e) {
			PreferencesWindow pref = new PreferencesWindow(parent);
			pref.start();
		}
		
	}

	/**
	 * Action to add a new IO Box in the box Area
	 */
	class ActionAddIO implements ActionListener
	{
		public ActionAddIO()
		{
		}

		public void actionPerformed(ActionEvent e)
		{
			JComboBox cb = (JComboBox)e.getSource();
			String item = (String)cb.getSelectedItem();
			if (item.equals("Input"))
			{
				document.addInputBox();
			}
			else if (item.equals("Output"))
			{
				document.addOuputBox();
			}
			cb.setSelectedItem(cb.getItemAt(0));
		}

	}

	/**
	 * Action to add a new Conditioner Box in the box Area
	 */
	class ActionAddConditioner implements ActionListener{
		public ActionAddConditioner(){
		}
		
		public void actionPerformed(ActionEvent e) {
			JComboBox cb = (JComboBox)e.getSource();
			if (cb.getSelectedIndex() != 0)
			{
				String item = (String)cb.getSelectedItem();
				document.addConditioner(clist.getConditionerBase(item));
				cb.setSelectedItem(cb.getItemAt(0));
			}
		}
		
	}
	
	/**
	 * Action to add a new macro Box in the display area
	 */
	class ActionAddMacro implements ActionListener{
		private JFrame parentFrame;
		
		public ActionAddMacro(JFrame _parent){
			parentFrame = _parent;
		}
		
		public void actionPerformed(ActionEvent event){
			JComboBox cb = (JComboBox)event.getSource();
			if (cb.getSelectedIndex() != 0)
			{
				String item = (String)cb.getSelectedItem();
				cb.setSelectedItem(cb.getItemAt(0));
				try
				{
					PipelineMacro macro = document.addMacro(mlist.getMacroBaseFromLibName(item));
					macro.setSimpleDisplayArea(boxes);
				}
				catch (Exception e)
				{
					JOptionPane.showMessageDialog(parentFrame, "An error occured:\n" + e.getMessage(), "Error",
							JOptionPane.ERROR_MESSAGE);
				}
			}
		}
	}
	

	/**
	 * Window events listener
	 */
	
	private class MainGUIWindowListener implements WindowListener{
		private MainGUI gui;
		
		public MainGUIWindowListener(MainGUI _gui){
			gui = _gui;
		}

		public void windowOpened(WindowEvent e) {
			// TODO Auto-generated method stub
			
		}

		public void windowClosing(WindowEvent e) {
			String pos = e.getWindow().getX() +"," + e.getWindow().getY();
			String size = boxes.getPreferredSize().width + "," + boxes.getPreferredSize().height;
			
			Refinery.instance.preferences.setValue("GUI_POSITION", pos);
			Refinery.instance.preferences.setValue("GUI_SIZE", size);
			
			Refinery.instance.preferences.setValue("GUI_PIPDIR", pipelineFileChooser.getCurrentDirectory().getPath());

			try {
				Refinery.instance.preferences.save();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		}

		public void windowClosed(WindowEvent e) {
			// TODO Auto-generated method stub
			
		}

		public void windowIconified(WindowEvent e) {
			// TODO Auto-generated method stub
			
		}

		public void windowDeiconified(WindowEvent e) {
			// TODO Auto-generated method stub
			
		}

		public void windowActivated(WindowEvent e) {
			// TODO Auto-generated method stub
			
		}

		public void windowDeactivated(WindowEvent e) {
			// TODO Auto-generated method stub
			
		}
		
	}
}
