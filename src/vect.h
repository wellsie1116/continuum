/* vect.h

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


#ifndef VECT_H_
#define VECT_H_

typedef struct
{
	float x;
	float y;
	float z;
} Vector3;

typedef Vector3 Point3;  //same data, different concept
typedef Point3 Euler3;    //rotational information

//most of these methods do not need descriptions, they do what you would expect with vectors
//one thing worth mentioning is that vectors passed byref are modified, and that reference is returned

Vector3* vect_normalize(Vector3* vector);
Vector3* vect_scale(Vector3* vector, float factor);
Vector3* vect_crossproduct(Vector3 vector1, Vector3 vector2, Vector3* result);
float vect_magnitude(Vector3 vector);
float vect_dot(Vector3 vector1, Vector3 vector2);
Vector3* vect_add(Vector3 vector1, Vector3 vector2, Vector3* result);
Vector3* vect_subtract(Vector3 vector1, Vector3 vector2, Vector3* result);
Vector3* vect_project(Vector3 vector1, Vector3 vector2, Vector3* result);

Vector3* vect_from_points(Point3 p1, Point3 p2, Vector3* result);

#endif /* VECT_H_ */
