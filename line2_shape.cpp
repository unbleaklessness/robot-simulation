#include "line2_shape.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

template <typename T>
line2_shape<T>::line2_shape(const std::vector<line2<T>> &lines) {
	this->lines = lines;
}

template <typename T>
void line2_shape<T>::draw() const {
	glColor3f(r, g, b);
	glBegin(GL_LINES);
	for (const auto &line : lines) {
		glVertex2f(line.getFirst().getX(), line.getFirst().getY());
		glVertex2f(line.getSecond().getX(), line.getSecond().getY());
	}
	glEnd();
}

template <typename T>
void line2_shape<T>::set_color(const float &r, const float &g, const float &b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

template class line2_shape<int>;
template class line2_shape<float>;