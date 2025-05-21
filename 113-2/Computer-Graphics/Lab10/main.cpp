#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib> // For rand()
#include <ctime>   // For time() for srand()
#include <algorithm> // For std::max, std::min, std::sort

#define MENU_GRID_10 1
#define MENU_GRID_15 2
#define MENU_GRID_20 3
#define MENU_EXIT    4

int gridSize = 10;
int windowWidth = 600, windowHeight = 600;

struct Pixel {
    int x, y;
    float r, g, b;
    bool isEndpointHighlight;
};

std::vector<Pixel> pixels;
std::vector<Pixel> fillQueue;
std::vector<Pixel> clearingQueue;
std::vector<Pixel> pointColors;

int animationIndex = 0;
bool isFilling = false;
bool isClosed = false;

// Forward declarations
void timerClear(int value);
Pixel interpolateFillColorFromEdges(int query_x, int query_y);

void setupProjection() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-gridSize, gridSize + 1.0, -gridSize, gridSize + 1.0);
}

void drawGrid() {
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);
    for (int i = -gridSize; i <= gridSize + 1; i++) {
        glVertex2f(i, -gridSize);
        glVertex2f(i, gridSize + 1);
        glVertex2f(-gridSize, i);
        glVertex2f(gridSize + 1, i);
    }
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-gridSize, 0);
    glVertex2f(gridSize + 1, 0);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0, -gridSize);
    glVertex2f(0, gridSize + 1);

    glEnd();
    glLineWidth(1.0f);

}

void drawPixels() {
    for (const auto& p : pixels) {
        if (p.isEndpointHighlight) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red for highlighted endpoints
        } else {
            glColor3f(p.r, p.g, p.b);
        }
        glBegin(GL_QUADS);
        glVertex2f(p.x, p.y);
        glVertex2f(p.x + 1.0f, p.y);
        glVertex2f(p.x + 1.0f, p.y + 1.0f);
        glVertex2f(p.x, p.y + 1.0f);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawPixels();
    glFlush();
}

std::pair<int, int> screenToGrid(int x, int y) {
    float worldX = (float(x) / windowWidth) * (2.0f * gridSize + 1.0f) - gridSize - 0.5f;
    float worldY = (float(windowHeight - 1 - y) / windowHeight) * (2.0f * gridSize + 1.0f) - gridSize - 0.5f;
    return std::make_pair((int)floor(worldX + 0.5f), (int)floor(worldY + 0.5f));
}

bool pointInPolygon(std::pair<int, int> pt, const std::vector<std::pair<int, int>>& poly_coords) {
    if (poly_coords.empty()) return false;
    int x = pt.first, y = pt.second;
    int cnt = 0;
    double ptx = x + 0.5; // Center of the pixel
    double pty = y + 0.5;

    for (size_t i = 0; i < poly_coords.size(); i++) {
        double x1 = poly_coords[i].first + 0.5;
        double y1 = poly_coords[i].second + 0.5;
        double x2 = poly_coords[(i + 1) % poly_coords.size()].first + 0.5;
        double y2 = poly_coords[(i + 1) % poly_coords.size()].second + 0.5;

        if (pty < std::min(y1, y2) || pty >= std::max(y1, y2)) continue;
        if (y1 == y2) continue;

        double xinters = (pty - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (xinters > ptx) {
            cnt++;
        }
    }
    return cnt % 2 != 0;
}


float distance(float x1, float y1, float x2, float y2) {
    return std::hypot(x1 - x2, y1 - y2);
}

void addLinearGradientLinePixels(int x0, int y0, float r0, float g0, float b0,
                                 int x1, int y1, float r1, float g1, float b1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0) return;

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(steps);
        float xf = static_cast<float>(x0) + static_cast<float>(dx) * t;
        float yf = static_cast<float>(y0) + static_cast<float>(dy) * t;
        int xi = static_cast<int>(std::round(xf));
        int yi = static_cast<int>(std::round(yf));
        float r = r0 * (1.0f - t) + r1 * t;
        float g = g0 * (1.0f - t) + g1 * t;
        float b = b0 * (1.0f - t) + b1 * t;
        pixels.push_back({xi, yi, r, g, b, false});
    }
}

