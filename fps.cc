//===================================================================
// File:        fps.cc
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


#include <cstring>

#include "fps.h"


FPS::FPS()
{
	init();
}

void FPS::init()
{
	memset(mSamples, 0, sizeof(mSamples));

	gettimeofday(&mTime, NULL);

	mLast = mTime.tv_sec * 1000.0 + mTime.tv_usec / 1000.0 + 0.5;

	mUpdate = 0;
	mFPS    = 0;
	mFrames = 0;
}

bool FPS::update()
{
	size_t index = mFrames % SAMPLES;

	gettimeofday(&mTime, NULL);

	size_t ticks = mTime.tv_sec * 1000.0 + mTime.tv_usec / 1000.0 + 0.5;

	mSamples[index] = ticks - mLast;

	mLast = ticks;

	++mFrames;

	if(ticks - mUpdate <= 1000)
		return false;

	mUpdate = ticks;

	size_t count = (mFrames < SAMPLES) ? mFrames : SAMPLES;

	mFPS = 0;

	for(size_t i = 0; i < count; ++i)
		mFPS += mSamples[i];

	mFPS /= count;

	mFPS = 1000.f / mFPS;

	return true;
}

