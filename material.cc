//===================================================================
// File:        material.cc
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


#include "material.h"


MaterialStack MaterialStack::singleton;
MaterialStack::stack_t MaterialStack::stack;


Material::~Material()
{
}

MaterialInfo Material::operator()(const Position &) const
{
	abort(); 	// virtual
}

MaterialStack::~MaterialStack()
{
	for(stack_t::iterator it = stack.begin(); it != stack.end(); ++it)
		delete it->second;
}

void MaterialStack::add(const char *name, const Material *m)
{
	stack[name] = m;
}

const Material * MaterialStack::get(const char *name)
{
	stack_t::iterator it = stack.find(name);
	assert(it != stack.end());
	return it->second;
}

