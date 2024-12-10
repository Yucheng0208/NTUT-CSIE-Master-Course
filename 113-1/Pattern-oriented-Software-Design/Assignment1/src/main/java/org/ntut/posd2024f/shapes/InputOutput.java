package org.ntut.posd2024f.shapes;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Comparator;

public class InputOutput {

    // Constructor
    public InputOutput() {
    }

    // Method to read shapes from input file and construct Shape objects
    public ArrayList<Shape> handleInput(String inputFileName) throws NumberFormatException, Exception {
        ArrayList<Shape> shapes = new ArrayList<>();

        try (BufferedReader reader = new BufferedReader(new FileReader(inputFileName))) {
            String line;
            while ((line = reader.readLine()) != null) {
                try {
                    Shape shape = ConstructShape(line);
                    if (shape != null) {
                        shapes.add(shape); // Add only valid shapes
                    }
                } catch (Exception e) {
                    // Ignore invalid shapes and continue reading
                    System.out.println(e.getMessage());
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading input file: " + e.getMessage());
        }

        return shapes;
    }

    // Method to sort shapes based on the given criteria
    public ArrayList<Shape> handleSort(ArrayList<Shape> shapes, String compare, String order) {
        Comparator<Shape> comparator = null;

        // Choose sorting criteria based on compare and order
        if (compare.equals("area")) {
            if (order.equals("inc")) {
                comparator = Sort.BY_AREA_ASCENDING;
            } else if (order.equals("dec")) {
                comparator = Sort.BY_AREA_DESCENDING;
            }
        } else if (compare.equals("perimeter")) {
            if (order.equals("inc")) {
                comparator = Sort.BY_PERIMETER_ASCENDING;
            } else if (order.equals("dec")) {
                comparator = Sort.BY_PERIMETER_DESCENDING;
            }
        }

        // If invalid sorting criteria, return shapes without sorting
        if (comparator != null) {
            shapes.sort(comparator);
        }

        return shapes;
    }

    // Method to write sorted shapes to output file
    public void handleOutput(ArrayList<Shape> shapes, String outputFileName) {
        try (PrintWriter writer = new PrintWriter(outputFileName)) {
            for (Shape shape : shapes) {
                writer.println(shape.toString());
            }
        } catch (IOException e) {
            System.err.println("Error writing to output file: " + e.getMessage());
        }
    }

    // Method to construct a Shape object from a string
    private Shape ConstructShape(String str) throws NumberFormatException, Exception {
        String[] parts = str.split(" ");
        String shapeType = parts[0];

        switch (shapeType) {
            case "Circle":
                if (parts.length == 2) {
                    double radius = Double.parseDouble(parts[1]);
                    return new Circle(radius);
                }
                break;

            case "Triangle":
                if (parts.length == 4) {
                    double a = Double.parseDouble(parts[1]);
                    double b = Double.parseDouble(parts[2]);
                    double c = Double.parseDouble(parts[3]);
                    return new Triangle(a, b, c);
                }
                break;

            case "Rectangle":
                if (parts.length == 3) {
                    double length = Double.parseDouble(parts[1]);
                    double width = Double.parseDouble(parts[2]);
                    return new Rectangle(length, width);
                }
                break;

            default:
                throw new Exception("Unknown shape type: " + shapeType);
        }

        throw new Exception("Invalid shape data: " + str);
    }
}