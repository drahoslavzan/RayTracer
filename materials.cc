//===================================================================
// File:        materials.cc
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


#include <noise/noise.h>

#include "materials.h"


Solid::Solid(const MaterialInfo &info)
:
	pi(info)
{
}

MaterialInfo Solid::operator()(const Position &) const
{
	return pi;
}


Checkerboard::Checkerboard(const MaterialInfo &a, const MaterialInfo &b)
:
	m1(a), m2(b)
{
}

MaterialInfo Checkerboard::operator()(const Position &p) const
{
	return ((int(floor(p.x() + 1e-10)) ^
		int(floor(p.y() + 1e-10)) ^
		int(floor(p.z() + 1e-10))) & 1) ? m1 : m2;
}


MaterialInfo Marble::operator()(const Position &p) const
{
	MaterialInfo m;
	noise::module::Perlin noise;

	double s = 30.0;
	double n = noise.GetValue(p.x(), p.y(), p.z()) ;
	double v = (sin(s * p.x()) - sin(s * p.z()) + n + 2.5) / 5.0;

	m.diffuse.setConstant(v);
	m.ambient.setConstant(v * .05);
	m.specular.setConstant(v * .5);
	m.shininess = 15.0;

	return m;
}


MaterialInfo Wood::operator()(const Position &p) const
{
	MaterialInfo m;
	noise::module::Perlin noise;

	double s = 1.0;
	double n = noise.GetValue(p.x(), p.y(), p.z()) * s;
	double v = fabs(n - int(n));

	m.diffuse[0] = v;
	m.diffuse[1] = v * 0.5;
	m.diffuse[2] = v * 0.5 * 0.35;
	m.ambient[0] = v * 0.05;
	m.ambient[1] = v * 0.5 * 0.05;
	m.ambient[2] = v * 0.5 * 0.35 * 0.05;	
	m.shininess = 10.0;

	return m;
}

