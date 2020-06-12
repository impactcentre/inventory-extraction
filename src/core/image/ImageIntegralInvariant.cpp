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
#include "ImageIntegralInvariant.hpp"

#include "BinaryImageMoments.hpp"

namespace impact_tr4 {
//calcIntegralInvariant( int radius = 3, InvariantType invType = BALL )
int ImageIntegralInvariant::calcIntegralInvariant( const Kernel<float>& mask )
{
//	mpImage->show("mpImage");
	// --- binarize the image:
	GrayImage<> imageBin;
	ImageFilter::threshold<uint8>( *mpImage, imageBin );

//	std::cout << "image bin size: " << imageBin.rows() << ", " << imageBin.columns() << std::endl;
//	std::cout << "Image Bin: " << std::endl << imageBin << std::endl;
//	imageBin.show("imageBin");

	// --- calculate the gradient image using morpholical gradient operation:
	GrayImage<> morphGradImage;
	const int nIts = 1;
	Morphology::gradient<uint8>( imageBin, morphGradImage, nIts );
//	morphGradImage.show("morphGradImage");
	// --- set edge in gradient image to 1:
	morphGradImage.binarize( 128, 1 );
	//	std::cout << morphGradImage << std::endl;
//	std::cout << "The invariant kernel is:" << "\n" << mask << std::endl;
	mSizeKernelNonZero = mask.countNonZero();
//	std::cout << "The nr of non zero elements in the kernel is: " << mSizeKernelNonZero << std::endl;

	// --- convolute image ( or binarization? ) with the invariant kernel:
	GrayImage<uint16> convImage;
	imageBin.binarize( 128, 1 );
	GrayImage<> imageBinInv;
	ImOp::subScalar<uint8>(1, imageBin, imageBinInv );

//	std::cout << imageBinInv << std::endl;
	// perform conv. on original gray image:
//	ImageFilter::filter2D< uint8, uint16 >( *mpImage, convImage, mask );
	// OR perform conv. on binarized image:
	ImageFilter::filter2D< uint8, uint16 >( imageBinInv, convImage, mask );
//	std::cout << convImage << std::endl;
//	convImage.show( "convImage" );
	// --- calculate the convoluted image on the edge and store it in convImageEdge:
	GrayImage<uint16> convImageEdge, morphGradImage16u;
#if 1
	ImOp::convert<uint8, uint16>( morphGradImage, morphGradImage16u ); // convert to 16u image
#else
	ImOp::convert<uint8, uint16>( imageBin, morphGradImage16u ); // convert to 16u image
#endif
	ImOp::mul<uint16>( convImage, morphGradImage16u, convImageEdge );
//	std::cout << convImageEdge << std::endl;

	mResultVec.clear();
	int size = 0;
	for (int i=0; i < convImageEdge.rows(); ++i) {
		for (int j=0; j < convImageEdge.columns(); ++j) {
			float val = convImageEdge(i,j);
			if (val!=0)
				{
				// store integral invariant value:
				mResultVec.resize ( ++size, true );
				mResultVec[size-1] = val;
				// store corresponding index:
				mIndicesVec.resize ( size, true );
				mIndicesVec[size-1] = Index(i, j);
//				mResultVec.push_back( val );
//				std::cout << val << std::endl;
				}
		} // end for j
	} // end for i
	return 1;
} // end calcIntegralInvariant()

/*static*/
void ImageIntegralInvariant::calcIntegralInvariantImage( const Kernel<float>& mask, const GrayImage<> &src, GrayImage<float> &result )
{
		// --- calculate the gradient image using morpholical gradient operation:
		GrayImage<> morphGradImage;
		const int nIts = 1;
		Morphology::gradient<uint8>( src, morphGradImage, nIts );
	//	morphGradImage.show("morphGradImage");
		// --- set edge in gradient image to 1:
		morphGradImage.binarize( 128, 1 );
		//	std::cout << morphGradImage << std::endl;
	//	std::cout << "The invariant kernel is:" << "\n" << mask << std::endl;
		int sizeKernelNonZero = mask.countNonZero();
	//	std::cout << "The nr of non zero elements in the kernel is: " << mSizeKernelNonZero << std::endl;

		// --- convolute image ( or binarization? ) with the invariant kernel:
		GrayImage<> imageBin = src;
		GrayImage<uint16> convImage(src.width(), src.height());
		imageBin.binarize( 128, 1 );

		// perform conv. on original gray image:
	//	ImageFilter::filter2D< uint8, uint16 >( *mpImage, convImage, mask );
		// OR perform conv. on binarized image:
		ImageFilter::filter2D< uint8, uint16 >( imageBin, convImage, mask );
	//	std::cout << convImage << std::endl;
	//	convImage.show( "convImage" );
		// --- calculate the convoluted image on the edge and store it in convImageEdge:
		GrayImage<uint16> convImageEdge, morphGradImage16u;

		result.init(src.width(), src.height()); // TEST
	#if 1
		ImOp::convert<uint8, uint16>( morphGradImage, morphGradImage16u ); // convert to 16u image
	#else
		ImOp::convert<uint8, uint16>( imageBin, morphGradImage16u ); // convert to 16u image
	#endif
		ImOp::mul<uint16>( convImage, morphGradImage16u, convImageEdge );

//		ImOp::convert<uint16, uint8>( convImageEdge, result );
		ImOp::convert<uint16, float>( convImageEdge, result );
		GrayImage<float> scaleFactorImage(src.width(), src.height());
		scaleFactorImage.fill((1.0f)/(float)(sizeKernelNonZero));
		GrayImage<float> tmp = result;
		ImOp::mul(tmp, scaleFactorImage, result);

	//	std::cout << convImageEdge << std::endl;
		return;
} // end calcIntegralInvariantImage


} // end of namespace impact_tr4
