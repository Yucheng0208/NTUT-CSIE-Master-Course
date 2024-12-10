package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import java.util.ArrayList;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class TriangleTest {
   
    @Rule
    public ExpectedException thrown = ExpectedException.none();

    // 測試三角形的面積計算
    @Test
    public void testTriangleArea() throws Exception {
        Triangle triangle = new Triangle(3.0, 4.0, 5.0);
        assertEquals(6.0, triangle.area(), 0.0001); // 預期結果為 6.0 (用海倫公式計算)
    }

    // 測試三角形的周長計算
    @Test
    public void testTrianglePerimeter() throws Exception {
        Triangle triangle = new Triangle(3.0, 4.0, 5.0);
        assertEquals(12.0, triangle.perimeter(), 0.0001); // 預期結果為 3 + 4 + 5 = 12.0
    }

    // 測試無效的三角形應該拋出例外
    @Test
    public void testInvalidTriangle() throws Exception {
        thrown.expect(Exception.class);
        thrown.expectMessage("It's not a triangle!");
        new Triangle(1.0, 2.0, 10.0); // 這不符合三角形不等式，應該拋出例外
    }

    // 測試三角形的 toString 方法
    @Test
    public void testTriangleToString() throws Exception {
        Triangle triangle = new Triangle(3.0, 4.0, 5.0);
        assertEquals("Triangle 3.0 4.0 5.0", triangle.toString()); // 預期輸出格式
    }
}