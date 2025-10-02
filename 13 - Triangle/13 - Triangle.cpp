#include <iostream>
#include <vector>
#include <cmath>

//точки
struct Point {
    double x, y;
};

//треуг
struct Triangle {
    Point p1, p2, p3;
};


// Вычисляет Z-компоненту
// > 0:  против часовой стрелки
// < 0: по часовой стрелке
// = 0:
double cross_product(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

// находится ли точка p внутри треугольника t
bool is_inside_triangle(Triangle t, Point p) {
    //векторные произведения для каждой стороны треугольника
    double d1 = cross_product(t.p1, t.p2, p);
    double d2 = cross_product(t.p2, t.p3, p);
    double d3 = cross_product(t.p3, t.p1, p);

    // проверка точка внутри или на границе
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

//полигон - акт вектор для трианг
//алгоритм триангуляции
std::vector<Triangle> triangulate(std::vector<Point> polygon) {
    std::vector<Triangle> triangles;

    // пока >3 вершин
    while (polygon.size() > 3) {
        bool ear_found = false;
        for (int i = 0; i < polygon.size(); ++i) {
            //три последовательные вершины
            Point p_prev = polygon[(i == 0) ? polygon.size() - 1 : i - 1];
            Point p_tip = polygon[i];
            Point p_next = polygon[(i + 1) % polygon.size()];

            Triangle potential_ear = { p_prev, p_tip, p_next };

            if (cross_product(p_prev, p_tip, p_next) <= 0) {
                continue;
            }

            //не лежат ли другие вершины внутри этого треугольника
            bool is_valid_ear = true;
            for (const auto& p_check : polygon) {
                //пропускаем вершины уха
                if ((p_check.x == p_prev.x && p_check.y == p_prev.y) ||
                    (p_check.x == p_tip.x && p_check.y == p_tip.y) ||
                    (p_check.x == p_next.x && p_check.y == p_next.y)) {
                    continue;
                }
                if (is_inside_triangle(potential_ear, p_check)) {
                    is_valid_ear = false;
                    break;
                }
            }

            //если оба - отсекаем
            if (is_valid_ear) {
                triangles.push_back(potential_ear);
                polygon.erase(polygon.begin() + i);
                ear_found = true;
                break; //поиск уха заново с измененного многоугольника
            }
        }

    }

    //последний оставшийся треугольник
    triangles.push_back({ polygon[0], polygon[1], polygon[2] });

    return triangles;
}

int main() {
    std::vector<Point> hexagon = {
        {1, 0}, {3, 0}, {4, 1}, {3, 2}, {1, 2}, {0, 1}
    };

    std::cout << "Triangulating a hexagon..." << std::endl;

    std::vector<Triangle> result_triangles = triangulate(hexagon);

    std::cout << "Resulting triangles (" << result_triangles.size() << " pieces):" << std::endl;
    int count = 1;
    for (const auto& t : result_triangles) {
        std::cout << "Triangle " << count++ << ": "
            << "{(" << t.p1.x << "," << t.p1.y << "), "
            << "(" << t.p2.x << "," << t.p2.y << "), "
            << "(" << t.p3.x << "," << t.p3.y << ")}" << std::endl;
    }

    return 0;
}