// ### HEADER ### //

#ifndef _OBJECT_H_
#define _OBJECT_H_


#include "math.h"
#include "ray.h"


struct LightInfo
{
	Color diffuse;
	Color specular;
	Color ambient;
public:
	LightInfo(const Color &d = Color::Constant(1.0),
			const Color &s = Color::Constant(1.0),
			const Color &a = Color::Constant(1.0))
	:
		diffuse(d), specular(s), ambient(a)
	{}
};


class Object
{
	Position position;
public:
	Object(const Position &pos);
	virtual ~Object() = 0;
	virtual void setPosition(const Position &pos);
	const Position & getPosition() const;
};


class Light : public Object
{
	LightInfo pi;
	double intensity; 		// in <0, 1>
public:
	Light(const Position &pos, const LightInfo &info,
			double i = 1.0);
	void setAttributes(const LightInfo &info);
	void setIntensity(double i);
	const LightInfo & getAttributes() const;
	double getIntensity() const;
};


class Camera : public Object
{
	Direction direction;
	Direction projection[2];
	double near;
	Position observer;
private:
	void updateObserver();
public:
	Camera(const Position &pos, const Direction &dir, double near);
	void setDirection(const Direction &dir);
	void setNear(double n);
	void setPosition(const Position &pos);
	double getNear() const;
	const Direction & getDirection() const;
	void setRay(Ray &r, double x, double y) const;
};


#endif /* _OBJECT_H_ */
