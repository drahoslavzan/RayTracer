//===================================================================
// File:        fps.h
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Dec 2 2011
// Comment:     Frame Per Second counter
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


#ifndef _FPS_H_
#define _FPS_H_


#include <cstdlib>
#include <sys/time.h>


class FPS
{
	static const size_t SAMPLES = 16;
private:
	size_t mSamples[SAMPLES];
	size_t mLast;
	size_t mUpdate;
	size_t mFrames;
	float  mFPS;
	struct timeval mTime;
public:
	FPS();

	void init();
	bool update();

	inline float frameRate() const { return mFPS; }
};


#endif /* _FPS_H_ */
