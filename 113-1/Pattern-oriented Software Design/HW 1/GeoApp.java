/*
Project Name: Homework 1
File Name: GeoApp.java
Author: Yu-Cheng Chang 張育丞
Student ID: 113598043
Datetime: 2024/09/09 10:50pm
*/
import java.io.*;
import java.util.*;

public class GeoApp {
    public static void main(String[] args) {
        // 檢查命令列參數是否正確
        if (args.length != 4) {
            System.out.println("Usage: java GeoApp <input_file> <output_file> <criteria> <order>");
            return;
        }
        
        String inputFile = args[0];
        String outputFile = args[1];
        String criteria = args[2];  // "area" 或 "perimeter"
        String order = args[3];     // "inc" 或 "dec"

        // 讀取形狀資料
        List<Shape> shapes = readShapesFromFile(inputFile);

        // 排序形狀
        Comparator<Shape> comparator = getComparator(criteria);
        if (order.equals("dec")) {
            comparator = comparator.reversed();
        }
        shapes.sort(comparator);

        // 將結果寫入到輸出檔案
        writeShapesToFile(shapes, outputFile);
    }

    // 根據面積或周長進行排序
    private static Comparator<Shape> getComparator(String criteria) {
        if (criteria.equals("area")) {
            return Comparator.comparing(Shape::getArea);
        } else if (criteria.equals("perimeter")) {
            return Comparator.comparing(Shape::getPerimeter);
        } else {
            throw new IllegalArgumentException("Invalid sorting criteria: " + criteria);
        }
    }

    // 從輸入檔案讀取形狀資料
    private static List<Shape> readShapesFromFile(String fileName) {
        List<Shape> shapes = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(" ");
                String shapeType = parts[0];
                switch (shapeType) {
                    case "Circle":
                        double radius = Double.parseDouble(parts[1]);
                        shapes.add(new Circle(radius));
                        break;
                    case "Rectangle":
                        double length = Double.parseDouble(parts[1]);
                        double width = Double.parseDouble(parts[2]);
                        shapes.add(new Rectangle(length, width));
                        break;
                    case "Triangle":
                        double base = Double.parseDouble(parts[1]);
                        double height = Double.parseDouble(parts[2]);
                        double side1 = Double.parseDouble(parts[3]);
                        double side2 = Double.parseDouble(parts[4]);
                        double side3 = Double.parseDouble(parts[5]);
                        shapes.add(new Triangle(base, height, side1, side2, side3));
                        break;
                    default:
                        throw new IllegalArgumentException("Unknown shape type: " + shapeType);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return shapes;
    }

    // 將排序結果寫入輸出檔案
    private static void writeShapesToFile(List<Shape> shapes, String fileName) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(fileName))) {
            for (Shape shape : shapes) {
                bw.write(shape.toString());
                bw.newLine();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
