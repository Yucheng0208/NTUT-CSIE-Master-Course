package org.ntut.posd2024f.shapes;

public class Circle implements Shape {
    private double radius;
    public Circle(double radius) throws Exception {
        if (radius <= 0) {
            throw new Exception("It's not a circle!");
        }
        this.radius = radius;
    }

    @Override
    public double area() {
        return Math.PI * Math.pow(this.radius, 2);
    } 

    @Override
    public double perimeter() {
        return 2 * Math.PI * this.radius;
    }
    @Override
    public String toString() {
        return "Circle with radius: " + this.radius;
    }
}
