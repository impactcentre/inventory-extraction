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
#ifndef CLUSTERINGCONTROLLER_HPP_
#define CLUSTERINGCONTROLLER_HPP_

#include <iostream>
#include <vector>
#include <cstring>

#include "core_headers.h"
#include "image_headers.h"

#include "DataStructs.hpp"
#include "ClusteringResult.hpp"
#include "CharFeatureCollection.hpp"
#include "Document.hpp"
#include "data_reader/CharDataReaderI.hpp"

namespace impact_tr4
{

class ClusteringController : public QObject
{
	Q_OBJECT
public:
	ClusteringController();
	virtual ~ClusteringController();

	void parseInputFiles(const ParsingParameters parsingParameters, const std::vector<std::string>& inputFns);

#if 1
	void preprocessGlyphs(const PreprocessingParameters parameters);

	void createFeatures(const PreprocessingParameters parameters,
						const PCAReductionParams &pcaParameters);
#endif

	void storeDataMatrix(const std::string& fn);

	void clusterGlyphs(	const PreprocessingParameters parameters,
						const PCAReductionParams pcaParameters,
						const ClusterMethodType clusterMethod,
						const ClusterMethodParameters* pParameters);

	/** Clears all previously stored data. */
	void clear();
	void createClusterImage( GrayImage<> &image );

	GrayImage<>& resultImage() { return mResultImage; }
	const bool hasImage() const { return !mResultImage.empty(); }
	ClusteringResult* clusteringResultPointer() { return &mClusteringResult; }
	Document* documentPointer() { return &mDocument; }
//public slots:
//	void clusteringFinished();

signals:
	void parsingFinished(bool success);
	void clusteringFinished(bool success);

private:
	Document mDocument;
	ClusteringResult mClusteringResult;
	GrayImage<> mResultImage;
};

} // end of namespace impact_tr4

#endif /*CLUSTERING_HPP_*/
