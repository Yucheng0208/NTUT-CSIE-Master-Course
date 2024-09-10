/*
Project Name: Homework 1
File Name: Shape.java
Datetime: 2024/09/09 11:18pm
*/

public abstract class Shape{

    // 抽象方向：計算面積
    abstract double getArea();

    // 抽象方法：計算周長
    abstract double getPerimeter();

    // 抽象方法：取得形狀名稱
    abstract String getName();

    @Override
    public String toString(){
        return getName() + " (Area: " + getArea() + ", Perimeter: " + getPerimeter() + ")";
    }
}

// Circle 類別
class Circle extends Shape{
    private double radius;

    public Circle(double radius){
        this.radius = radius;
    }

    @Override
    double getArea(){
        return Math.PI * radius * radius;
    }

    @Override
    double getPerimeter(){
        return 2 * Math.PI * radius; 
    }

    @Override
    String getName(){
        return "Circle";
    }
}

// Rectangle 類別
class Rectangle extends Shape{
    private double length, width;

    public Rectangle(double length, double width){
        this.length = length;
        this.width = width;
    }

    @Override
    double getArea(){
        return length * width;
    }

    @Override
    double getPerimeter(){
        return 2 * (length + width);
    }

    @Override
    String getName(){
        return "Rectangle";
    }
}

// Triangle 類別
class Triangle extends Shape{
    private double base, height, side1, side2, side3;

    public Triangle(double base, double height, double side1, double side2, double side3){
        this.base = base;
        this.height = height;
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }

    @Override
    double getArea(){
        return 0.5 * base * height;
    }

    @Override
    double getPerimeter(){
        return side1 + side2 + side3;
    }

    @Override
    String getName(){
        return "Triangle";
    }
}