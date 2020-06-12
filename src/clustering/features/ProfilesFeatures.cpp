/*
 * ProfilesFeatures.cpp
 *
 *  Created on: 12.02.2010
 *      
 */

#include "ProfilesFeatures.hpp"

namespace impact_tr4
{

ProfilesFeatures::ProfilesFeatures(const int nHorizontalBlocks, const int nVerticalBlocks, bool useHorizontalOuter/*=true*/, bool useVerticalOuter/*=true*/, bool useHorizontalInner/*=true*/, bool useVerticalInner/*=true*/)
: mNHorizontalBlocks(nHorizontalBlocks), mNVerticalBlocks(nVerticalBlocks), mUseHorizontalOuter(useHorizontalOuter), mUseVerticalOuter(useVerticalOuter), mUseHorizontalInner(useHorizontalInner), mUseVerticalInner(useVerticalInner)
{

}

ProfilesFeatures::~ProfilesFeatures()
{

}

const int
ProfilesFeatures::vectorSize() const
{
	int size = 0;
	if (mUseHorizontalOuter) size += 2*mNHorizontalBlocks;
	if (mUseHorizontalInner) size += 2*mNHorizontalBlocks;
	if (mUseVerticalOuter) size += 2*mNVerticalBlocks;
	if (mUseVerticalInner) size += 2*mNVerticalBlocks;

	return size;
}

void ProfilesFeatures::calculateOutputVector(const GrayImage<> &image, const int rowIndex, const int colIndex, ublas::matrix<float>& outMatrix)
{
	// compute center of mass:
	Index ctrOfMassIndex;
	image.computeCenterOfMassIndex(ctrOfMassIndex);
	bool isValidIndex=true;
	if (ctrOfMassIndex(0)<0 || ctrOfMassIndex(1)<0) {
//		std::cout << "invalid index!" << std::endl;
		isValidIndex=false;
	}

	// compute horizontal profiles (if necessary):
	std::vector<float> profileUpOuterB(mNHorizontalBlocks, 0);
	std::vector<float> profileUpInnerB(mNHorizontalBlocks, 0);
	std::vector<float> profileDownOuterB(mNHorizontalBlocks, 0);
	std::vector<float> profileDownInnerB(mNHorizontalBlocks, 0);
	if ( isValidIndex && (mUseHorizontalOuter || mUseHorizontalInner) && (mNHorizontalBlocks > 0) ) {
		image.computeHorizontalProfileBlocks(mNHorizontalBlocks, ctrOfMassIndex, profileUpOuterB, profileUpInnerB, profileDownOuterB, profileDownInnerB);
	}

	// compute vertical profiles (if necessary):
	std::vector<float> profileLeftOuterB(mNVerticalBlocks, 0);
	std::vector<float> profileLeftInnerB(mNVerticalBlocks, 0);
	std::vector<float> profileRightOuterB(mNVerticalBlocks, 0);
	std::vector<float> profileRightInnerB(mNVerticalBlocks, 0);
	if ( isValidIndex && (mUseVerticalOuter || mUseVerticalInner) && (mNVerticalBlocks > 0) ) {
		image.computeVerticalProfileBlocks(mNVerticalBlocks, ctrOfMassIndex, profileLeftOuterB, profileLeftInnerB, profileRightOuterB, profileRightInnerB);
	}

	// store data in matrix row vector:
	int actIndex = colIndex;
	// store horizontal profiles:
	if (mUseHorizontalOuter) {
		for (int i=0; i<profileUpOuterB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileUpOuterB[i];
		for (int i=0; i<profileDownOuterB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileDownOuterB[i];
	}
	if (mUseHorizontalInner) {
		for (int i=0; i<profileUpInnerB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileUpInnerB[i];
		for (int i=0; i<profileDownInnerB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileDownInnerB[i];
	}
	// store vertical profiles:
	if (mUseVerticalOuter) {
		for (int i=0; i<profileLeftOuterB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileLeftOuterB[i];
		for (int i=0; i<profileRightOuterB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileRightOuterB[i];
	}
	if (mUseVerticalInner) {
		for (int i=0; i<profileLeftInnerB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileLeftInnerB[i];
		for (int i=0; i<profileRightInnerB.size(); ++i)
			outMatrix(rowIndex, actIndex++) = profileRightInnerB[i];
	}

//	std::cout << "actIndex = " << actIndex << " size = " << vectorSize() << " (should be the same!" << std::endl;

	return;
} // end calculateOutputVector

} // end namespace impact_tr4

