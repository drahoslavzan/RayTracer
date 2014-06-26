//===================================================================
// File:        object.cc
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Dec 2 2011
// Project:     Raytracer using CSG Scene Representation (RCSR)
//-------------------------------------------------------------------
// Copyright (C) 2011 Drahoslav Zan
//
// This file is part of RCSR.
//
// RCSR is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCSR is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RCSR. If not, see <http://www.gnu.org/licenses/>.
//===================================================================
// vim: set nowrap sw=2 ts=2


#include "object.h"


Object::Object(const Position &pos)
:
	position(pos)
{
}

Object::~Object()
{
}

void Object::setPosition(const Position &pos)
{
	position = pos;
}

const Position & Object::getPosition() const
{
	return position;
}

Light::Light(const Position &pos, const LightInfo &info, double i)
:
	Object(pos)
{
	setAttributes(info);
	setIntensity(i);
}

void Light::setAttributes(const LightInfo &info)
{
	pi = info;
}

void Light::setIntensity(double i)
{
	i = (i < 0.0) ? 0.0 : (i > 1.0) ? 1.0 : i;
	intensity = i;

	pi.ambient *= i;
	pi.diffuse *= i;
	pi.specular *= i;
}

const LightInfo & Light::getAttributes() const
{
	return pi;
}

double Light::getIntensity() const
{
	return intensity;
}

void Camera::updateObserver()
{
	observer = getPosition() - near * direction;
}

Camera::Camera(const Position &pos, const Direction &dir, double near)
:
	Object(pos)
{
	setDirection(dir);
	setNear(near);
}

void Camera::setDirection(const Direction &dir)
{
	direction = dir;

	if(dir[0] == 0 && dir[2] == 0)
		projection[0] = Direction(1.0, 0.0, 0.0);
	else
	{
		Quaternion<double> q(AngleAxis<double>(-PI_2, Direction(0.0, 1.0, 0.0)));

		projection[0] = q * dir;
		projection[0][1] = 0.0;
	}

	projection[1] = projection[0].cross(dir);

	direction.normalize();
	projection[0].normalize();
	projection[1].normalize();

	updateObserver();
}

void Camera::setNear(double n)
{
	if(n <= 0) n = 1.0;
	near = n;

	updateObserver();
}

void Camera::setPosition(const Position &pos)
{
	Object::setPosition(pos);

	updateObserver();
}

double Camera::getNear() const
{
	return near;
}

const Direction & Camera::getDirection() const
{
	return direction;
}

void Camera::setRay(Ray &r, double x, double y) const
{
	Position p = getPosition() + x * projection[0] + y * projection[1];

	r.setBase(p);
	r.setDirection(p - observer);
}

