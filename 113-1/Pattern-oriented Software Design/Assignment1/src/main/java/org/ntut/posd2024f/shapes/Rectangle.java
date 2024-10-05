package org.ntut.posd2024f.shapes;

public class Rectangle implements Shape {
    private double length;
    private double width;

    // Constructor
    public Rectangle(double length, double width) throws Exception {
        if (length <= 0 || width <= 0) {
            throw new Exception("It's not a rectangle!");
        }
        this.length = length;
        this.width = width;
    }

    // Method to calculate the area of the rectangle
    @Override
    public double area() {
        return length * width; // Area formula: length * width
    }

    // Method to calculate the perimeter of the rectangle
    @Override
    public double perimeter() {
        return 2 * (length + width); // Perimeter formula: 2 * (length + width)
    }

    // Method to return a string representation of the rectangle in the format "Rectangle length width"
    @Override
    public String toString() {
        return "Rectangle " + length + " " + width;
    }
}