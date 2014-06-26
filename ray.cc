// ### HEADER ### //

#include "ray.h"

Ray::Ray()
{
}

Ray::Ray(const Position &base, const Direction &direction)
{
	setBase(base);
	setDirection(direction);
}

void Ray::setBase(const Position &base)
{
	b = base;
}

void Ray::setDirection(const Direction &direction)
{
	d = direction;
	d.normalize();
}

const Position & Ray::getBase() const
{
	return b;
}

const Direction & Ray::getDirection() const
{
	return d;
}

Position Ray::operator()(double t) const
{
	return b + t * d;
}

