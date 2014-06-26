// ### HEADER ### //


#include "scene.h"

#include <cassert>
#include <omp.h>


void Scene::figure(int x, int y, const Color &pixel) const
{
	pixmap[3 * y * w + 3 * x + 0] = 255 * pixel[0];
	pixmap[3 * y * w + 3 * x + 1] = 255 * pixel[1];
	pixmap[3 * y * w + 3 * x + 2] = 255 * pixel[2];
}

Color Scene::getPixel(int x, int y) const
{
	return Color(pixmap[3 * y * w + 3 * x + 0] / 255.0,
			pixmap[3 * y * w + 3 * x + 1] / 255.0,
			pixmap[3 * y * w + 3 * x + 2] / 255.0);
}

double Scene::findClosestShape(const Ray &r, const Shape * &s) const
{
	s = NULL;
	double close = -1;

	shape_t::const_iterator it = shapes.begin();
	for(; it != shapes.end(); ++it)
	{
		double t = (*it)->intersect(r);
		if(t >= 0)
		{
			close = t;
			s = *it;
			++it;
			break;
		}
	}
	for(; it != shapes.end(); ++it)
	{
		double t = (*it)->intersect(r);
		if(t >= 0 && t < close)
		{
			close = t;
			s = *it;
		}
	}

	return close;
}

void Scene::trace(const Ray &r, Color &c, int depth) const
{
	const Shape *s;
	double close = findClosestShape(r, s);

	if(close < 0) 		// no intersection
	{
		c = uvColor;
		return;
	}

	assert(s != NULL);

	Position p = r(close);

	MaterialInfo info = s->getMaterial()(p - s->getPosition());

	Direction rdNorm = r.getDirection().normalized();
	Direction normal = s->getNormal(p);

	Ray lr;

	for(light_t::const_iterator it = lights.begin(); it != lights.end(); ++it)
	{
		const Light &light = *(*it);
		const LightInfo &lightInfo = light.getAttributes();

		lr.setBase(light.getPosition());
		lr.setDirection(p - light.getPosition());

		const Shape *v;

		if(findClosestShape(lr, v) >= 0 && s == v)
		{
			Direction srdNorm = -lr.getDirection().normalized();

			Direction reflect  = 2 * normal * normal.dot(srdNorm) - srdNorm;
			reflect.normalize();

			double df = normal.dot(srdNorm);
			if(df < 0.0) df = 0.0;

			//double rf = reflect.dot(-rdNorm); 			// Phong

			Direction h = srdNorm - rdNorm;
			h.normalize();
			double rf = h.dot(normal); 								// Blinn-Phong

			if(rf < 0.0) rf = 0.0;
			else rf = pow(rf, info.shininess);

			c[0] += info.emission[0] + info.ambient[0] * lightInfo.ambient[0]
				+ info.diffuse[0] * lightInfo.diffuse[0] * df
				+ info.specular[0] * lightInfo.specular[0] * rf;
			c[1] += info.emission[1] + info.ambient[1] * lightInfo.ambient[1]
				+ info.diffuse[1] * lightInfo.diffuse[1] * df
				+ info.specular[1] * lightInfo.specular[1] * rf;
			c[2] += info.emission[2] + info.ambient[2] * lightInfo.ambient[2]
				+ info.diffuse[2] * lightInfo.diffuse[2] * df
				+ info.specular[2] * lightInfo.specular[2] * rf;
		}
		else 					// in shadow
		{
			c[0] += info.emission[0] + info.ambient[0] * lightInfo.ambient[0];
			c[1] += info.emission[1] + info.ambient[1] * lightInfo.ambient[1];
			c[2] += info.emission[2] + info.ambient[2] * lightInfo.ambient[2];
		}
	}

	c[0] = (c[0] <= 1.0) ? c[0] : 1.0;
	c[1] = (c[1] <= 1.0) ? c[1] : 1.0;
	c[2] = (c[2] <= 1.0) ? c[2] : 1.0;

	if(--depth < 0)
		return;

	if(info.transparency > 0)
	{
		double n = info.refraction;
		double cosI = normal.dot(rdNorm);

		double sinT2 = n * n * (1.0 - cosI * cosI);

		if(sinT2 <= 1.0)
		{
			lr.setBase(p + EPSILON * rdNorm);
			lr.setDirection(n * rdNorm - (n + sqrt(1.0 - sinT2)) * normal);

			Color nc(Color::Constant(0.0));

			trace(lr, nc, depth);

			c = (1.0 - info.transparency) * c + info.transparency * nc;
		}
	}

	if(info.reflectance > 0)
	{
		lr.setBase(p);
		lr.setDirection(rdNorm - 2 * normal * normal.dot(rdNorm));

		Color nc(Color::Constant(0.0));

		trace(lr, nc, depth);

		c = (1.0 - info.reflectance) * c + info.reflectance * nc;
	}
}

Scene::Scene(Camera *c, int width, int height, const Color &u, int d)
:
	camera(c), pixmap(NULL)
{
	setViewPort(width, height);
	setUniverseColor(u);
	setDepth(d);
}

Scene::~Scene()
{
	delete [] pixmap;

	delete camera;

	for(light_t::iterator it = lights.begin(); it != lights.end(); ++it)
		delete *it;
	for(shape_t::iterator it = shapes.begin(); it != shapes.end(); ++it)
		delete *it;
}

void Scene::setViewPort(int width, int height)
{
	delete [] pixmap;

	w = width;
	h = height;

	aspectRatio = double(w) / double(h);

	pixmap = new unsigned char[w * h * 3];
}

void Scene::setUniverseColor(const Color &u)
{
	uvColor = u;
}

void Scene::setDepth(int d)
{
	depth = d;
}

void Scene::addLight(Light *l)
{
	lights.push_back(l);
}

void Scene::addShape(Shape *s)
{
	shapes.push_back(s);
}

int Scene::getWidth() const
{
	return w;
}

int Scene::getHeight() const
{
	return h;
}

int Scene::getDepth() const
{
	return depth;
}

double Scene::getAspectRatio() const
{
	return aspectRatio;
}

Camera & Scene::getCamera() const
{
	return *camera;
}

unsigned char * Scene::render() const
{
	double aspect = 2.0 / aspectRatio;

#pragma omp parallel for schedule(static)
	for(int i = 0; i < h; ++i)
	{
		Ray r;
		Color pixel;
		Camera cam = *camera;

		for(int j = 0; j < w; ++j)
		{
			double x = -1.0 + j * (2.0 / w);
			double y = -(aspect / 2) + i * (aspect / h);

			cam.setRay(r, x, y);
			pixel.setZero();

			trace(r, pixel, depth);

			figure(j, i, pixel);
		}
	}

	return pixmap;
}