std::vector<Pixel> currentFrameHighlights;
int lastHighlightedY = -gridSize -1;

void timerFill(int value) {
    if (isFilling && animationIndex < (int)fillQueue.size()) {
        const Pixel& nextPixelToFill = fillQueue[animationIndex];
        if (nextPixelToFill.y != lastHighlightedY && !currentFrameHighlights.empty()) {
            for(const auto& hp : currentFrameHighlights) {
                pixels.erase(std::remove_if(pixels.begin(), pixels.end(),
                    [&](const Pixel& p_in_pixels){
                        return p_in_pixels.x == hp.x && p_in_pixels.y == hp.y && p_in_pixels.isEndpointHighlight;
                    }), pixels.end());
            }
            currentFrameHighlights.clear();
        }
        lastHighlightedY = nextPixelToFill.y;

        if (nextPixelToFill.isEndpointHighlight) {
            pixels.erase(std::remove_if(pixels.begin(), pixels.end(),
                [&](const Pixel& p_in_pixels){
                    return p_in_pixels.x == nextPixelToFill.x && p_in_pixels.y == nextPixelToFill.y && !p_in_pixels.isEndpointHighlight;
                }), pixels.end());

            bool alreadyHighlighted = false;
            for(const auto& hp : currentFrameHighlights) {
                if(hp.x == nextPixelToFill.x && hp.y == nextPixelToFill.y) {
                    alreadyHighlighted = true;
                    break;
                }
            }
            if (!alreadyHighlighted) {
                pixels.push_back(nextPixelToFill);
                currentFrameHighlights.push_back(nextPixelToFill);
            }
        } else {
            pixels.push_back(nextPixelToFill);
        }

        animationIndex++;
        glutPostRedisplay();
        glutTimerFunc(1, timerFill, 0);
    } else {
        if (!currentFrameHighlights.empty()) {
             for(const auto& hp : currentFrameHighlights) {
                pixels.erase(std::remove_if(pixels.begin(), pixels.end(),
                    [&](const Pixel& p_in_pixels){
                        return p_in_pixels.x == hp.x && p_in_pixels.y == hp.y && p_in_pixels.isEndpointHighlight;
                    }), pixels.end());
            }
            currentFrameHighlights.clear();
        }
        isFilling = false;
        animationIndex = 0;
        lastHighlightedY = -gridSize - 1;
        glutPostRedisplay();
    }
}


