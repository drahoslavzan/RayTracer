// ### HEADER ### //

#include "shape.h"
#include "math.h"

#include <cassert>

Shape::Shape(const Position &pos, const Material *m)
:
	Object(pos)
{
	setMaterial(m);
}

Shape::~Shape()
{
	delete material;
}

double Shape::intersect(const Ray &) const
{
	abort(); 		// virtual
}

Direction Shape::getNormal(const Position &) const
{
	abort(); 		// virtual
}

void Shape::setMaterial(const Material *m)
{
	assert(m != NULL);
	material = m;
}

const Material & Shape::getMaterial() const
{
	return *material;
}

Sphere::Sphere(const Position &pos, double radius, const Material *m)
:
	Shape(pos, m)
{
	setRadius(radius);
}

void Sphere::setRadius(double radius)
{
	r = radius;
}

double Sphere::getRadius() const
{
	return r;
}

Direction Sphere::getNormal(const Position &p) const
{
	return (p - getPosition()).normalized();
}

double Sphere::intersect(const Ray &ray) const
{
	Direction q = getPosition() - ray.getBase();
	double b = ray.getDirection().dot(q);
	double d = b * b - q.dot(q) + r * r;

	if(d < 0)
		return -1;
	return b - sqrt(d);
}

Plane::Plane(const Position &pos, const Direction &n, const Material *m)
:
	Shape(pos, m)
{
	normal = n;
	normal.normalize();
	distance = sqrt(pos.dot(pos));
}

Direction Plane::getNormal(const Position &) const
{
	return normal;
}

double Plane::intersect(const Ray &ray) const
{
	double vd = normal.dot(ray.getDirection());

	if(vd >= 0)
		return -1;

	double v0 = -(normal.dot(ray.getBase()) + distance);

	double r = v0 / vd;

	return r;
}

Cylinder::Cylinder(const Position &pos, const Direction &dir, double radius,
			const Material *m)
:
	Shape(pos, m)
{
	setDirection(dir);
	setRadius(radius);
}

void Cylinder::setRadius(double radius)
{
	r = radius;
}

void Cylinder::setDirection(const Direction &dir)
{
	d = dir;
}

double Cylinder::getRadius() const
{
	return r;
}

const Direction & Cylinder::getDirection() const
{
	return d;
}

Direction Cylinder::getNormal(const Position &p) const
{
	Direction u = getPosition() + (p - getPosition()).dot(d) * d;
	return (p - u).normalized();
}

double Cylinder::intersect(const Ray &ray) const
{
	Direction rc = ray.getBase() - getPosition();
	double bd = ray.getDirection().dot(d);
	double b2 = ray.getDirection().dot(ray.getDirection());
	double cb = rc.dot(ray.getDirection());
	double cd = rc.dot(d);
	double c2 = rc.dot(rc);

	double a = b2 - bd * bd;
	double b = 2 * cb - 2 * cd * bd;
	double c = c2 - cd * cd - r * r;

	double d = b * b - 4 * a * c;

	if(d < 0)
		return -1;
	return (-b - sqrt(d)) / (2 * a);
}

