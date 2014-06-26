// ### HEADER ### //

#ifndef _SCENE_H_
#define _SCENE_H_


#include "object.h"
#include "shape.h"
#include "ray.h"
#include "math.h"

#include <list>


class Scene
{
	typedef std::list<Light *> light_t;
	typedef std::list<Shape *> shape_t;
private:
	int w, h;
	double aspectRatio;
	Color uvColor;
	Camera *camera;
	light_t lights;
	shape_t shapes;
	int depth;
protected:
	unsigned char *pixmap;
protected:
	void figure(int x, int y, const Color &pixel) const;
	Color getPixel(int x, int y) const;
	void trace(const Ray &r, Color &c, int depth) const;
	double findClosestShape(const Ray &r, const Shape *&s) const;
public:
	Scene(Camera *c, int width, int height,
			const Color &u = Color::Constant(0.0), int d = 2);
	~Scene();
	void setViewPort(int width, int height);
	void setUniverseColor(const Color &u);
	void setDepth(int d);
	void addLight(Light *l);
	void addShape(Shape *s);
	int getWidth() const;
	int getHeight() const;
	int getDepth() const;
	double getAspectRatio() const;
	Camera & getCamera() const;
	virtual unsigned char * render() const;
};


#endif /* _SCENE_H_ */