void fillPolygonAnimated(const std::vector<std::pair<int, int>>& poly_coords) {
    fillQueue.clear();
    currentFrameHighlights.clear();
    lastHighlightedY = -gridSize - 1;

    if (poly_coords.empty() || poly_coords.size() < 3) return;

    int min_poly_y = gridSize + 1, max_poly_y = -gridSize - 1;
    int min_poly_x = gridSize + 1, max_poly_x = -gridSize - 1; // For boundary check
    for(const auto& p : poly_coords) {
        min_poly_y = std::min(min_poly_y, p.second);
        max_poly_y = std::max(max_poly_y, p.second);
        min_poly_x = std::min(min_poly_x, p.first);
        max_poly_x = std::max(max_poly_x, p.first);
    }


    for (int y_scan_idx = min_poly_y; y_scan_idx <= max_poly_y; y_scan_idx++) {
        std::vector<double> float_intersections_x;
        double current_scanline_y_center = static_cast<double>(y_scan_idx) + 0.5;

        for (size_t i = 0; i < poly_coords.size(); ++i) {
            const auto& p1_coord = poly_coords[i];
            const auto& p2_coord = poly_coords[(i + 1) % poly_coords.size()];

            double y1_center = static_cast<double>(p1_coord.second) + 0.5;
            double x1_center = static_cast<double>(p1_coord.first) + 0.5;
            double y2_center = static_cast<double>(p2_coord.second) + 0.5;
            double x2_center = static_cast<double>(p2_coord.first) + 0.5;

            if ((y1_center <= current_scanline_y_center && y2_center > current_scanline_y_center) ||
                (y2_center <= current_scanline_y_center && y1_center > current_scanline_y_center)) {
                double intersect_x_center = (current_scanline_y_center - y1_center) * (x2_center - x1_center) / (y2_center - y1_center) + x1_center;
                float_intersections_x.push_back(intersect_x_center);
            }
        }
        std::sort(float_intersections_x.begin(), float_intersections_x.end());

        for (size_t i = 0; i + 1 < float_intersections_x.size(); i += 2) {
            double true_x_start_center = float_intersections_x[i];
            double true_x_end_center = float_intersections_x[i+1];

            int x_fill_start_idx = static_cast<int>(std::ceil(true_x_start_center - 0.5));
            int x_fill_end_exclusive_idx = static_cast<int>(std::ceil(true_x_end_center - 0.5));


            if (x_fill_start_idx < x_fill_end_exclusive_idx) {
                Pixel start_h_strict = interpolateFillColorFromEdges(x_fill_start_idx, y_scan_idx);
                start_h_strict.isEndpointHighlight = true;
                fillQueue.push_back(start_h_strict);

                int end_strict_idx = x_fill_end_exclusive_idx;
                if (x_fill_start_idx != end_strict_idx) {
                    Pixel end_h_strict = interpolateFillColorFromEdges(end_strict_idx, y_scan_idx);
                    end_h_strict.isEndpointHighlight = true;
                    fillQueue.push_back(end_h_strict);
                }

                // Fill from x_fill_start_idx up to and including x_fill_end_exclusive_idx
                for (int x_idx = x_fill_start_idx; x_idx <= x_fill_end_exclusive_idx; ++x_idx) {
                     // Boundary check for x_idx to prevent excessive overdraw if logic is off
                     // This check might be too restrictive if the "extra" pixel is desired even outside poly bounds.
                     // For a simple "fill one more to the right" this check is helpful.
                     if (x_idx >= -gridSize && x_idx <= gridSize ) { // Basic grid boundary
                        Pixel fill_pixel = interpolateFillColorFromEdges(x_idx, y_scan_idx);
                        fill_pixel.isEndpointHighlight = false;
                        fillQueue.push_back(fill_pixel);
                     }
                }
            } else if (x_fill_start_idx == x_fill_end_exclusive_idx) {
                 // Original span is empty or single point based on strict rule.
                 // If we want to fill "one more", we fill x_fill_start_idx.
                 Pixel h_single = interpolateFillColorFromEdges(x_fill_start_idx, y_scan_idx);
                 h_single.isEndpointHighlight = true;
                 fillQueue.push_back(h_single);

                 if (x_fill_start_idx >= -gridSize && x_fill_start_idx <= gridSize) {
                    Pixel fill_pixel_single = interpolateFillColorFromEdges(x_fill_start_idx, y_scan_idx);
                    fill_pixel_single.isEndpointHighlight = false;
                    fillQueue.push_back(fill_pixel_single);
                 }
            }
        }
    }
    isFilling = true;
    animationIndex = 0;
    glutTimerFunc(0, timerFill, 0);
}


void clearAnimated() {
    clearingQueue.assign(pixels.begin(), pixels.end());
    if (clearingQueue.empty()) {
         pointColors.clear();
         isClosed = false;
         fillQueue.clear();
         currentFrameHighlights.clear();
         lastHighlightedY = -gridSize-1;
         glutPostRedisplay();
         return;
    }
    glutTimerFunc(0, timerClear, 0);
}

