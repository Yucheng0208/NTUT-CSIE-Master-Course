package org.ntut.posd2024f.shapes;

import static org.junit.Assert.assertEquals;
import java.util.ArrayList;
import java.util.Collections;
import org.junit.Test;

public class SortTest {

    @Test
    public void testSortByAreaAscending() throws Exception {
        ArrayList<Shape> shapes = new ArrayList<>();
        shapes.add(new Circle(2));  // 面積: 12.5664
        shapes.add(new Rectangle(2, 3));  // 面積: 6
        shapes.add(new Triangle(3, 4, 5));  // 面積: 6
        
        Collections.sort(shapes, Sort.BY_AREA_ASCENDING);

        // 按面積升序排序，面積相同時按名稱排序，Rectangle 應排在 Triangle 之前
        assertEquals("Rectangle", shapes.get(0).getClass().getSimpleName());
        assertEquals(6.0, shapes.get(0).area(), 0.0001); // Rectangle with area 6

        assertEquals("Triangle", shapes.get(1).getClass().getSimpleName());
        assertEquals(6.0, shapes.get(1).area(), 0.0001); // Triangle with area 6

        assertEquals("Circle", shapes.get(2).getClass().getSimpleName());
        assertEquals(12.5664, shapes.get(2).area(), 0.0001); // Circle with area 12.5664
    }

    @Test
    public void testSortByPerimeterDescending() throws Exception {
        ArrayList<Shape> shapes = new ArrayList<>();
        shapes.add(new Circle(2));  // 周長: 12.5664
        shapes.add(new Rectangle(2, 3));  // 周長: 10
        shapes.add(new Triangle(3, 4, 5));  // 周長: 12

        Collections.sort(shapes, Sort.BY_PERIMETER_DESCENDING);

        // 按周長降序排序，當周長相同時按名稱排序，Circle 應排在 Triangle 之前
        assertEquals("Circle", shapes.get(0).getClass().getSimpleName());
        assertEquals(12.5664, shapes.get(0).perimeter(), 0.0001); // Circle with perimeter 12.5664

        assertEquals("Triangle", shapes.get(1).getClass().getSimpleName());
        assertEquals(12.0, shapes.get(1).perimeter(), 0.0001); // Triangle with perimeter 12

        assertEquals("Rectangle", shapes.get(2).getClass().getSimpleName());
        assertEquals(10.0, shapes.get(2).perimeter(), 0.0001); // Rectangle with perimeter 10
    }
}