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
package refinery;

/**
 * The <code>Couple</code> class is a simple implementation of a couple of generic objects.
 * 
 * @author Philippe David
 *
 * @param <T1> Type of the first member
 * @param <T2> Type of the second member
 */
public class Couple<T1, T2> {
	public T1 first;
	public T2 second;
	
	public Couple(T1 _first, T2 _second){
		first = _first;
		second = _second;
	}
	
	public boolean equals(Couple c2){
		return (first.equals(c2.first) && second.equals(c2.second));
	}
	
	public String toString(){
		return "Couple "+first+" <-> "+second;
	}
}
