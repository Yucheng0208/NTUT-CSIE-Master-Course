package org.ntut.posd2024f.shapes;

import java.lang.Math;
import java.util.ArrayList;
import java.util.Arrays;

public class Triangle implements Shape {
    private double a, b, c;

    // Constructor
    public Triangle(double a, double b, double c) throws Exception {
        // Check if the sides form a valid triangle using the triangle inequality theorem
        if (a <= 0 || b <= 0 || c <= 0 || (a + b <= c) || (a + c <= b) || (b + c <= a)) {
            throw new Exception("It's not a triangle!");
        }
        this.a = a;
        this.b = b;
        this.c = c;
    }

    // Method to calculate the perimeter of the triangle
    @Override
    public double perimeter() {
        return a + b + c; // Perimeter formula: sum of all sides
    }

    // Method to calculate the area of the triangle using Heron's formula
    @Override
    public double area() {
        double s = perimeter() / 2; // Semi-perimeter
        return Math.sqrt(s * (s - a) * (s - b) * (s - c)); // Heron's formula
    }

    // Method to return a string representation of the triangle in the format "Triangle a b c"
    @Override
    public String toString() {
        return "Triangle " + a + " " + b + " " + c;
    }
}