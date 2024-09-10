package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class RectangleTest {

    @Test
    public void testRectangleArea() throws Exception {
        Rectangle rectangle = new Rectangle(4, 5);
        assertEquals(20.0, rectangle.area(), 0.0001);
    }

    @Test
    public void testRectanglePerimeter() throws Exception {
        Rectangle rectangle = new Rectangle(0.5, 1.5);
        assertEquals(4.0, rectangle.perimeter(), 0.0001);
    }

    @Test(expected = Exception.class)
    public void testInvalidRectangle() throws Exception {
        new Rectangle(-0.5, 1.5);  // This should throw an exception
    }

    @Test
    public void testRectangleToString() throws Exception {
        Rectangle rectangle = new Rectangle(0.5, 1.5);
        assertEquals("Rectangle 0.5 1.5", rectangle.toString());
    }    
}
