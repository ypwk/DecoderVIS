#include "RotatedPlanarCode.h"

RotatedPlanarCode::RotatedPlanarCode(int dist)
{
    distance = dist;
}

unsigned int RotatedPlanarCode::render()
{
	int i;
	int lineAmount = 100; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * 3.141592;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= lineAmount; i++) {
		glVertex2f(
			0 + (200 * cos(i * twicePi / lineAmount)),
			0 + (200 * sin(i * twicePi / lineAmount))
		);
	}
	glEnd();
	return 0;
}
