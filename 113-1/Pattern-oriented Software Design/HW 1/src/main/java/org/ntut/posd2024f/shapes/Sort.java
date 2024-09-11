package org.ntut.posd2024f.shapes;

import java.util.Comparator;

public class Sort {
    // Comparators for sorting
    public static Comparator<Shape> BY_AREA_ASCENDING = new ByAreaAscending();
    public static Comparator<Shape> BY_AREA_DESCENDING = new ByAreaDescending();
    public static Comparator<Shape> BY_PERIMETER_ASCENDING = new ByPerimeterAscending();
    public static Comparator<Shape> BY_PERIMETER_DESCENDING = new ByPerimeterDescending();

    // Class to sort by area in ascending order
    private static class ByAreaAscending implements Comparator<Shape> {
        @Override
        public int compare(Shape left, Shape right) {
            // Compare two shapes by area, in ascending order
            return Double.compare(left.area(), right.area());
        }
    }

    // Class to sort by area in descending order
    private static class ByAreaDescending implements Comparator<Shape> {
        @Override
        public int compare(Shape left, Shape right) {
            // Compare two shapes by area, in descending order
            return Double.compare(right.area(), left.area());
        }
    }

    // Class to sort by perimeter in ascending order
    private static class ByPerimeterAscending implements Comparator<Shape> {
        @Override
        public int compare(Shape left, Shape right) {
            // Compare two shapes by perimeter, in ascending order
            return Double.compare(left.perimeter(), right.perimeter());
        }
    }

    // Class to sort by perimeter in descending order
    private static class ByPerimeterDescending implements Comparator<Shape> {
        @Override
        public int compare(Shape left, Shape right) {
            // Compare two shapes by perimeter, in descending order
            return Double.compare(right.perimeter(), left.perimeter());
        }
    }
}



