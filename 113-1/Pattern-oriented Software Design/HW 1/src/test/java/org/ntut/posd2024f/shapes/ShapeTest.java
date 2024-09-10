package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import java.util.ArrayList;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class ShapeTest {
    
    @Test
    public void testShapeInterface() throws Exception {
        Shape circle = new Circle(5);
        assertEquals(78.5398, circle.area(), 0.0001);
        assertEquals(31.4159, circle.perimeter(), 0.0001);
        assertEquals("Circle 5.0", circle.toString());

        Shape rectangle = new Rectangle(0.5, 1.5);
        assertEquals(0.75, rectangle.area(), 0.0001); 
        assertEquals(4.0, rectangle.perimeter(), 0.0001);  
        assertEquals("Rectangle 0.5 1.5", rectangle.toString());

        Shape triangle = new Triangle(3, 4, 5);
        assertEquals(6.0, triangle.area(), 0.0001);
        assertEquals(12.0, triangle.perimeter(), 0.0001);
        assertEquals("Triangle 3.0 4.0 5.0", triangle.toString());
    }
}
