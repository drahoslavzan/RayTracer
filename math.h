//===================================================================
// File:        math.h
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


#ifndef _MATH_H_
#define _MATH_H_


#include <cmath>

#include <Eigen/Core>
#include <Eigen/Geometry>


using namespace Eigen;


static const double EPSILON = 1e-6;

static const double PI   = 3.14159265358979323846;
static const double PI_2 = 1.57079632679489661923;
static const double PI_4 = 0.78539816339744830962;


typedef Vector3d Position;
typedef Vector3d Direction;

typedef Vector3d Color;


#endif /* _MATH_H_ */
