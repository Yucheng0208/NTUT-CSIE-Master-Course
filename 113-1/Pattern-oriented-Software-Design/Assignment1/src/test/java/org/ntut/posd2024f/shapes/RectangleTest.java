package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class RectangleTest {
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();

    // 測試矩形面積計算
    @Test
    public void testRectangleArea() throws Exception {
        Rectangle rectangle = new Rectangle(2.0, 3.0);
        assertEquals(6.0, rectangle.area(), 0.0001); // 預期面積為 2 * 3 = 6.0
    }

    // 測試矩形周長計算
    @Test
    public void testRectanglePerimeter() throws Exception {
        Rectangle rectangle = new Rectangle(2.0, 3.0);
        assertEquals(10.0, rectangle.perimeter(), 0.0001); // 預期周長為 2 * (2 + 3) = 10.0
    }

    // 測試無效的長和寬（長或寬 <= 0）應拋出例外
    @Test
    public void testInvalidRectangle() throws Exception {
        thrown.expect(Exception.class);
        thrown.expectMessage("It's not a rectangle!");
        new Rectangle(-1.0, 2.0); // 長度為負，應該拋出例外
    }

    // 測試 toString 方法
    @Test
    public void testRectangleToString() throws Exception {
        Rectangle rectangle = new Rectangle(2.5, 3.5);
        assertEquals("Rectangle 2.5 3.5", rectangle.toString()); // 預期輸出 "Rectangle 2.5 3.5"
    }
}