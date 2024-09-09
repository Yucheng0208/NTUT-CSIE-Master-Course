# Review Problem 1 (Homework 1): sorting shapes

A simple geometry application called geo is needed to sort shapes such as triangles, circles, rectangles and others. As a command line application, geo reads shapes from an input file, sorts the shapes by area or perimeter in increasing order or decreasing order, and write the result to an output file. For example, geo input.txt output.txt area sorts the shapes in file input.txt in increasing order by area, and writes the result to the file output.txt. And

```geo input.txt output.txt perimeter dec```

sorts the shapes in file input.txt in decreasing order by perimeter, and writes the result to the file output.txt.

## Practice Problem 1: Shape manipulation

A program for manipulating geometric shapes is needed. With the program, the user creates/deletes/modifies simple shapes such as square, circle, triangle, and convex polygons. Individual shapes will have properties such as area, perimeter, color and so on. Compound shapes can be formed from individual shapes. Area and perimeter properties are still needed on compound shapes. A compound shape can include other compound shapes. The following operations on shapes are needed.

* Add shapes to a compound shape;
* Delete shapes from a compound shape;
* Group and ungroup;
* Move shapes out of a compound shape;
* Move shapes from a compound shape to another compound shape;
* Select shapes by Boolean expression on the properties area, perimeter, color and type; and
* Load/store shapes from/into a file.