void timerClear(int value) {
    if (!clearingQueue.empty()) {
        clearingQueue.pop_back();
        pixels.assign(clearingQueue.begin(), clearingQueue.end());
        glutPostRedisplay();
        glutTimerFunc(5, timerClear, 0);
    } else {
        pixels.clear();
        pointColors.clear();
        isClosed = false;
        fillQueue.clear();
        currentFrameHighlights.clear();
        lastHighlightedY = -gridSize-1;
        glutPostRedisplay();
    }
}

bool isCloseToFirstPoint(int x, int y) {
    if (pointColors.empty()) return false;
    int fx = pointColors[0].x, fy = pointColors[0].y;
    return distance(static_cast<float>(x), static_cast<float>(y), static_cast<float>(fx), static_cast<float>(fy)) < 1.5;
}

Pixel interpolateFillColorFromEdges(int query_x, int query_y) {
    if (pointColors.empty()) return {query_x, query_y, 0.5f, 0.5f, 0.5f, false};

    float total_r = 0.0f, total_g = 0.0f, total_b = 0.0f;
    float total_weight = 0.0f;
    const float epsilon = 1e-4f;
    const float very_small_length_sq = 1e-5f;

    float q_px = static_cast<float>(query_x) + 0.5f;
    float q_py = static_cast<float>(query_y) + 0.5f;

    for (size_t i = 0; i < pointColors.size(); ++i) {
        const auto& p0_vertex = pointColors[i];
        const auto& p1_vertex = pointColors[(i + 1) % pointColors.size()];

        float p0x_center = static_cast<float>(p0_vertex.x) + 0.5f;
        float p0y_center = static_cast<float>(p0_vertex.y) + 0.5f;
        float p1x_center = static_cast<float>(p1_vertex.x) + 0.5f;
        float p1y_center = static_cast<float>(p1_vertex.y) + 0.5f;

        float edge_dx = p1x_center - p0x_center;
        float edge_dy = p1y_center - p0y_center;

        float r_on_edge, g_on_edge, b_on_edge;
        float dist_to_query_pt;

        float len_sq = edge_dx * edge_dx + edge_dy * edge_dy;

        if (len_sq < very_small_length_sq) {
            dist_to_query_pt = distance(q_px, q_py, p0x_center, p0y_center);
            r_on_edge = p0_vertex.r;
            g_on_edge = p0_vertex.g;
            b_on_edge = p0_vertex.b;
        } else {
            float t = ((q_px - p0x_center) * edge_dx + (q_py - p0y_center) * edge_dy) / len_sq;
            float t_clamped = std::max(0.0f, std::min(1.0f, t));

            float closest_x_on_segment = p0x_center + t_clamped * edge_dx;
            float closest_y_on_segment = p0y_center + t_clamped * edge_dy;

            r_on_edge = p0_vertex.r * (1.0f - t_clamped) + p1_vertex.r * t_clamped;
            g_on_edge = p0_vertex.g * (1.0f - t_clamped) + p1_vertex.g * t_clamped;
            b_on_edge = p0_vertex.b * (1.0f - t_clamped) + p1_vertex.b * t_clamped;

            dist_to_query_pt = distance(q_px, q_py, closest_x_on_segment, closest_y_on_segment);
        }

        float weight = 1.0f / (dist_to_query_pt + epsilon);
         if (dist_to_query_pt < epsilon) { // query_x, query_y is ON the edge
            return {query_x, query_y, r_on_edge, g_on_edge, b_on_edge, false};
        }

        total_r += r_on_edge * weight;
        total_g += g_on_edge * weight;
        total_b += b_on_edge * weight;
        total_weight += weight;
    }

    if (total_weight < epsilon) {
        if (!pointColors.empty())
            return {query_x, query_y, pointColors[0].r, pointColors[0].g, pointColors[0].b, false};
        return {query_x, query_y, 0.1f, 0.1f, 0.1f, false};
    }

    return {query_x, query_y, total_r / total_weight, total_g / total_weight, total_b / total_weight, false};
}


