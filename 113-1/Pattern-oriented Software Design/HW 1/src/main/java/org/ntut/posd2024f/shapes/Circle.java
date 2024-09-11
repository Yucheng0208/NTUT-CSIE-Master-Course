package org.ntut.posd2024f.shapes;

public class Circle implements Shape {
    private double radius;

    // Constructor
    public Circle(double radius) throws Exception {
        if (radius <= 0) {
            throw new Exception("It's not a circle!");
        }
        this.radius = radius;
    }

    // Method to calculate the area of the circle
    @Override
    public double area() {
        return Math.PI * radius * radius; // Area formula: πr²
    }

    // Method to calculate the perimeter (circumference) of the circle
    @Override
    public double perimeter() {
        return 2 * Math.PI * radius; // Perimeter formula: 2πr
    }

    // Method to return a string representation of the circle in the format "Circle radius"
    @Override
    public String toString() {
        return "Circle " + radius; // Output format as per the requirement
    }
}
