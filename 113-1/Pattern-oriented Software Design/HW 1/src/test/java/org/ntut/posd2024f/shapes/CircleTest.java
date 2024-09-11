package org.ntut.posd2024f.shapes;

import static org.junit.Assert.*;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class CircleTest {

    @Rule
    public ExpectedException thrown = ExpectedException.none();

    // 測試面積計算
    @Test
    public void testCircleArea() throws Exception {
        Circle circle = new Circle(1.0);
        assertEquals(Math.PI, circle.area(), 0.0001); // 預期結果為 π，容許誤差 0.0001
    }

    // 測試周長計算
    @Test
    public void testCirclePerimeter() throws Exception {
        Circle circle = new Circle(1.0);
        assertEquals(2 * Math.PI, circle.perimeter(), 0.0001); // 預期結果為 2π，容許誤差 0.0001
    }

    // 測試無效的半徑應該拋出例外
    @Test
    public void testInvalidCircleRadius() throws Exception {
        thrown.expect(Exception.class);
        thrown.expectMessage("It's not a circle!");
        new Circle(-1.0); // 半徑為負數，應該拋出例外
    }

    // 測試 toString 方法
    @Test
    public void testCircleToString() throws Exception {
        Circle circle = new Circle(2.5);
        assertEquals("Circle 2.5", circle.toString()); // 預期輸出 "Circle 2.5"
    }
}