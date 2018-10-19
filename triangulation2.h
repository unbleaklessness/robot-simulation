#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif
#include <GL/glut.h>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>

#include "point.h"

template <typename T>
using line2 = std::pair<point2<T>, point2<T>>;

typedef line2<int> line2i;
typedef line2<float> line2f;

namespace {
	enum side {
		left = 0,
		right = 1
	};
}

template <typename T>
static side get_side(
    const line2<T> &line,
    const point2<T> &point)
{
    T value = (point.getX() - line.first.getX()) * (line.second.getY() - line.first.getY()) -
        (point.getY() - line.first.getY()) * (line.second.getX() - line.first.getX());
    return value <= 0 ? side::left : side::right;
}

template <typename T>
static typename std::list<T>::iterator next_iter(
    typename std::list<T>::iterator it,
    const typename std::list<T>::iterator &begin,
    const typename std::list<T>::iterator &end)
{
    it++;
    if (it == end) {
        it = begin;
    }
    return it;
}

template <typename T>
static typename std::list<T>::iterator prev_iter(
    typename std::list<T>::iterator it,
    const typename std::list<T>::iterator &begin,
    const typename std::list<T>::iterator &end)
{
    if (it == begin) {
        it = end;
    }
    it--;
    return it;
}

template <typename T>
void draw_lines(const std::vector<line2<T>> &lines) {
    glBegin(GL_LINES);
    for (const auto &line : lines) {
		glVertex2f(line.first.getX(), line.first.getY());
		glVertex2f(line.second.getX(), line.second.getY());
    }
    glEnd();
}

template <typename T>
std::vector<line2<T>> *triangulate(
    std::vector<point2<T>> &points)
{
    if (points.size() < 3) return nullptr;

    std::sort(points.begin(), points.end());

    auto result = new std::vector<line2<T>>;
    std::list<line2<T>> hull;

    if (get_side(line2<T>(points[0], points[1]), points[2]) == side::left) {
        hull.push_back(line2<T>(points[0], points[1]));
        hull.push_back(line2<T>(points[1], points[2]));
        hull.push_back(line2<T>(points[2], points[0]));
    } else {
        hull.push_back(line2<T>(points[1], points[0]));
        hull.push_back(line2<T>(points[0], points[2]));
        hull.push_back(line2<T>(points[2], points[1]));
    }
	result->push_back(line2<T>(points[0], points[1]));
	result->push_back(line2<T>(points[1], points[2]));
	result->push_back(line2<T>(points[2], points[0]));

    for (size_t i = 3; i < points.size(); i++) {
        auto it = hull.begin();
        while (true) {
            if (side::right == get_side(*it, points[i])) {
				result->push_back(line2<T>(it->first, points[i]));
				result->push_back(line2<T>(points[i], it->second));
				result->push_back(line2<T>(it->second, it->first));

                auto top_line = line2<T>(points[i], it->second);
                auto bottom_line = line2<T>(it->first, points[i]);

                auto top_line_r = line2<T>(it->second, points[i]);
                auto bottom_line_r = line2<T>(points[i], it->first);

                it = prev_iter<line2<T>>(it, hull.begin(), hull.end());
                if (*it == bottom_line_r)
                {
                    if ((it = hull.erase(it)) == hull.end()) {
                        it = hull.begin();
                    }
                } else {
                    it = next_iter<line2<T>>(it, hull.begin(), hull.end());
                    it = hull.insert(it, bottom_line);
                    it = next_iter<line2<T>>(it, hull.begin(), hull.end());
                }

                it = next_iter<line2<T>>(it, hull.begin(), hull.end());
                if (*it == top_line_r)
                {
                    if ((it = hull.erase(it)) == hull.end()) {
                        it = hull.begin();
                    }
                    it = prev_iter<line2<T>>(it, hull.begin(), hull.end());
                } else {
                    it = hull.insert(it, top_line);
                    it = prev_iter<line2<T>>(it, hull.begin(), hull.end());
                }

                if ((it = hull.erase(it)) == hull.end()) {
                    it = hull.begin();
                }
            }
            it = next_iter<line2<T>>(it, hull.begin(), hull.end());
            if (it == hull.begin()) {
                break;
            }
        }
    }

    return result;
}
