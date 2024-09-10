package org.ntut.posd2024f.shapes;

import java.util.Comparator;

public class Sort {

    // 按照面積進行升序排序，當面積相同時按周長升序，若周長也相同則按類別名稱排序
    public static final Comparator<Shape> BY_AREA_ASCENDING = (s1, s2) -> {
        int areaComparison = Double.compare(s1.area(), s2.area());
        if (areaComparison == 0) {
            // 當面積相同時，按周長排序
            int perimeterComparison = Double.compare(s1.perimeter(), s2.perimeter());
            if (perimeterComparison == 0) {
                // 若面積和周長相同，按類別名稱字母順序排序
                return s1.getClass().getSimpleName().compareTo(s2.getClass().getSimpleName());
            }
            return perimeterComparison;
        }
        return areaComparison;
    };

    // 按照面積進行降序排序，當面積相同時按周長升序，若周長也相同則按類別名稱排序
    public static final Comparator<Shape> BY_AREA_DESCENDING = (s1, s2) -> {
        int areaComparison = Double.compare(s2.area(), s1.area());
        if (areaComparison == 0) {
            // 當面積相同時，按周長排序
            int perimeterComparison = Double.compare(s1.perimeter(), s2.perimeter());
            if (perimeterComparison == 0) {
                // 若面積和周長相同，按類別名稱字母順序排序
                return s1.getClass().getSimpleName().compareTo(s2.getClass().getSimpleName());
            }
            return perimeterComparison;
        }
        return areaComparison;
    };

    // 按周長升序排序，當周長相同時按面積升序，若面積也相同則按類別名稱排序
    public static final Comparator<Shape> BY_PERIMETER_ASCENDING = (s1, s2) -> {
        int perimeterComparison = Double.compare(s1.perimeter(), s2.perimeter());
        if (perimeterComparison == 0) {
            // 當周長相同時，按面積排序
            int areaComparison = Double.compare(s1.area(), s2.area());
            if (areaComparison == 0) {
                // 若周長和面積相同，按類別名稱字母順序排序
                return s1.getClass().getSimpleName().compareTo(s2.getClass().getSimpleName());
            }
            return areaComparison;
        }
        return perimeterComparison;
    };

    // 按周長降序排序，當周長相同時按面積升序，若面積也相同則按類別名稱排序
    public static final Comparator<Shape> BY_PERIMETER_DESCENDING = (s1, s2) -> {
        int perimeterComparison = Double.compare(s2.perimeter(), s1.perimeter());
        if (perimeterComparison == 0) {
            // 當周長相同時，按面積排序
            int areaComparison = Double.compare(s1.area(), s2.area());
            if (areaComparison == 0) {
                // 若周長和面積相同，按類別名稱字母順序排序
                return s1.getClass().getSimpleName().compareTo(s2.getClass().getSimpleName());
            }
            return areaComparison;
        }
        return perimeterComparison;
    };
}
