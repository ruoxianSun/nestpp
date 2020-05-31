/** @file SkylineBinPack.h
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the SKYLINE data structure.

	This work is released to Public Domain, do whatever you want with it.
*/
#pragma once

#include <vector>

#include "Rect.h"
#include "GuillotineBinPack.h"

namespace rbp {

/** Implements bin packing algorithms that use the SKYLINE data structure to store the bin contents. Uses
	GuillotineBinPack as the waste map. */
class SkylineBinPack
{
public:
	/// Instantiates a bin of size (0,0). Call Init to create a new bin.
	SkylineBinPack();

	/// Instantiates a bin of the given size.
    SkylineBinPack(value_type binWidth, value_type binHeight, bool useWasteMap);

	/// (Re)initializes the packer to an empty bin of width x height units. Call whenever
	/// you need to restart with a new bin.
    void Init(value_type binWidth, value_type binHeight, bool useWasteMap);

	/// Defines the different heuristic rules that can be used to decide how to make the rectangle placements.
	enum LevelChoiceHeuristic
	{
		LevelBottomLeft,
		LevelMinWasteFit
	};

	/// Inserts the given list of rectangles in an offline/batch mode, possibly rotated.
	/// @param rects The list of rectangles to insert. This vector will be destroyed in the process.
	/// @param dst [out] This list will contain the packed rectangles. The indices will not correspond to that of rects.
	/// @param method The rectangle placement rule to use when packing.
	void Insert(std::vector<RectSize> &rects, std::vector<Rect> &dst, LevelChoiceHeuristic method);

	/// Inserts a single rectangle into the bin, possibly rotated.
    Rect Insert(value_type width, value_type height, LevelChoiceHeuristic method);

	/// Computes the ratio of used surface area to the total bin area.
	float Occupancy() const;

private:
    value_type binWidth;
    value_type binHeight;

#ifdef _DEBUG
	DisjointRectCollection disjointRects;
#endif

	/// Represents a single level (a horizontal line) of the skyline/horizon/envelope.
	struct SkylineNode
	{
		/// The starting x-coordinate (leftmost).
        value_type x;

		/// The y-coordinate of the skyline level line.
        value_type y;

		/// The line width. The ending coordinate (inclusive) will be x+width-1.
        value_type width;
	};

	std::vector<SkylineNode> skyLine;

    value_type usedSurfaceArea;

	/// If true, we use the GuillotineBinPack structure to recover wasted areas into a waste map.
	bool useWasteMap;
	GuillotineBinPack wasteMap;

    Rect InsertBottomLeft(value_type width, value_type height);
    Rect InsertMinWaste(value_type width, value_type height);

    Rect FindPositionForNewNodeMinWaste(value_type width, value_type height, value_type &bestHeight, value_type &bestWastedArea, int &bestIndex) const;
    Rect FindPositionForNewNodeBottomLeft(value_type width, value_type height, value_type &bestHeight, value_type &bestWidth, int &bestIndex) const;

    bool RectangleFits(int skylineNodeIndex, value_type width, value_type height, value_type &y) const;
    bool RectangleFits(int skylineNodeIndex, value_type width, value_type height, value_type &y, value_type &wastedArea) const;
    value_type ComputeWastedArea(int skylineNodeIndex, value_type width, value_type height, value_type y) const;

    void AddWasteMapArea(int skylineNodeIndex, value_type width, value_type height, value_type y);

	void AddSkylineLevel(int skylineNodeIndex, const Rect &rect);

	/// Merges all skyline nodes that are at the same level.
	void MergeSkylines();
};

}
