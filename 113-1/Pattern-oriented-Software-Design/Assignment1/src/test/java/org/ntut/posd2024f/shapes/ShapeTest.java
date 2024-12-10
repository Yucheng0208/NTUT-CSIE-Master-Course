package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import java.util.ArrayList;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class ShapeTest {
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();

    // 測試 Circle 作為 Shape 的面積計算
    @Test
    public void testCircleAsShapeArea() throws Exception {
        Shape circle = new Circle(1.0);
        assertEquals(Math.PI, circle.area(), 0.0001); // 預期結果為 π
    }

    // 測試 Rectangle 作為 Shape 的面積計算
    @Test
    public void testRectangleAsShapeArea() throws Exception {
        Shape rectangle = new Rectangle(2.0, 3.0);
        assertEquals(6.0, rectangle.area(), 0.0001); // 預期結果為 2 * 3 = 6.0
    }

    // 測試 Triangle 作為 Shape 的面積計算
    @Test
    public void testTriangleAsShapeArea() throws Exception {
        Shape triangle = new Triangle(3.0, 4.0, 5.0);
        assertEquals(6.0, triangle.area(), 0.0001); // 預期結果為 6.0
    }

    // 測試 Circle 作為 Shape 的周長計算
    @Test
    public void testCircleAsShapePerimeter() throws Exception {
        Shape circle = new Circle(1.0);
        assertEquals(2 * Math.PI, circle.perimeter(), 0.0001); // 預期結果為 2π
    }

    // 測試 Rectangle 作為 Shape 的周長計算
    @Test
    public void testRectangleAsShapePerimeter() throws Exception {
        Shape rectangle = new Rectangle(2.0, 3.0);
        assertEquals(10.0, rectangle.perimeter(), 0.0001); // 預期結果為 10.0
    }

    // 測試 Triangle 作為 Shape 的周長計算
    @Test
    public void testTriangleAsShapePerimeter() throws Exception {
        Shape triangle = new Triangle(3.0, 4.0, 5.0);
        assertEquals(12.0, triangle.perimeter(), 0.0001); // 預期結果為 12.0
    }

    // 測試將 Shape 物件放入 ArrayList 並檢查它們的面積
    @Test
    public void testShapesInArrayList() throws Exception {
        ArrayList<Shape> shapes = new ArrayList<>();
        shapes.add(new Circle(1.0));
        shapes.add(new Rectangle(2.0, 3.0));
        shapes.add(new Triangle(3.0, 4.0, 5.0));

        // 檢查每個 Shape 的面積
        assertEquals(Math.PI, shapes.get(0).area(), 0.0001); // Circle
        assertEquals(6.0, shapes.get(1).area(), 0.0001); // Rectangle
        assertEquals(6.0, shapes.get(2).area(), 0.0001); // Triangle
    }
}