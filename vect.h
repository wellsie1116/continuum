
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

typedef double Quaternion[4];

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

void quat_identity(Quaternion q);
void quat_init(Quaternion q, double ax, double ay, double az, double angle);

//void dRfromQ (dMatrix3 R, const Quaternion q);
//void dQfromR (Quaternion q, const dMatrix3 R);

/* qa = rotate by qc, then qb */
void quat_multiply0(Quaternion qa, const Quaternion qb, const Quaternion qc);

/* qa = rotate by qc, then by inverse of qb */
void quat_multiply1(Quaternion qa, const Quaternion qb, const Quaternion qc);

/* qa = rotate by inverse of qc, then by qb */
void quat_multiply2(Quaternion qa, const Quaternion qb, const Quaternion qc);

/* qa = rotate by inverse of qc, then by inverse of qb */
void quat_multiply3(Quaternion qa, const Quaternion qb, const Quaternion qc);



#endif /* VECT_H_ */
