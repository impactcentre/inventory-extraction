/*******************************************************************************
 * 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef CLUSTER_HPP_
#define CLUSTER_HPP_

// global includes:
#include <iostream>
// boost includes:
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
// project includes:
#include "core_headers.h"
#include "image_headers.h"
// local includes:
#include "DataStructs.hpp"

//static float (*sfpImageCharDistance)(const ImageChar& imc1, const ImageChar& imc2);

namespace impact_tr4 {

class Document;

typedef std::list<ImageChar*>::iterator ImageCharIt;
typedef std::list<ImageChar*>::const_iterator ImageCharConstIt;

/// Compares two ImageCharIts 's by returning distToClusterCenter1 < distToClusterCenter2
inline bool compareImageCharByDistToClustCenter(ImageChar *im1, ImageChar *im2)
{
	return im1->distToClusterCenter < im2->distToClusterCenter;
}

class CharCluster
{
public:
	/// Default Constructor
	CharCluster()
	{
		mUnicodeLabel = 0;
		mInfoText = "";
		mPrototypeImage.init(defaultPrototypeWidth, defaultPrototypeHeight); mPrototypeImage.fill(255);
		mPrototypeImageThresh.init(defaultPrototypeWidth, defaultPrototypeHeight); mPrototypeImage.fill(0);
		mIsPrototypeUpToDate = true;
	}

	/// Returns a const reference to the glyph list
//	const std::list<ImageChar*>& charList() const { return mCharList; }
	/// Returns a reference to the glyph list
//	std::list<ImageChar*>& charList() { return mCharList; }

	ImageCharConstIt begin() const { return mCharList.begin(); }
	ImageCharConstIt end() const { return mCharList.end(); }
	ImageCharIt begin() { return mCharList.begin(); }
	ImageCharIt end() { return mCharList.end(); }

	// EDITING OPERATIONS:
	/// Adds the specified pointer to an ImageChar to this cluster
	void addChar(ImageChar* imChar);
	/// Removes the character with the specified iterator it from this cluster
	void removeChar(ImageCharIt& it);
	/// Removes the characters with the specified iterators it from this cluster
	void removeChars(std::vector<ImageCharIt>& iterators);
	void clear();

	/** Appends the cluster otherCluster to this one, i.e. appends its elements to the list.
	 * Note, that the elements of the list otherCluster are erased within this operation!
	 *  */
	void appendCluster( CharCluster& otherCluster );

	// METHODS:
	/// True, if this cluster is empty
	const bool empty() const { return mCharList.empty(); }
	/// Returns number of elements in this cluster
	const int size() const { return mCharList.size(); }

	/// Computes min/max width/height of all glyphs in this cluster
	void minMaxDims(int *minWidth, int *maxWidth, int *minHeight, int *maxHeight) const;
	/// Determines the width of all glyphs in this cluster
	int determineImageWidth() const;
//	void computeMean(ublas::vector<float> &mean) const;

	/// Does this cluster has a prototype image computed, i.e. is it non-empty?
//	bool hasPrototypeImage() const { return !mPrototypeImage.empty(); }
	bool isPrototypeUpToDate() const { return mIsPrototypeUpToDate; }

//	PreprocessingResults* getPreprocessingPointer() { return &mPrototypePreprocessing; }
//	void preprocessPrototype();

	/// Recompute the prototype image for this cluster
	void recomputePrototypeImage(const int width=defaultPrototypeWidth, const int height=defaultPrototypeHeight);
	GrayImage<>* getPrototypeImagePointer() { return &mPrototypeImage; }
	GrayImage<>* getPrototypeImageThreshPointer() { return &mPrototypeImageThresh; }

	void setUnicodeLabel(const unsigned int unicodeLabel) { mUnicodeLabel = unicodeLabel; }
	void setUnicodeLabel(const std::string &hexUnicodeStr);

	unsigned int unicodeLabel() const { return mUnicodeLabel; }
	std::string unicodeLabelString() const;

	void setInfoText(const std::string &infoText) { mInfoText = infoText; }
	std::string infoText() { return mInfoText; }

	void sortClusterByDistanceToClusterCenter();

	// FRIENDS:
#if 0
	friend float clusterDistMean(const CharCluster& c1, const CharCluster& c2);
	friend float clusterDistWeightedMean(const CharCluster& c1, const CharCluster& c2);
#endif
	friend float clusterDistAvg(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat);
#if 0
	friend float clusterDistAvgDirect(const CharCluster& c1, const CharCluster& c2);
#endif
	friend float clusterDistMax(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat);
	friend float clusterDistMin(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat);
	// PUBLIC STATIC MEMBERS:
#if 0
	typedef float (*FEATURE_DIST_FUNCTION_TYPE)(const ImageChar& imc1, const ImageChar& imc2);
	static FEATURE_DIST_FUNCTION_TYPE sfpImageCharDistance;
#endif

//	bool operator<(const CharCluster& other) { return this->size() < other.size(); }

//	static float (*sfpImageCharDistance)(const ImageChar& imc1, const ImageChar& imc2);
private:
	// MEMBERS:
	std::list<ImageChar*> mCharList;

	GrayImage<> mPrototypeImage;
	GrayImage<> mPrototypeImageThresh;
	bool mIsPrototypeUpToDate;

	PreprocessingResults mPrototypePreprocessing;

	unsigned int  mUnicodeLabel;
	std::string mInfoText;

	static const int defaultPrototypeWidth;
	static const int defaultPrototypeHeight;
};


// CLUSTER DIFFERENCE FUNCTIONS:

/** Compute the distance between two clusters, with respect to their mean values. */
//float clusterDistMean(const CharCluster& c1, const CharCluster& c2);
/** Compute the distance between by averaging over all inter-cluster differences. */
float clusterDistAvg(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat);
/** Compute the distance between by averaging over all inter-cluster differences
 * using direct distance measure between images of cluster using OpenCV's cvMatchShapes function. */
//float clusterDistAvgDirect(const CharCluster& c1, const CharCluster& c2);
/** Compute the distance between by minimizing over all inter-cluster differences. */
float clusterDistMin(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat);
/** Compute the distance between by maximizing over all inter-cluster differences. */
float clusterDistMax(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat);

/** General template based cluster distance function.
 * The template parameter distType specifies the specific distance function
 * that is used to measure the distance between c1 and c2.
 * */
//template <ClusterDistanceType distType> inline
//float clusterDist(const CharCluster& c1, const CharCluster& c2);
//// Template specializations for clusterDist:
//template <> inline
//float clusterDist<MEAN_DIST>(const CharCluster& c1, const CharCluster& c2)
//	{ return clusterDistMean(c1,c2); }
//template <> inline
//float clusterDist<AVG_DIST>(const CharCluster& c1, const CharCluster& c2)
//	{ return clusterDistAvg(c1,c2); }
//template <> inline
//float clusterDist<AVG_DIRECT_DIST>(const CharCluster& c1, const CharCluster& c2)
//	{ return clusterDistAvgDirect(c1,c2); }
//template <> inline
//float clusterDist<MIN_DIST>(const CharCluster& c1, const CharCluster& c2)
//	{ return clusterDistMin(c1,c2); }
//template <> inline
//float clusterDist<MAX_DIST>(const CharCluster& c1, const CharCluster& c2)
//	{ return clusterDistMax(c1,c2); }
////////////////////////////////////////////

} // end of namespace impact_tr4

#endif /*CLUSTER_HPP_*/
