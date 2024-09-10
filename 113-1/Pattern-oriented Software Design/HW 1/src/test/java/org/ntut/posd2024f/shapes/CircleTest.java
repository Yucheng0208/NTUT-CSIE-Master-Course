package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class CircleTest {

    @Test
    public void testCircleArea() throws Exception {
        Circle circle = new Circle(5);
        assertEquals(78.5398, circle.area(), 0.0001);
    }
    
    @Test
    public void testCirclePerimeter() throws Exception {
        Circle circle = new Circle(5);
        assertEquals(31.4159, circle.perimeter(), 0.0001);
    }

    @Test(expected = Exception.class)
    public void testInvalidCircle() throws Exception {
        new Circle(-5);  // This should throw an exception
    }

    @Test
    public void testCircleToString() throws Exception {
        Circle circle = new Circle(5);
        assertEquals("Circle 5.0", circle.toString());
    }
}