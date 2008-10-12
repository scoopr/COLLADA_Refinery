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
package refinery.history;

import java.util.UUID;

/**
 * The <code>Identifiable</code> abstract class provides an unique id created randomly in
 * the constructor, and getter and setter on this id.
 * 
 * Note: Making this a class instead of an interface may cause problems because we cannot
 * make a class inherit from 2 classes at the same time in Java. But we have some code here,
 * so we factorize as long as this is not a problem for something else.
 * @author PDavid
 */

public abstract class IdentifiableImpl implements Identifiable{
	protected UUID id;
	
	
	public IdentifiableImpl(){
		id = UUID.randomUUID();
	}

	public UUID getUID() {
		return id;
	}

	public void setUID(UUID _id) {
		id = _id;
	}
}
