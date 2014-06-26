//===================================================================
// File:        materials.h
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


#ifndef _MATERIALS_H_
#define _MATERIALS_H_


#include "material.h"


class Solid : public Material
{
	MaterialInfo pi;
public:
	Solid(const MaterialInfo &info);
	MaterialInfo operator()(const Position &) const;
};


class Checkerboard : public Material
{
	MaterialInfo m1;
	MaterialInfo m2;
public:
	Checkerboard(const MaterialInfo &a, const MaterialInfo &b);
	MaterialInfo operator()(const Position &p) const;
};


class Marble : public Material
{
public:
	MaterialInfo operator()(const Position &p) const;
};


class Wood : public Material
{
public:
	MaterialInfo operator()(const Position &p) const;
};


#endif /* _MATERIALS_H_ */
