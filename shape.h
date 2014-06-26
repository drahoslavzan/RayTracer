// ### HEADER ### //

#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "object.h"
#include "material.h"
#include "ray.h"

class Shape : public Object
{
	const Material *material;
public:
	Shape(const Position &pos, const Material *m);
	virtual ~Shape() = 0;
	virtual double intersect(const Ray &) const;
	virtual Direction getNormal(const Position &) const;
	void setMaterial(const Material *m);
	const Material & getMaterial() const;
};

class Sphere : public Shape
{
	double r;
public:
	Sphere(const Position &pos, double radius, const Material *m);
	void setRadius(double radius);
	double getRadius() const;
	Direction getNormal(const Position &p) const;
	double intersect(const Ray &ray) const;
};

class Plane : public Shape
{
	Direction normal;
	double distance;
public:
	Plane(const Position &pos, const Direction &n, const Material *m);
	Direction getNormal(const Position &) const;
	double intersect(const Ray &ray) const;
};

class Cylinder : public Shape
{
	Direction d;
	double r;
public:
	Cylinder(const Position &pos, const Direction &dir, double radius,
			const Material *m);
	void setRadius(double radius);
	void setDirection(const Direction &dir);
	double getRadius() const;
	const Direction & getDirection() const;
	Direction getNormal(const Position &p) const;
	double intersect(const Ray &ray) const;
};

#endif /* _SHAPE_H_ */
