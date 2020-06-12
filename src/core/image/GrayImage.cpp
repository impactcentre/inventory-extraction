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
#include "GrayImage.hpp"
#include "ImageIterator.hpp"

#include "clustering/DataStructs.hpp"
#include "clustering/CharCluster.hpp"
#include "clustering/ClusteringResult.hpp"

#if 1
	#include <QImage>
#endif

namespace impact_tr4 {

// IMPLEMENTATIONS

// helper functions:

void writeOneClusterImage( const CharCluster &cluster, GrayImage<> &resultImage, const int maxWidth /* = 1500*/)
{
	// determine dimensions:
	const int spaceX = 15, spaceY = 15;
	const int xOffLeft = 25, xOffRight = 25;
	const int yOffTop = 25, yOffBottom = 25;

	const int nChars = cluster.size();
//	std::cout << "nr of chars: " << nChars << std::endl;
	std::vector<int> rowIndices(nChars);
//	int actWidth = 0;
//	const std::list<ImageChar*>& charList = cluster.charList();

	ImageCharConstIt it = cluster.begin();
	ImageCharConstIt itTemp = cluster.begin();
	int actRow = 0, actW = 0;
	int c = 0;
	int totalHeight = 0;
	int maxHeight = -1;
	while ( it!=cluster.end() ) {
		BoundingBox bbox = (*it)->bBox;
		if (bbox.height() > maxHeight) maxHeight = bbox.height();
		if ( (actW + bbox.width() + spaceX) > maxWidth ) {
			++actRow;
			actW = bbox.width() + spaceX;
			totalHeight += maxHeight + spaceY;
			maxHeight = bbox.height();
		}
		else {
			actW += bbox.width() + spaceX;
		}
		rowIndices[c] = actRow;
		++c;
		++it;
	}
	totalHeight += maxHeight;
//	std::cout << "determined image size" << std::endl;
	///////////////////////////////////////////
	const int imWidth = maxWidth + xOffLeft + xOffRight;
	const int imHeight = totalHeight + yOffTop + yOffBottom;
	resultImage.init(imWidth, imHeight);
	resultImage.fill(255);

	actRow = 0; actW = 0; int actH = 0;
	c = 0;
	totalHeight = 0;
	maxHeight = -1;
	int x = 0, y = 0;

	it = cluster.begin();
	itTemp = cluster.begin();
	while ( it!=cluster.end() ) {
		GrayImage<> *pImage = (*it)->pImage;
		BoundingBox bbox = (*it)->bBox;
		pImage->setRoi( bbox );
	//			std::cout << bbox << std::endl;

		x = xOffRight + actW;
		y = yOffTop + actH;

//		std::cout << "x = " << x << ", y = " << y << std::endl;

		if (bbox.height() > maxHeight) maxHeight = bbox.height();
		if ( (actW + bbox.width() + spaceX) > maxWidth ) {
			++actRow;
			x = xOffRight;
			actW = bbox.width() + spaceX;
			actH += maxHeight + spaceY;
			y = yOffTop + actH;

			totalHeight += maxHeight + spaceY;
			maxHeight = bbox.height();
		}
		else {
			x = xOffRight + actW;
			actW += bbox.width() + spaceX;
			y = yOffTop + actH;
		}
		rowIndices[c] = actRow;

		resultImage.set( y, x, *pImage );
		pImage->releaseRoi();
		++c;
		++it;
	}
	totalHeight += maxHeight;

	return;
}

void writeClusterImageBlocked( const ClusteringResult &clusteringResult, GrayImage<uint8> &resultImage, const int maxClustW /*= 1500*/ )
{
	// determine cluster image for all clusters:
	const int nClusts = clusteringResult.nClusters();
	std::vector<GrayImage<uint8> > clusterImages( nClusts );

	int maxW = 0, height = 0;

	int i=0;
	for (CharClustConstIt clust_it = clusteringResult.begin(); clust_it!= clusteringResult.end(); ++clust_it) {
		writeOneClusterImage( *(*clust_it), clusterImages[i], maxClustW);

		if (clusterImages[i].width() > maxW) maxW = clusterImages[i].width();
		height += clusterImages[i].height();
		++i;
	}
	const int xOffLeft = 25, xOffRight = 25;
	const int yOffTop = 25, yOffBottom = 25;

	int imWidth = xOffLeft + xOffRight + maxW;
	int imHeight = yOffTop + yOffBottom + height;

	resultImage.init(imWidth, imHeight);
	resultImage.fill(255);

	int y = yOffTop;
	for (int i=0; i<nClusts; ++i) {
		resultImage.set(y, xOffLeft, clusterImages[i]);
		y += clusterImages[i].height();

		resultImage.drawLine( Index(y-1, xOffLeft), Index(y-1, xOffLeft+maxW), 1 );
		// write horizontal line:s
//		for ( int j=xOffLeft; j <= xOffLeft+maxW ; ++j) {
//			resultImage(y-1, j) = 0;
//			}

	}

	return;
} // end writeClusterImageBlocked

void writeClusterImage( const std::vector<CharCluster> &clusters, GrayImage<> &resultImage )
{
	const int nClusters = clusters.size();
	int minWidth=0, maxWidth=0, minHeight=0, maxHeight=0;
	int glMinWidth=1e6;
	int glMaxWidth=-1;
	int glMinHeight=1e6;
	int glMaxHeight=-1;
	int maxClustSize = -1;
	std::vector<int> clusterWidths(nClusters);
	for (int i=0; i<clusterWidths.size(); ++i) clusterWidths[i] = 0;

	int imageHeight=0;
	for (int i=0; i<nClusters; ++i) {
		// determine width of cluster:
		clusterWidths[i] = clusters[i].determineImageWidth();
		clusters[i].minMaxDims(&minWidth, &maxWidth, &minHeight, &maxHeight);
//		int clustHeight =


		imageHeight += maxHeight;

		if (minWidth < glMinWidth) glMinWidth = minWidth;
		if (maxWidth > glMaxWidth) glMaxWidth = maxWidth;
		if (minHeight < glMinHeight) glMinHeight = minHeight;
		if (maxHeight > glMaxHeight) glMaxHeight = maxHeight;
		if (clusters[i].size() > maxClustSize) maxClustSize = clusters[i].size();
	} // end for i
	int maxClusterWidth = *std::max_element(clusterWidths.begin(), clusterWidths.end());

	std::cout << "global min/max vals are:" << std::endl;
	std::cout << glMinWidth << ", " << glMaxWidth << "; " << glMinHeight << ", " << glMaxHeight << std::endl;
	std::cout << "maxclustsize = " << maxClustSize << std::endl;

	const int xOffLeft = 25, xOffRight = 25;
	const int yOffTop = 25, yOffBottom = 25;
	const int spaceX = 15, spaceY = 15; // should both be > 4

//	const int imWidth = maxClustSize*glMaxWidth +
	const int imWidth = maxClusterWidth +
						(maxClustSize-1)*spaceX +
						xOffLeft + xOffRight + 100;
	const int imHeight = nClusters*glMaxHeight +
//	const int imHeight = imageHeight +
						 (nClusters-1)*spaceY +
						 yOffTop + yOffBottom;
	std::cout << imWidth << " x " << imHeight << std::endl;

	GrayImage<> tempImage(imWidth, imHeight);
	tempImage.fill(255);

	int x = spaceX, y = spaceY;
	int actMaxWidth = -1;
	int actMaxHeight = -1;
#if 1
	for ( int i=0; i<nClusters; ++i ) { // for all clusters
//		const std::list<ImageChar*>& charList = clusters[i].charList();
		ImageCharConstIt it = clusters[i].begin();
		ImageCharConstIt itTemp = clusters[i].begin();
		int maxY = -1;
		while ( it!=clusters[i].end() ) {
			GrayImage<> *pImage = (*it)->pImage;
			BoundingBox bbox = (*it)->bBox;
			pImage->setRoi( bbox );
//			std::cout << bbox << std::endl;
			tempImage.set( y, x, *pImage );
			pImage->releaseRoi();
			x += (bbox.width()+spaceX);

			// draw vertical line between characters: TODO??
//			int lineX = x - spaceX + 2;
//			for (int vertLine=y; y<)

			if (bbox.height() > maxY) maxY = bbox.height();
			itTemp = it;
			++it;
		} // end  while
		// draw horizontal line between clusters:
		int lineY = y+maxY+2;
		for (int horLine=0; horLine<x-spaceX; ++horLine) {
			tempImage(lineY, horLine) = 0;
		}

		actMaxHeight = y + (*itTemp)->bBox.height();
		y += (maxY + spaceY);
		if ( (x+(*itTemp)->bBox.width()) > actMaxWidth ) actMaxWidth = (x+(*itTemp)->bBox.width());
		x = spaceX;
	} // end for i

	std::cout << actMaxWidth << " x " << actMaxHeight << std::endl;

	resultImage.init(actMaxWidth+xOffRight, actMaxHeight+yOffBottom);
	resultImage.fill(255);
	BoundingBox bbox(0,0, actMaxWidth+xOffRight-1, actMaxHeight+yOffBottom-1);
	std::cout << bbox << std::endl;
	tempImage.setRoi(bbox);

	resultImage.set(0,0,tempImage);
	tempImage.releaseRoi();

#endif
	return;
} // end writeClusterImage

const std::string createSegmentationResultFileName(const std::string inputFn, const std::string fnType/*="xml"*/, const std::string prefix/*=""*/, const std::string suffix/*=""*/)
{
	std::string baseFnStr = baseFileName( inputFn );
	std::string pathStr = pathName( inputFn );
	std::cout << "base filename = " << baseFnStr << std::endl;
	std::cout << "path filename = " << pathStr << std::endl;

	std::string segResultFn = pathStr + prefix + baseFnStr + suffix + "." + fnType;
	return segResultFn;
}

bool makeWhitespaceImageOne(const GrayImage<uint8> &inputImage, GrayImage<uint8> &resultImage, double thresh /*=0.05*/)
{
//	std::cout << inputImage.size() << std::endl;
	double dens = (double)(inputImage.countNonZero()) / (double)(inputImage.size());
	if (dens < thresh) { // this is a whitespace image
//		std::cout << "this is a whitespace image!" << std::endl;
		resultImage = inputImage;
#if 1
		resultImage.fill((uint8)0);
#else
		resultImage.fill((uint8)(0));
//		resultImage.set(0,0,255);
#endif
		return true;
	}
	else {
		resultImage = inputImage;
		return false;
	}
}

double signatureDistance(const GrayImage<uint8> &im1, const GrayImage<uint8> &im2, const GrayImage<float> &distTransf1, const GrayImage<float> &distTransf2, const GrayImage<float> &intInv1, const GrayImage<float> &intInv2)
{
	GrayImage<float> maxDistTransf;
	GrayImage<float> maxIntInv;
	ImOp::max(distTransf1, distTransf2, maxDistTransf);
	ImOp::max(intInv1, intInv2, maxIntInv);
	GrayImage<> im1Bin = im1;
	GrayImage<> im2Bin = im2;
	im1Bin.binarize(128, 1); im2Bin.binarize(128, 1);
	GrayImage<> absDiff;
	ImOp::absDiff(im1Bin, im2Bin, absDiff);
	GrayImage<float> tmp1, tmp2, absDiffFloat;
	ImOp::convert<uint8, float>(absDiff, absDiffFloat);
//	ImOp::convert<float, float>(maxIntInv, tmp2);
	ImOp::mul(absDiffFloat, maxDistTransf, tmp2);
#if 0 // TEST
	double dist = cvNorm( tmp2.iplImage(), NULL, CV_L2 );
	return dist;
#else // ORIG
	ImOp::mul(tmp2, maxIntInv, tmp1);
	double dist = cvNorm( tmp1.iplImage(), NULL, CV_L2 );
	return dist;
#endif
}

/////////////////////////////////////////////////////////////////

#if 1
template <>
GrayImage<uint8>& GrayImage<uint8>::init(const QImage& qImage)
{
	const int height = qImage.height();
	const int width = qImage.width();

	releaseImage();
	createImage(width, height);

	// resize internal image matrix to size of image:
//	mpImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	// assign (gray-) values to the image matrix
	// NOTE QImage stores images in width/height (column/row) order --> reverse indices!
	for (int i=0; i<height; ++i) {
		for (int j=0; j<width; ++j) {
			this->set(i,j, qGray(qImage.pixel(j,i)));
//			(*this)(i,j) = qGray(qImage.pixel(j,i));
		}
	} // end for all rows i

	return *this;
}
#endif

#if 0
template <class T>
void GrayImage<T>::computeQImage(QImage& qImage) const
{ throw std::bad_typeid("computeQImage(QImage&) operation only supported for uint8 images!"); }
#endif

#if 1
template <>
void GrayImage<uint8>::computeQImage(QImage& qImage) const
{
	// NOTE QImage stores images in width/height order --> reverse indices!
//	std::cout << columns() << " " << rows() << endl;

	qImage = QImage(columnsRoi(), rowsRoi(), QImage::Format_RGB32);

	uchar v;
#if 1
	for (int i=yRoi(); i<yRoi()+rowsRoi(); ++i) {
		for (int j=xRoi(); j<xRoi()+columnsRoi(); ++j) {
			v = get(i,j);
			qImage.setPixel( j-xRoi(),i-yRoi(), qRgb(v,v,v) );
		}
	}
#else
	// TODO: BUG (endless loop!)
	ImageIterator<> it = begin();
	ImageIterator<> endIt = end();
	int actRow = it.row();
	while (it != endIt) {
		v = (*it);
//		std::cout << it.column() << ", " << it.row() << std::endl;
//		std::cout << endIt.column() << ", " << endIt.row() << std::endl;
		qImage.setPixel( it.column(), it.row(), qRgb(v,v,v) );
		++it;
	} // end while
#endif

	return;
} // end of computeQImage
#endif

template <>
void GrayImage<uint8>::computeCenterOfMassCenteredImage(GrayImage<>& centeredImage, const uint8 padVal/*=0*/)
{
	const int nRows = rows();
	const int nColumns = columns();
	Index ctrOfMassIndex;
	// compute center of mass index:
	this->computeCenterOfMassIndex(ctrOfMassIndex, true);
//	std::cout << "center of mass of image is: " << ctrOfMassIndex << std::endl;
	int diff_row_top = ctrOfMassIndex[0];
	int diff_row_bottom = nRows - ctrOfMassIndex[0] - 1;
	int diff_col_left = ctrOfMassIndex[1];
	int diff_col_right = nColumns - ctrOfMassIndex[1] - 1;

	int extTop, extBottom, extLeft, extRight;

	if (diff_row_top >= diff_row_bottom && diff_row_top >= diff_col_left && diff_row_top >= diff_col_right) {
		extTop = 0;
		extBottom = diff_row_top - diff_row_bottom;
		extLeft = diff_row_top - diff_col_left;
		extRight = diff_row_top - diff_col_right;
	}
	if (diff_row_bottom >= diff_row_top && diff_row_bottom >= diff_col_left && diff_row_bottom >= diff_col_right) {
		extTop = diff_row_bottom - diff_row_top;
		extBottom = 0;
		extLeft = diff_row_bottom - diff_col_left;
		extRight = diff_row_bottom - diff_col_right;
	}
	if (diff_col_left >= diff_row_top && diff_col_left >= diff_row_bottom && diff_col_left >= diff_col_right) {
		extTop = diff_col_left - diff_row_top;
		extBottom = diff_col_left - diff_row_bottom;
		extLeft = 0;
		extRight = diff_col_left - diff_col_right;
	}

	if (diff_col_right >= diff_row_top && diff_col_right >= diff_row_bottom && diff_col_right >= diff_col_left) {
		extTop = diff_col_right - diff_row_top;
		extBottom = diff_col_right - diff_row_bottom;
		extLeft = diff_col_right - diff_col_left;
		extRight = 0;
	}
	int nNewRows = nRows + (extTop+extBottom);
	int nNewColumns = nColumns + (extLeft+extRight);
	int rowOffset = extTop;
	int colOffset = extLeft;

//	std::cout << extTop << ", " << extBottom << ", " << extLeft << ", " << extRight << std::endl;
//	std::cout << nColumns << ", " << nRows << std::endl;
//	std::cout << nNewColumns << ", " << nNewRows << std::endl;
//	std::cout << colOffset << ", " << rowOffset << std::endl;

	centeredImage.init(nNewColumns, nNewRows);
	centeredImage.fill(padVal);
	centeredImage.set(rowOffset, colOffset, *this);

	centeredImage.computeCenterOfMassIndex(ctrOfMassIndex, true);
//	std::cout << "new center of mass of image is: " << ctrOfMassIndex << std::endl;

	return;
}

template <>
void GrayImage<uint8>::resizeRelativeToCenterOfMass(const int newWidth, const int newHeight, const uint8 padVal/*=0*/)
{
	GrayImage<uint8> tmpImage;
	this->computeCenterOfMassCenteredImage(tmpImage, padVal);
	tmpImage.resize(newWidth, newHeight, CV_INTER_LINEAR);

	*this = tmpImage;

	return;
}

template <>
void GrayImage<uint8>::computeHorizontalProfiles(const Index &center, std::vector<int> &profileUpOuter, std::vector<int> &profileUpInner, std::vector<int> &profileDownOuter, std::vector<int> &profileDownInner) const
{
	profileUpOuter.resize(this->columns());
	profileUpInner.resize(this->columns());
	profileDownOuter.resize(this->columns());
	profileDownInner.resize(this->columns());

	//	std::vector<int> profile
		// compute horizontal profile:
		for (int j=0; j<columns(); ++j) {
			int indexUpOuter=-1, indexUpInner=-1;
			for (int i=0; i<center(0); ++i) {
				if ((*this)(i,j) > 0) {
					indexUpInner = i;
					if (indexUpOuter == -1) {
						indexUpOuter = i;
					}
				}
			} // end i
			profileUpOuter[j]=(indexUpOuter==-1)? 0 : (center(0)-indexUpOuter);
			profileUpInner[j]=(indexUpInner==-1)? 0 : (center(0)-indexUpInner);

			int indexDownOuter=-1, indexDownInner=-1;
			for (int i=center(0)+1; i<rows(); ++i) {
				if ((*this)(i,j) > 0) {
					indexDownOuter=i;
					if (indexDownInner == -1) {
						indexDownInner = i;
					}
				}
			} // end i
			profileDownOuter[j]=(indexDownOuter==-1)? 0 : (indexDownOuter-center(0));
			profileDownInner[j]=(indexDownInner==-1)? 0 : (indexDownInner-center(0));
		} // end for all columns
		return;
}

template <>
void GrayImage<uint8>::computeVerticalProfiles(const Index &center, std::vector<int> &profileLeftOuter, std::vector<int> &profileLeftInner, std::vector<int> &profileRightOuter, std::vector<int> &profileRightInner) const
{
	profileLeftOuter.resize(this->rows());
	profileLeftInner.resize(this->rows());
	profileRightOuter.resize(this->rows());
	profileRightInner.resize(this->rows());

		// compute vertical profile:
		for (int i=0; i<rows(); ++i) {
			int indexLeftOuter=-1, indexLeftInner=-1;
			for (int j=0; j<center(1); ++j) {
				if ((*this)(i,j) > 0) {
					indexLeftInner = j;
					if (indexLeftOuter == -1) {
						indexLeftOuter = j;
					}
				}
			} // end j
			profileLeftOuter[i]=(indexLeftOuter==-1)? 0 : (center(1)-indexLeftOuter);
			profileLeftInner[i]=(indexLeftInner==-1)? 0 : (center(1)-indexLeftInner);

			int indexRightOuter=-1, indexRightInner=-1;
			for (int j=center(1)+1; j<columns(); ++j) {
				if ((*this)(i,j) > 0) {
					indexRightOuter=j;
					if (indexRightInner == -1) {
						indexRightInner = j;
					}
				}
			} // end j
			profileRightOuter[i]=(indexRightOuter==-1)? 0 : (indexRightOuter-center(1));
			profileRightInner[i]=(indexRightInner==-1)? 0 : (indexRightInner-center(1));
		} // end for all columns
		return;
}

template <>
void GrayImage<uint8>::computeHorizontalProfileBlocks(const int nBlocks, const Index &center, std::vector<float> &profileUpOuterB, std::vector<float> &profileUpInnerB, std::vector<float> &profileDownOuterB, std::vector<float> &profileDownInnerB) const
{
	Index ctrOfMass;
	this->computeCenterOfMassIndex(ctrOfMass);
	std::vector<int> profileUpOuter, profileUpInner, profileDownOuter, profileDownInner;
	computeHorizontalProfiles(ctrOfMass, profileUpOuter, profileUpInner, profileDownOuter, profileDownInner);

	int colStep = ceil((float)this->columns()/(float)nBlocks);
//	std::cout << "colStep = " << colStep << std::endl;

	int diffUp = ctrOfMass(0);
	int diffDown = this->rows() - ctrOfMass(0) - 1;
//	std::cout << "diffUp = " << diffUp << ", diffDown = " << diffDown << std::endl;

	profileUpOuterB.resize(nBlocks, -1);
	profileUpInnerB.resize(nBlocks, -1);
	profileDownOuterB.resize(nBlocks, -1);
	profileDownInnerB.resize(nBlocks, -1);

	int count = 0;
	for (int j=0; j<this->columns(); j+=colStep) {
		int nBars = -1;
		if (j+colStep-1 < columns()) {
			nBars = colStep;
		}
		else {
			nBars = columns()-j;
		}
		float sumUpOuter=0, sumUpInner=0, sumDownOuter=0, sumDownInner=0;
		for (int i=j; i<j+nBars; ++i) {
			sumUpOuter+=profileUpOuter[i];
			sumUpInner+=profileUpInner[i];
			sumDownOuter+=profileDownOuter[i];
			sumDownInner+=profileDownInner[i];
		}

		profileUpOuterB[count] = (diffUp*nBars!=0) ? (sumUpOuter / (float)(diffUp*nBars)) : 0;
		profileUpInnerB[count] = (diffUp*nBars!=0) ? (sumUpInner / (float)(diffUp*nBars)) : 0;
		profileDownOuterB[count] = (diffDown*nBars!=0) ? (sumDownOuter / (float)(diffDown*nBars)) : 0;
		profileDownInnerB[count] = (diffDown*nBars!=0) ? (sumDownInner / (float)(diffDown*nBars)) : 0;
		++count;
	}
	return;
}

template <>
void GrayImage<uint8>::computeVerticalProfileBlocks(const int nBlocks, const Index &center, std::vector<float> &profileLeftOuterB, std::vector<float> &profileLeftInnerB, std::vector<float> &profileRightOuterB, std::vector<float> &profileRightInnerB) const
{
	Index ctrOfMass;
	this->computeCenterOfMassIndex(ctrOfMass);
	std::vector<int> profileLeftOuter, profileLeftInner, profileRightOuter, profileRightInner;
	computeVerticalProfiles(ctrOfMass, profileLeftOuter, profileLeftInner, profileRightOuter, profileRightInner);

	int rowStep = ceil((float)this->rows()/(float)nBlocks);
//	std::cout << "rowStep = " << rowStep << std::endl;

	int diffLeft = ctrOfMass(1);
	int diffRight = this->columns() - ctrOfMass(1) - 1;
//	std::cout << "diffLeft = " << diffLeft << ", diffRight = " << diffRight << std::endl;

	profileLeftOuterB.resize(nBlocks, -1);
	profileLeftInnerB.resize(nBlocks, -1);
	profileRightOuterB.resize(nBlocks, -1);
	profileRightInnerB.resize(nBlocks, -1);

	int count = 0;
	for (int i=0; i<this->rows(); i+=rowStep) {
		int nBars = -1;
		if (i+rowStep-1 < this->rows()) {
			nBars = rowStep;
		}
		else {
			nBars = rows()-i;
		}
		float sumLeftOuter=0, sumLeftInner=0, sumRightOuter=0, sumRightInner=0;
		for (int j=i; j<i+nBars; ++j) {
			sumLeftOuter+=profileLeftOuter[j];
			sumLeftInner+=profileLeftInner[j];
			sumRightOuter+=profileRightOuter[j];
			sumRightInner+=profileRightInner[j];
		}
		profileLeftOuterB[count] = (diffLeft*nBars!=0) ? (sumLeftOuter / (float)(diffLeft*nBars)) : 0;
		profileLeftInnerB[count] = (diffLeft*nBars!=0) ? (sumLeftInner / (float)(diffLeft*nBars)) : 0;
		profileRightOuterB[count] = (diffRight*nBars!=0) ? (sumRightOuter / (float)(diffRight*nBars)) : 0;
		profileRightInnerB[count] = (diffRight*nBars!=0) ? (sumRightInner / (float)(diffRight*nBars)) : 0;
		++count;
	}
	return;
}

#if 0
template <>
void GrayImage<uint8>::resize(const int width, const int height, int interpolation/*=CV_INTER_LINEAR*/)
{
	if ( !empty() ) {
//		cvResize(mpImage, mpImage, interpolation);

#if 1
		std::cout << "resizing (spec)..." << std::endl;
		// store old image pointer:
		IplImage* pImageOld = mpImage;
		// create a new image with specified size:c
		mpImage = cvCreateImage( cvSize(width, height), determineDepth(), 1 );

		cvResize(pImageOld, mpImage, interpolation);
//		if (pImageOld != 0x0) {
//			cvReleaseImage(&pImageOld);
//		}
#endif
	}
	else {
		releaseImage();
		mpImage = cvCreateImage( cvSize(width, height), determineDepth(), 1 );
	}
}
#endif

//GrayImage::Iterator GrayImage::begin()
//{
//return(Iterator(this, 0, 0));
//}
//
//GrayImage::Iterator GrayImage::end()
//{
//return(Iterator(this, rows()-1, columns()));
//}


} // end of namespace impact_tr4

