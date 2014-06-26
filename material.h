//===================================================================
// File:        material.h
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


#ifndef _MATERIAL_H_
#define _MATERIAL_H_


#include <map>

#include "math.h"
#include "object.h"


struct MaterialInfo
{
	Color diffuse;
	Color specular;
	Color ambient;
	Color emission;
	double reflectance; 		// in <0, 1>
	double transparency; 		// in <0, 1>
	double shininess;
	double refraction; 			// index: n1 / n2
public:
	MaterialInfo(const Color &d = Color::Constant(0.0),
			const Color &s = Color::Constant(0.0),
			const Color &a = Color::Constant(0.0),
			const Color &e = Color::Constant(0.0),
			double r = 0.0, double t = 0.0,
			double i = 0.0, double n = 0.0)
	:
		diffuse(d), specular(s), ambient(a), emission(e),
		reflectance(r), transparency(t), shininess(i), refraction(n)
	{}
};


class Material
{
public:
	virtual ~Material() = 0;
	virtual MaterialInfo operator()(const Position &) const;
};


class MaterialStack
{
	typedef std::map<const char *, const Material *> stack_t;
private:
	static MaterialStack singleton;
	static stack_t stack;
private:
	~MaterialStack();
public:
	static void add(const char *name, const Material *m);
	static const Material * get(const char *name);
};


#endif /* _MATERIAL_H_ */
