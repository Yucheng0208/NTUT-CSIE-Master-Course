package org.ntut.posd2024f.shapes;

import java.util.ArrayList;

import static org.junit.Assert.*;

import org.junit.Test;

public class SortTest {
        // 測試按面積升序排序
        @Test
        public void testSortByAreaAscending() throws Exception {
            ArrayList<Shape> shapes = new ArrayList<>();
            shapes.add(new Circle(1.0)); // Area: π
            shapes.add(new Rectangle(2.0, 3.0)); // Area: 6.0
            shapes.add(new Triangle(3.0, 4.0, 5.0)); // Area: 6.0
            shapes.add(new Circle(0.5)); // Area: 0.785
    
            shapes.sort(Sort.BY_AREA_ASCENDING);
    
            assertEquals(0.7853981633974483, shapes.get(0).area(), 0.0001); // Circle with radius 0.5
            assertEquals(Math.PI, shapes.get(1).area(), 0.0001); // Circle with radius 1.0
            assertEquals(6.0, shapes.get(2).area(), 0.0001); // Rectangle with sides 2x3
            assertEquals(6.0, shapes.get(3).area(), 0.0001); // Triangle with sides 3, 4, 5
        }
    
        // 測試按面積降序排序
        @Test
        public void testSortByAreaDescending() throws Exception {
            ArrayList<Shape> shapes = new ArrayList<>();
            shapes.add(new Circle(1.0)); // Area: π
            shapes.add(new Rectangle(2.0, 3.0)); // Area: 6.0
            shapes.add(new Triangle(3.0, 4.0, 5.0)); // Area: 6.0
            shapes.add(new Circle(0.5)); // Area: 0.785
    
            shapes.sort(Sort.BY_AREA_DESCENDING);
    
            assertEquals(6.0, shapes.get(0).area(), 0.0001); // Rectangle with sides 2x3
            assertEquals(6.0, shapes.get(1).area(), 0.0001); // Triangle with sides 3, 4, 5
            assertEquals(Math.PI, shapes.get(2).area(), 0.0001); // Circle with radius 1.0
            assertEquals(0.7853981633974483, shapes.get(3).area(), 0.0001); // Circle with radius 0.5
        }
    
        // 測試按周長升序排序
        @Test
        public void testSortByPerimeterAscending() throws Exception {
            ArrayList<Shape> shapes = new ArrayList<>();
            shapes.add(new Circle(1.0)); // Perimeter: 2π
            shapes.add(new Rectangle(2.0, 3.0)); // Perimeter: 10.0
            shapes.add(new Triangle(3.0, 4.0, 5.0)); // Perimeter: 12.0
            shapes.add(new Circle(0.5)); // Perimeter: π
    
            shapes.sort(Sort.BY_PERIMETER_ASCENDING);
    
            assertEquals(Math.PI, shapes.get(0).perimeter(), 0.0001); // Circle with radius 0.5
            assertEquals(2 * Math.PI, shapes.get(1).perimeter(), 0.0001); // Circle with radius 1.0
            assertEquals(10.0, shapes.get(2).perimeter(), 0.0001); // Rectangle with sides 2x3
            assertEquals(12.0, shapes.get(3).perimeter(), 0.0001); // Triangle with sides 3, 4, 5
        }
    
        // 測試按周長降序排序
        @Test
        public void testSortByPerimeterDescending() throws Exception {
            ArrayList<Shape> shapes = new ArrayList<>();
            shapes.add(new Circle(1.0)); // Perimeter: 2π
            shapes.add(new Rectangle(2.0, 3.0)); // Perimeter: 10.0
            shapes.add(new Triangle(3.0, 4.0, 5.0)); // Perimeter: 12.0
            shapes.add(new Circle(0.5)); // Perimeter: π
    
            shapes.sort(Sort.BY_PERIMETER_DESCENDING);
    
            assertEquals(12.0, shapes.get(0).perimeter(), 0.0001); // Triangle with sides 3, 4, 5
            assertEquals(10.0, shapes.get(1).perimeter(), 0.0001); // Rectangle with sides 2x3
            assertEquals(2 * Math.PI, shapes.get(2).perimeter(), 0.0001); // Circle with radius 1.0
            assertEquals(Math.PI, shapes.get(3).perimeter(), 0.0001); // Circle with radius 0.5
        }
}