void mouse(int button, int state, int x, int y) {
    if (isFilling) return;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        std::pair<int, int> gp = screenToGrid(x, y);
        int gx = gp.first, gy = gp.second;

        if (gx < -gridSize || gx >= gridSize + 1 || gy < -gridSize || gy >= gridSize + 1) {
             std::cout << "Clicked outside valid grid area." << std::endl;
             return;
        }

        if (isClosed) {
            std::cout << "Polygon already closed. Press 'R' or 'C' to clear." << std::endl;
            return;
        }

        if (pointColors.size() >= 3 && isCloseToFirstPoint(gx, gy)) {
            isClosed = true;
            if (!pointColors.empty()) {
                 const auto& p_last = pointColors.back();
                 const auto& p_first = pointColors.front();
                 addLinearGradientLinePixels(p_last.x, p_last.y, p_last.r, p_last.g, p_last.b,
                                             p_first.x, p_first.y, p_first.r, p_first.g, p_first.b);
            }

            std::vector<std::pair<int, int>> poly_coords;
            for (const auto& p : pointColors) {
                poly_coords.push_back({p.x, p.y});
            }
            fillPolygonAnimated(poly_coords);
        } else {
            if (!pointColors.empty()) {
                const auto& p_last = pointColors.back();
                if (p_last.x == gx && p_last.y == gy) {
                    return;
                }
            }

            float r_rand = static_cast<float>(rand() % 101) / 100.0f;
            float g_rand = static_cast<float>(rand() % 101) / 100.0f;
            float b_rand = static_cast<float>(rand() % 101) / 100.0f;

            pixels.push_back({gx, gy, r_rand, g_rand, b_rand, false});
            pointColors.push_back({gx, gy, r_rand, g_rand, b_rand, false});

            if (pointColors.size() >= 2) {
                const auto& p_curr = pointColors.back();
                const auto& p_prev = pointColors[pointColors.size() - 2];
                addLinearGradientLinePixels(p_prev.x, p_prev.y, p_prev.r, p_prev.g, p_prev.b,
                                            p_curr.x, p_curr.y, p_curr.r, p_curr.g, p_curr.b);
            }
        }
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int, int) {
    if (key == 'r' || key == 'R') {
        isFilling = false;
        animationIndex = (int)fillQueue.size();
        glutTimerFunc(0, timerFill, 0);
        clearAnimated();
    }
    if (key == 'c' || key == 'C') {
        isFilling = false;
        animationIndex = (int)fillQueue.size();
        glutTimerFunc(0, timerFill, 0);

        pixels.clear();
        pointColors.clear();
        fillQueue.clear();
        clearingQueue.clear();
        currentFrameHighlights.clear();
        lastHighlightedY = -gridSize-1;
        isClosed = false;
        glutPostRedisplay();
    }
}

void selectFromMenu(int choice) {
    isFilling = false;
    animationIndex = (int)fillQueue.size();
    glutTimerFunc(0, timerFill, 0);


    switch (choice) {
        case MENU_GRID_10: gridSize = 10; break;
        case MENU_GRID_15: gridSize = 15; break;
        case MENU_GRID_20: gridSize = 20; break;
        case MENU_EXIT: exit(0);
    }
    setupProjection();

    pixels.clear();
    pointColors.clear();
    fillQueue.clear();
    clearingQueue.clear();
    currentFrameHighlights.clear();
    lastHighlightedY = -gridSize-1;
    isClosed = false;
    glutPostRedisplay();
}

int buildPopupMenu() {
    int menu = glutCreateMenu(selectFromMenu);
    glutAddMenuEntry("Grid 10x10", MENU_GRID_10);
    glutAddMenuEntry("Grid 15x15", MENU_GRID_15);
    glutAddMenuEntry("Grid 20x20", MENU_GRID_20);
    glutAddMenuEntry("Exit", MENU_EXIT);
    return menu;
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    setupProjection();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Polygon Fill - Extended Scan");

    setupProjection();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    buildPopupMenu();
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}