/** @file GuillotineBinPack.cpp
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the GUILLOTINE data structure.

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>
#include <iostream>
#include <limits>

#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "GuillotineBinPack.h"

namespace rbp {

using namespace std;

GuillotineBinPack::GuillotineBinPack()
:binWidth(0),
binHeight(0)
{
}

GuillotineBinPack::GuillotineBinPack(value_type width, value_type height)
{
	Init(width, height);
}

void GuillotineBinPack::Init(value_type width, value_type height)
{
	binWidth = width;
	binHeight = height;

#ifdef _DEBUG
	disjointRects.Clear();
#endif

	// Clear any memory of previously packed rectangles.
	usedRectangles.clear();

	// We start with a single big free rectangle that spans the whole bin.
	Rect n;
	n.x = 0;
	n.y = 0;
    n.size.width = width;
    n.size.height = height;

	freeRectangles.clear();
	freeRectangles.push_back(n);
}

void GuillotineBinPack::Insert(std::vector<RectSize> &rects, bool merge, 
	FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod)
{
	// Remember variables about the best packing choice we have made so far during the iteration process.
    size_t bestFreeRect = 0;
    size_t bestRect = 0;
	bool bestFlipped = false;

	// Pack rectangles one at a time until we have cleared the rects array of all rectangles.
	// rects will get destroyed in the process.
	while(rects.size() > 0)
	{
		// Stores the penalty score of the best rectangle placement - bigger=worse, smaller=better.
        value_type bestScore = std::numeric_limits<value_type>::max();

        for(size_t i = 0; i < freeRectangles.size(); ++i)
		{
            for(size_t j = 0; j < rects.size(); ++j)
			{
				// If this rectangle is a perfect match, we pick it instantly.
                if (abs(rects[j].width - freeRectangles[i].size.width)<1e-6f &&
                        abs(rects[j].height - freeRectangles[i].size.height)<1e-6f)
				{
					bestFreeRect = i;
					bestRect = j;
					bestFlipped = false;
                    bestScore = std::numeric_limits<value_type>::min();
					i = freeRectangles.size(); // Force a jump out of the outer loop as well - we got an instant fit.
					break;
				}
				// If flipping this rectangle is a perfect match, pick that then.
                else if (abs(rects[j].height - freeRectangles[i].size.width)<1e-6f &&
                         abs(rects[j].width - freeRectangles[i].size.height)<1e-6f)
				{
					bestFreeRect = i;
					bestRect = j;
					bestFlipped = true;
                    bestScore = std::numeric_limits<value_type>::min();
					i = freeRectangles.size(); // Force a jump out of the outer loop as well - we got an instant fit.
					break;
				}
				// Try if we can fit the rectangle upright.
                else if (rects[j].width <= freeRectangles[i].size.width &&
                         rects[j].height <= freeRectangles[i].size.height)
				{
                    value_type score = ScoreByHeuristic(rects[j].width, rects[j].height, freeRectangles[i], rectChoice);
					if (score < bestScore)
					{
						bestFreeRect = i;
						bestRect = j;
						bestFlipped = false;
						bestScore = score;
					}
				}
				// If not, then perhaps flipping sideways will make it fit?
                else if (rects[j].height <= freeRectangles[i].size.width &&
                         rects[j].width <= freeRectangles[i].size.height)
				{
                    value_type score = ScoreByHeuristic(rects[j].height, rects[j].width, freeRectangles[i], rectChoice);
					if (score < bestScore)
					{
						bestFreeRect = i;
						bestRect = j;
						bestFlipped = true;
						bestScore = score;
					}
				}
			}
		}

		// If we didn't manage to find any rectangle to pack, abort.
        if (bestScore == std::numeric_limits<value_type>::max())
			return;

		// Otherwise, we're good to go and do the actual packing.
		Rect newNode;
		newNode.x = freeRectangles[bestFreeRect].x;
		newNode.y = freeRectangles[bestFreeRect].y;
        newNode.size.width = rects[bestRect].width;
        newNode.size.height = rects[bestRect].height;

		if (bestFlipped)
            std::swap(newNode.size.width, newNode.size.height);

		// Remove the free space we lost in the bin.
		SplitFreeRectByHeuristic(freeRectangles[bestFreeRect], newNode, splitMethod);
        freeRectangles.erase(freeRectangles.begin() + (int)bestFreeRect);

		// Remove the rectangle we just packed from the input list.
        rects.erase(rects.begin() + (int)bestRect);

		// Perform a Rectangle Merge step if desired.
		if (merge)
			MergeFreeList();

		// Remember the new used rectangle.
		usedRectangles.push_back(newNode);

		// Check that we're really producing correct packings here.
		debug_assert(disjointRects.Add(newNode) == true);
	}
}

/// @return True if r fits inside freeRect (possibly rotated).
bool Fits(const RectSize &r, const Rect &freeRect)
{
    return (r.width <= freeRect.size.width && r.height <= freeRect.size.height) ||
        (r.height <= freeRect.size.width && r.width <= freeRect.size.height);
}

/// @return True if r fits perfectly inside freeRect, i.e. the leftover area is 0.
bool FitsPerfectly(const RectSize &r, const Rect &freeRect)
{
    return (abs(r.width - freeRect.size.width)<1e-6f && abs(r.height - freeRect.size.height)<1e-6f) ||
        (abs(r.height - freeRect.size.width)<1e-6f && abs(r.width - freeRect.size.height)<1e-6f);
}


Rect GuillotineBinPack::Insert(value_type width, value_type height, bool merge, FreeRectChoiceHeuristic rectChoice,
	GuillotineSplitHeuristic splitMethod)
{
	// Find where to put the new rectangle.
	int freeNodeIndex = 0;
	Rect newRect = FindPositionForNewNode(width, height, rectChoice, &freeNodeIndex);

	// Abort if we didn't have enough space in the bin.
    if (newRect.size.height == 0)
		return newRect;

	// Remove the space that was just consumed by the new rectangle.
	SplitFreeRectByHeuristic(freeRectangles[freeNodeIndex], newRect, splitMethod);
	freeRectangles.erase(freeRectangles.begin() + freeNodeIndex);

	// Perform a Rectangle Merge step if desired.
	if (merge)
		MergeFreeList();

	// Remember the new used rectangle.
	usedRectangles.push_back(newRect);

	// Check that we're really producing correct packings here.
	debug_assert(disjointRects.Add(newRect) == true);

	return newRect;
}

/// Computes the ratio of used surface area to the total bin area.
value_type GuillotineBinPack::Occupancy() const
{
	///\todo The occupancy rate could be cached/tracked incrementally instead
	///      of looping through the list of packed rectangles here.
    value_type usedSurfaceArea = 0;
	for(size_t i = 0; i < usedRectangles.size(); ++i)
        usedSurfaceArea += usedRectangles[i].size.width * usedRectangles[i].size.height;

    return (value_type)usedSurfaceArea / (binWidth * binHeight);
}

/// Returns the heuristic score value for placing a rectangle of size width*height into freeRect. Does not try to rotate.
value_type GuillotineBinPack::ScoreByHeuristic(value_type width, value_type height, const Rect &freeRect, FreeRectChoiceHeuristic rectChoice)
{
	switch(rectChoice)
	{
	case RectBestAreaFit: return ScoreBestAreaFit(width, height, freeRect);
	case RectBestShortSideFit: return ScoreBestShortSideFit(width, height, freeRect);
	case RectBestLongSideFit: return ScoreBestLongSideFit(width, height, freeRect);
	case RectWorstAreaFit: return ScoreWorstAreaFit(width, height, freeRect);
	case RectWorstShortSideFit: return ScoreWorstShortSideFit(width, height, freeRect);
	case RectWorstLongSideFit: return ScoreWorstLongSideFit(width, height, freeRect);
	default: assert(false); return std::numeric_limits<int>::max();
	}
}

value_type GuillotineBinPack::ScoreBestAreaFit(value_type width, value_type height, const Rect &freeRect)
{
    return freeRect.size.width * freeRect.size.height - width * height;
}

value_type GuillotineBinPack::ScoreBestShortSideFit(value_type width, value_type height, const Rect &freeRect)
{
    value_type leftoverHoriz = abs(freeRect.size.width - width);
    value_type leftoverVert = abs(freeRect.size.height - height);
    value_type leftover = min(leftoverHoriz, leftoverVert);
	return leftover;
}

value_type GuillotineBinPack::ScoreBestLongSideFit(value_type width, value_type height, const Rect &freeRect)
{
    int leftoverHoriz = abs(freeRect.size.width - width);
    int leftoverVert = abs(freeRect.size.height - height);
	int leftover = max(leftoverHoriz, leftoverVert);
	return leftover;
}

value_type GuillotineBinPack::ScoreWorstAreaFit(value_type width, value_type height, const Rect &freeRect)
{
	return -ScoreBestAreaFit(width, height, freeRect);
}

value_type GuillotineBinPack::ScoreWorstShortSideFit(value_type width, value_type height, const Rect &freeRect)
{
	return -ScoreBestShortSideFit(width, height, freeRect);
}

value_type GuillotineBinPack::ScoreWorstLongSideFit(value_type width, value_type height, const Rect &freeRect)
{
	return -ScoreBestLongSideFit(width, height, freeRect);
}

Rect GuillotineBinPack::FindPositionForNewNode(value_type width, value_type height, FreeRectChoiceHeuristic rectChoice, int *nodeIndex)
{
	Rect bestNode;
	memset(&bestNode, 0, sizeof(Rect));

    value_type bestScore = std::numeric_limits<value_type>::max();

	/// Try each free rectangle to find the best one for placement.
	for(size_t i = 0; i < freeRectangles.size(); ++i)
	{
		// If this is a perfect fit upright, choose it immediately.
        if (width == freeRectangles[i].size.width && height == freeRectangles[i].size.height)
		{
			bestNode.x = freeRectangles[i].x;
			bestNode.y = freeRectangles[i].y;
            bestNode.size.width = width;
            bestNode.size.height = height;
			bestScore = std::numeric_limits<int>::min();
			*nodeIndex = i;
			debug_assert(disjointRects.Disjoint(bestNode));
			break;
		}
		// If this is a perfect fit sideways, choose it.
        else if (height == freeRectangles[i].size.width && width == freeRectangles[i].size.height)
		{
			bestNode.x = freeRectangles[i].x;
			bestNode.y = freeRectangles[i].y;
            bestNode.size.width = height;
            bestNode.size.height = width;
			bestScore = std::numeric_limits<int>::min();
			*nodeIndex = i;
			debug_assert(disjointRects.Disjoint(bestNode));
			break;
		}
		// Does the rectangle fit upright?
        else if (width <= freeRectangles[i].size.width && height <= freeRectangles[i].size.height)
		{
            value_type score = ScoreByHeuristic(width, height, freeRectangles[i], rectChoice);

			if (score < bestScore)
			{
				bestNode.x = freeRectangles[i].x;
				bestNode.y = freeRectangles[i].y;
                bestNode.size.width = width;
                bestNode.size.height = height;
				bestScore = score;
				*nodeIndex = i;
				debug_assert(disjointRects.Disjoint(bestNode));
			}
		}
		// Does the rectangle fit sideways?
        else if (height <= freeRectangles[i].size.width && width <= freeRectangles[i].size.height)
		{
            value_type score = ScoreByHeuristic(height, width, freeRectangles[i], rectChoice);

			if (score < bestScore)
			{
				bestNode.x = freeRectangles[i].x;
				bestNode.y = freeRectangles[i].y;
                bestNode.size.width = height;
                bestNode.size.height = width;
				bestScore = score;
				*nodeIndex = i;
				debug_assert(disjointRects.Disjoint(bestNode));
			}
		}
	}
	return bestNode;
}

void GuillotineBinPack::SplitFreeRectByHeuristic(const Rect &freeRect, const Rect &placedRect, GuillotineSplitHeuristic method)
{
	// Compute the lengths of the leftover area.
    const value_type w = freeRect.size.width - placedRect.size.width;
    const value_type h = freeRect.size.height - placedRect.size.height;

	// Placing placedRect into freeRect results in an L-shaped free area, which must be split into
	// two disjoint rectangles. This can be achieved with by splitting the L-shape using a single line.
	// We have two choices: horizontal or vertical.	

	// Use the given heuristic to decide which choice to make.

	bool splitHorizontal;
	switch(method)
	{
	case SplitShorterLeftoverAxis:
		// Split along the shorter leftover axis.
		splitHorizontal = (w <= h);
		break;
	case SplitLongerLeftoverAxis:
		// Split along the longer leftover axis.
		splitHorizontal = (w > h);
		break;
	case SplitMinimizeArea:
		// Maximize the larger area == minimize the smaller area.
		// Tries to make the single bigger rectangle.
        splitHorizontal = (placedRect.size.width * h > w * placedRect.size.height);
		break;
	case SplitMaximizeArea:
		// Maximize the smaller area == minimize the larger area.
		// Tries to make the rectangles more even-sized.
        splitHorizontal = (placedRect.size.width * h <= w * placedRect.size.height);
		break;
	case SplitShorterAxis:
		// Split along the shorter total axis.
        splitHorizontal = (freeRect.size.width <= freeRect.size.height);
		break;
	case SplitLongerAxis:
		// Split along the longer total axis.
        splitHorizontal = (freeRect.size.width > freeRect.size.height);
		break;
	default:
		splitHorizontal = true;
		assert(false);
	}

	// Perform the actual split.
	SplitFreeRectAlongAxis(freeRect, placedRect, splitHorizontal);
}

/// This function will add the two generated rectangles into the freeRectangles array. The caller is expected to
/// remove the original rectangle from the freeRectangles array after that.
void GuillotineBinPack::SplitFreeRectAlongAxis(const Rect &freeRect, const Rect &placedRect, bool splitHorizontal)
{
	// Form the two new rectangles.
	Rect bottom;
	bottom.x = freeRect.x;
    bottom.y = freeRect.y + placedRect.size.height;
    bottom.size.height = freeRect.size.height - placedRect.size.height;

	Rect right;
    right.x = freeRect.x + placedRect.size.width;
	right.y = freeRect.y;
    right.size.width = freeRect.size.width - placedRect.size.width;

	if (splitHorizontal)
	{
        bottom.size.width = freeRect.size.width;
        right.size.height = placedRect.size.height;
	}
	else // Split vertically
	{
        bottom.size.width = placedRect.size.width;
        right.size.height = freeRect.size.height;
	}

	// Add the new rectangles into the free rectangle pool if they weren't degenerate.
    if (bottom.size.width > 0 && bottom.size.height > 0)
		freeRectangles.push_back(bottom);
    if (right.size.width > 0 && right.size.height > 0)
		freeRectangles.push_back(right);

	debug_assert(disjointRects.Disjoint(bottom));
	debug_assert(disjointRects.Disjoint(right));
}

void GuillotineBinPack::MergeFreeList()
{
#ifdef _DEBUG
	DisjointRectCollection test;
	for(size_t i = 0; i < freeRectangles.size(); ++i)
		assert(test.Add(freeRectangles[i]) == true);
#endif

	// Do a Theta(n^2) loop to see if any pair of free rectangles could me merged into one.
	// Note that we miss any opportunities to merge three rectangles into one. (should call this function again to detect that)
	for(size_t i = 0; i < freeRectangles.size(); ++i)
		for(size_t j = i+1; j < freeRectangles.size(); ++j)
		{
            if (freeRectangles[i].size.width == freeRectangles[j].size.width && freeRectangles[i].x == freeRectangles[j].x)
			{
                if (freeRectangles[i].y == freeRectangles[j].y + freeRectangles[j].size.height)
				{
                    freeRectangles[i].y -= freeRectangles[j].size.height;
                    freeRectangles[i].size.height += freeRectangles[j].size.height;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
                else if (freeRectangles[i].y + freeRectangles[i].size.height == freeRectangles[j].y)
				{
                    freeRectangles[i].size.height += freeRectangles[j].size.height;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
			}
            else if (freeRectangles[i].size.height == freeRectangles[j].size.height && freeRectangles[i].y == freeRectangles[j].y)
			{
                if (freeRectangles[i].x == freeRectangles[j].x + freeRectangles[j].size.width)
				{
                    freeRectangles[i].x -= freeRectangles[j].size.width;
                    freeRectangles[i].size.width += freeRectangles[j].size.width;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
                else if (freeRectangles[i].x + freeRectangles[i].size.width == freeRectangles[j].x)
				{
                    freeRectangles[i].size.width += freeRectangles[j].size.width;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
			}
		}

#ifdef _DEBUG
	test.Clear();
	for(size_t i = 0; i < freeRectangles.size(); ++i)
		assert(test.Add(freeRectangles[i]) == true);
#endif
}

}
