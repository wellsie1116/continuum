/* vect.c

   Copyright (C) 2009 - Kevin Wells <kevin@wellsie.net>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "vect.h"

#include <math.h>


Vector3*
vect_normalize(Vector3* vector)
{
	float length = vect_magnitude(*vector);
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
	return vector;
}

Vector3*
vect_scale(Vector3* vector, float factor)
{
	vector->x *= factor;
	vector->y *= factor;
	vector->z *= factor;
	return vector;
}

Vector3*
vect_crossproduct(Vector3 vector1, Vector3 vector2, Vector3* result)
{
	result->x = vector1.y * vector2.z - vector1.z * vector2.y;
	result->y = vector1.z * vector2.x - vector1.x * vector2.z;
	result->z = vector1.x * vector2.y - vector1.y * vector2.x;
	return result;
}

float
vect_magnitude(Vector3 vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float
vect_dot(Vector3 vector1, Vector3 vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

Vector3*
vect_add(Vector3 vector1, Vector3 vector2, Vector3* result)
{
	result->x = vector1.x + vector2.x;
	result->y = vector1.y + vector2.y;
	result->z = vector1.z + vector2.z;
	return result;
}

Vector3*
vect_subtract(Vector3 vector1, Vector3 vector2, Vector3* result)
{
	result->x = vector1.x - vector2.x;
	result->y = vector1.y - vector2.y;
	result->z = vector1.z - vector2.z;
	return result;
}

Vector3*
vect_project(Vector3 vector1, Vector3 vector2, Vector3* result)
{
	*result = vector2;
	vect_scale(result, vect_dot(vector1, vector2) / vect_dot(vector2, vector2));
	return result;
}

Vector3*
vect_from_points(Point3 p1, Point3 p2, Vector3* result)
{
	result->x = p2.x - p1.x;
	result->y = p2.y - p1.y;
	result->z = p2.z - p1.z;
	return result;
}
