### Problem statement
** Review Problem 1 (Homework 1): sorting shapes**
A simple geometry application called _geo_ is needed to sort shapes such as triangles, circles, rectangles and others. As a command line application, _geo_ reads shapes from an input file, sorts the shapes by area or perimeter in increasing order or decreasing order, and write the result to an output file. For example,
```
geo input.txt output.txt area inc
```
sorts the shapes in file _input.txt_ in increasing order by area, and writes the result to the file _output.txt_. And
```
geo input.txt output.txt perimeter dec
```
sorts the shapes in file _input.txt_ in decreasing order by perimeter, and writes the result to the file _output.txt_.

### Specification
Please complete the given code to satisfy all the following conditions.

* Please implement shapes - `Circle`, `Triangle`, `Rectangle`.
	* Complete `area()`, `perimeter()`, and `toString()`.

	* The `toString()` method will return the format we've given in `input.txt`.

* Please implement the following class - `Sort`.

	* In the Sort class please complete "ByAreaAscending", "ByAreaDescending", "ByPerimeterAscending", and "ByPerimeterDescending".

	* **Don't use lambda in this assignment.**

* Please do exception handling if the shape can't be successfully created.(The excepton type of the TA's test will be **Exception** and throw `It's not a circle!`, `It's not a rectangle!`, `It's not a triangle!`)

* If the input shape can't be successfully created, just **ignore** and output the shapes which are correct.

* For each function/method, write at least one unit test for it.

* For the arguments in this project,
	* `args[0]` is the input file name,
	* `args[1]` is the output file name,
	* `args[2]` is the condition which you want to sort,
	* `args[3]` is the order which you want to sort.
	* Hint:
	You can execute the program by the following command
	```
	mvn exec:java -Dexec.mainClass="org.ntutssl.shapes.Geo" -Dexec.args="test_data/input.txt test_data/output.txt {area inc}"
	```
	change ```area inc``` to `area dec`, `perimeter inc` or `perimeter dec`

* Please make your directory follow the following file structure:

```
├── pom.xml
├── src
│   ├── main
│   │   └── java
│   │       └── org
│   │           └── ntut
│   │               └── posd2024f
│   │               	└── shapes
│   │                   	├── Circle.java
│   │                   	├── Geo.java
│   │                   	├── InputOutput.java
│   │                   	├── Shape.java
│   │                   	├── Rectangle.java
│   │                   	├── Sort.java
│   │                   	└── Triangle.java
│   └── test
│       └── java
│           └── org
│               └── ntut
│                   └── posd2024f
│                   	└── shapes
│                       	├── CircleTest.java
│                       	├── RectangleTest.java
│                       	├── ShapeTest.java
│                       	├── SortTest.java
│                       	└── TriangleTest.java
└── test_data
    ├── input.txt
    └── output.txt
```
## Score
1. Unit tests written by yourself: 35%.
2. Unit tests written by TA: 35%.
3. The executable program: 30%.

