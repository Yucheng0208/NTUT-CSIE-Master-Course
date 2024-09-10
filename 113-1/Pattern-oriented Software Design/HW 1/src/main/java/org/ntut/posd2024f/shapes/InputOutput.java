package org.ntut.posd2024f.shapes;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

public class InputOutput {

    public InputOutput() {
    }

    public ArrayList<Shape> handleInput(String inputFileName) throws NumberFormatException, Exception {
        ArrayList<Shape> shapes = new ArrayList<>();  // 定義 shapes 變數
        try (BufferedReader br = new BufferedReader(new FileReader(inputFileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                try {
                    shapes.add(ConstructShape(line));
                } catch (Exception e) {
                    System.err.println(e.getMessage());
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return shapes;  // 返回 shapes 變數
    }

    public ArrayList<Shape> handleSort(ArrayList<Shape> shapes, String compare, String order) {
        if ("area".equals(compare)) {
            if ("inc".equals(order)) {
                shapes.sort(Sort.BY_AREA_ASCENDING);
            } else {
                shapes.sort(Sort.BY_AREA_DESCENDING);
            }
        } else if ("perimeter".equals(compare)) {
            if ("inc".equals(order)) {
                shapes.sort(Sort.BY_PERIMETER_ASCENDING);
            } else {
                shapes.sort(Sort.BY_PERIMETER_DESCENDING);
            }
        }
        return shapes;
    }

    public void handleOutput(ArrayList<Shape> shapes, String outputFileName) {
        try (PrintWriter writer = new PrintWriter(outputFileName)) {
            for (Shape shape : shapes) {
                writer.println(shape.toString());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private Shape ConstructShape(String str) throws NumberFormatException, Exception {
        String[] parts = str.split(" ");
        switch (parts[0]) {
            case "Circle":
                return new Circle(Double.parseDouble(parts[1]));
            case "Rectangle":
                return new Rectangle(Double.parseDouble(parts[1]), Double.parseDouble(parts[2]));
            case "Triangle":
                return new Triangle(Double.parseDouble(parts[1]), Double.parseDouble(parts[2]), Double.parseDouble(parts[3]));
            default:
                throw new Exception("Unknown shape type");
        }
    }
}