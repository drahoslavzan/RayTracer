// ### HEADER ### //

#ifndef _RAY_H_
#define _RAY_H_

#include "math.h"

class Ray
{
	Position b;
	Position d;
public:
	Ray();
	Ray(const Position &base, const Direction &direction);
	void setBase(const Position &base);
	void setDirection(const Direction &direction);
	const Position & getBase() const;
	const Direction & getDirection() const;
	Position operator()(double t) const;
};

#endif /* _RAY_H_ */
