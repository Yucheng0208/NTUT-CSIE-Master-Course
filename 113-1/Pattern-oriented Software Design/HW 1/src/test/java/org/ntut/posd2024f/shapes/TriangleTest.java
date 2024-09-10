package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import java.util.ArrayList;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class TriangleTest {

    @Test
    public void testTriangleArea() throws Exception {
        Triangle triangle = new Triangle(3, 4, 5);
        assertEquals(6.0, triangle.area(), 0.0001);
    }

    @Test
    public void testTrianglePerimeter() throws Exception {
        Triangle triangle = new Triangle(3, 4, 5);
        assertEquals(12.0, triangle.perimeter(), 0.0001);
    }

    @Test(expected = Exception.class)
    public void testInvalidTriangle() throws Exception {
        new Triangle(1, 2, 10);  // This should throw an exception
    }

    @Test
    public void testTriangleToString() throws Exception {
        Triangle triangle = new Triangle(3, 4, 5);
        assertEquals("Triangle 3.0 4.0 5.0", triangle.toString());
    }
}