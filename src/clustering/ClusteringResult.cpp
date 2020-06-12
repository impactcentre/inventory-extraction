/*
 * ClusteringResult.cpp
 *
 *  Created on: 06.08.2009
 *      
 */

//#include <boost/bind/bind.hpp>

#include "ClusteringResult.hpp"

#include "clustering/Document.hpp"
#include "clustering/CharCluster.hpp"
#include "clustering/features/BasicMomentsFeature.hpp"
#include "clustering/features/HuMomentsFeature.hpp"
#include "clustering/features/ProfilesFeatures.hpp"

namespace impact_tr4 {

ClusteringResult::ClusteringResult()
{
	mClusterList.clear();

	mTrashClusterList.clear();
	CharCluster *pTrashCluster = new CharCluster();
	mTrashClusterList.push_back(pTrashCluster);

//	mTrashCluster.clear();
	addDefaultPrototypeFeatures();
	clearPrototypeDistanceMatrix();
}

ClusteringResult::ClusteringResult(const ClusteringResult& source)
{
//	std::cout << "makying a copy of ClusteringResult!!!" << std::endl;
//	std::cout << "nr of clusters in source: " << source.nClusters() << std::endl;

	mClusterList.clear();
	for (CharClustConstIt clust_it = source.begin(); clust_it!=source.end(); ++clust_it) {
		CharCluster *pCharCluster = new CharCluster(*(*clust_it));
		mClusterList.push_back(pCharCluster);
	}
	mPrototypeFeatures = CharFeatureCollection(); addDefaultPrototypeFeatures();

	mTrashClusterList.resize(1);
	mTrashClusterList.front() = new CharCluster(*source.mTrashClusterList.front());

//	mTrashCluster = source.mTrashCluster;
//	mPrototypeFeatures = source.mPrototypeFeatures;

	mDistMat = source.mDistMat;
	mIsDistMatUpToDate = source.mIsDistMatUpToDate;
//	std::cout << "Done copying ClusteringResult!" << std::endl;
}

ClusteringResult::~ClusteringResult()
{
	deleteClustering();
	return;
}

const ClusteringResult& ClusteringResult::operator=(const ClusteringResult &rhs)
{
	if (this != &rhs) {
		this->deleteClustering();

		for (CharClustConstIt clust_it = rhs.begin(); clust_it!=rhs.end(); ++clust_it) {
			CharCluster *pCharCluster = new CharCluster(*(*clust_it));
			mClusterList.push_back(pCharCluster);
		}
		delete mTrashClusterList.front();
		mTrashClusterList.front() = new CharCluster(*rhs.mTrashClusterList.front());

		// NOTE: no copy-constructor for class CharFeatureCollection available, so have to make a new ojbect with default features:
		mPrototypeFeatures.deleteFeatures();
		mPrototypeFeatures = CharFeatureCollection(); addDefaultPrototypeFeatures();
//		mPrototypeFeatures = rhs.mPrototypeFeatures;

		mDistMat = rhs.mDistMat;
		mIsDistMatUpToDate = rhs.mIsDistMatUpToDate;
	}
	return *this;
}

void ClusteringResult::deleteClustering()
{
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		delete *clust_it;
	}
	mClusterList.clear();
//	mTrashClusterList.front()->clear();
	clearPrototypeDistanceMatrix();
	return;
}

void ClusteringResult::addDefaultPrototypeFeatures()
{
//	mPrototypeFeatures.addFeature(new BasicMomentsFeature());
//	mPrototypeFeatures.addFeature(new HuMomentsFeature());
	mPrototypeFeatures.addFeature(new ProfilesFeatures(5, 5, true, true, true, true));

	return;
}

CharClustIt ClusteringResult::getClusterItFromIndex(const int i)
{
	if (i < 0 || i >= this->nClusters()) {
		throw IndexOutOfBoundsException("Given cluster index is out of bounds!");
	}

	CharClustIt it = this->begin();
	int c = 0;
	while (it != this->end()) {
		if (c==i) {
			return it;
		}
		++c;
		++it;
	}
	throw IndexOutOfBoundsException("Given cluster index is out of bounds!");
}

int ClusteringResult::getClusterIndex(CharClustIt clustIt)
{
	CharClustIt it = this->begin();
	int c = 0;
	while (it != this->end()) {
		if (it==clustIt) {
			return c;
		}
		++c;
		++it;
	}
	return -1;
//	throw Exception("Cluster not found in getClusterIndex()!");
}

void ClusteringResult::updatePrototypes(const bool forceAll/*=false*/)
{
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		if (!(*clust_it)->isPrototypeUpToDate() || forceAll) { // only update prototype if not up to date!
			(*clust_it)->recomputePrototypeImage();

			// update feature for prototype:
//			int cluster_index = this->getClusterIndex(clust_it);
//			std::cout << "updating feature for cluster " << cluster_index << std::endl;
//			mPrototypeFeatures.calculateOutputDataRow(
//						*(*clust_it)->getPrototypeImageThreshPointer(),
//						cluster_index,
//						mPrototypeFeatures.dataMatrixRef());
		}
	}
	return;
}

void ClusteringResult::computePrototypeFeatures()
{
	std::vector<GrayImage<>* > prototypeImagePointers;
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
//		if (!(*clust_it)->hasPrototypeImage()) {
//			(*clust_it)->recomputePrototypeImage();
//		}
		prototypeImagePointers.push_back((*clust_it)->getPrototypeImageThreshPointer());
	}
	std::cout << "computing features for prototypes..." << std::endl;
	StopWatch watch; watch.start();
	mPrototypeFeatures.calculateOutputDataMatrix(prototypeImagePointers);
	watch.stop();

	return;
}

void ClusteringResult::computeUnsorted(std::vector<CharClustIt>& unsorted)
{
	std::cout << "Retrieving unsorted list..." << std::endl;
	unsorted.clear();

	// create vector of cluster iterators:
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		unsorted.push_back(clust_it);
	}

	return;
}

void ClusteringResult::computeSizeSorting(std::vector<CharClustIt>& sizeSorting)
{
	std::cout << "Doing size sorting..." << std::endl;
	sizeSorting.clear();

	// create vector of cluster iterators:
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		sizeSorting.push_back(clust_it);
	}
	// sort vector using custom compare function
	std::sort(sizeSorting.begin(), sizeSorting.end(), compareCharClustItBySize);

	return;
}

void ClusteringResult::computekNNSorting(CharClustIt clustIt, std::vector<CharClustIt>& kNNSorting)
{
	std::cout << "Doing knn sorting..." << std::endl;
	kNNSorting.clear();
	StopWatch watch;

	int sampleIndex = getClusterIndex(clustIt);

	watch.start();
	std::vector<int> kNNSortingIndex(this->nClusters());

	ublas::vector<int> responses(this->nClusters());
	for (int i=0; i<this->nClusters();++i) { responses[i] = i; }

	ublas::matrix<float> &dataMat = mPrototypeFeatures.dataMatrixRef();
	ublas::matrix<float> sample(1, dataMat.size2());
	for (int j=0; j<dataMat.size2(); ++j)
		sample(0,j) = dataMat(sampleIndex, j);

	CvKNearest knearest;
	int max_k = 1e6;
	bool validknearest = knearest.train(
			OpenCV::cvMatFromBoostMat(dataMat),
			OpenCV::cvMatFromBoostVec(responses, COLUMN_VECTOR),
			NULL, false, max_k);

	int k = this->nClusters();
	ublas::vector<int> results(1);
	ublas::vector<float> neighbor_responses(k);
	ublas::vector<float> dist(k);

	knearest.find_nearest(OpenCV::cvMatFromBoostMat(sample),
						k, OpenCV::cvMatFromBoostVec(results, COLUMN_VECTOR),
						NULL,
						OpenCV::cvMatFromBoostVec(neighbor_responses, ROW_VECTOR),
						OpenCV::cvMatFromBoostVec(dist, ROW_VECTOR)
						);

	kNNSorting.clear();
	for (int i=0; i<neighbor_responses.size(); ++i) {
		kNNSorting.push_back(this->getClusterItFromIndex(neighbor_responses[i]));
	}
	watch.stop();

	std::cout << "neighbor_responses = " << std::endl << neighbor_responses << std::endl;
	std::cout << "dist = " << std::endl << dist << std::endl;

	return;
}

void ClusteringResult::sortAllClustersByDistanceToClusterCenter()
{
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		(*clust_it)->sortClusterByDistanceToClusterCenter();
	}
	return;
}

int ClusteringResult::nTotalElements() const
{
	int count = 0;
	for (CharClustConstIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		count += (*clust_it)->size();
	}
	return count;
}

void ClusteringResult::print()
{
	int i = 1;
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		ImageCharConstIt it = (*clust_it)->begin();
		std::cout << "Cluster " << i++ << ":" << std::endl;
		while (it != (*clust_it)->end()) {
			std::cout << *(*it) << std::endl;
			++it;
		}
	}
	return;
}

CharCluster* ClusteringResult::addEmptyCluster(Document *pDocument)
{
	CharCluster *pCluster = new CharCluster();
	mClusterList.push_back(pCluster);
	mIsDistMatUpToDate = false;
	return mClusterList.back();
}

void ClusteringResult::removeCluster(const CharClustIt it)
{
	mClusterList.erase(it);
	mIsDistMatUpToDate = false;
}

void ClusteringResult::removeEmptyClusters()
{
	// gather iterators of empty clusters:
	std::vector<CharClustIt> emptyClusterIts;
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		if ((*clust_it)->empty()) {
			emptyClusterIts.push_back(clust_it);
		}
	}
	for (int i=0; i<emptyClusterIts.size(); ++i) {
		this->removeCluster(emptyClusterIts[i]);
	}
	return;
}

void ClusteringResult::mergeClusters(const CharClustIt from, const CharClustIt to)
{
	if (from != to) {
		(*to)->appendCluster(*(*from));
		this->removeCluster(from);
	}
	this->updatePrototypes();
	mIsDistMatUpToDate = false;
	return;
}

void ClusteringResult::moveElements(const CharClustIt from, const CharClustIt to, std::vector<ImageCharIt> iterators)
{
	if (from != to) {
		for (int i=0; i<iterators.size(); ++i) {
			(*to)->addChar(*iterators[i]);
		}
		(*from)->removeChars(iterators);
	}

	this->updatePrototypes();
	mIsDistMatUpToDate = false;
	return;
}


void ClusteringResult::moveToTrash(const CharClustIt from, std::vector<ImageCharIt> iterators)
{
	for (int i=0; i<iterators.size(); ++i) {
		mTrashClusterList.front()->addChar(*iterators[i]);
	}
	(*from)->removeChars(iterators);

	this->updatePrototypes();
	mIsDistMatUpToDate = false;
	return;
}

void ClusteringResult::moveClusterToTrash(const CharClustIt it)
{
	std::vector<ImageCharIt> clusterContent;
	for (ImageCharIt imIt = (*it)->begin(); imIt != (*it)->end(); ++imIt) {
		clusterContent.push_back(imIt);
	}
	this->moveToTrash(it, clusterContent);
	this->removeCluster(it);

	this->updatePrototypes();
	mIsDistMatUpToDate = false;
	return;
}

void ClusteringResult::clearTrash()
{
	mTrashClusterList.front()->clear();

	return;
}

void ClusteringResult::removeElements(const CharClustIt from, std::vector<ImageCharIt> iterators)
{
	(*from)->removeChars(iterators);

	this->updatePrototypes();
	mIsDistMatUpToDate = false;
	return;
}

void ClusteringResult::computePrototypeDistanceMatrix()
{
	const int nClusts = this->nClusters();

	std::vector<GrayImage<> > clustPrototypeImages(nClusts);
	std::vector<GrayImage<float> > clustPrototypeIntInvImages(nClusts);
	std::vector<GrayImage<float> > clustPrototypeDistTransformImages(nClusts);

	// compute preprocessings for prototypes:
	std::cout << "computing pp for prototypes...";

	CharClustIt it = this->begin();

	for (int i=0; it!=this->end(); ++it, ++i) {
//		std::cout << "i=" << i << std::endl;
		clustPrototypeImages[i] = *((*it)->getPrototypeImageThreshPointer());
	}

//	std::cout << "1...";

	#pragma omp parallel for
	for (int i=0; i<nClusts; ++i) {
		PreprocessingResults::computeOtherPreprocessings(	clustPrototypeImages[i],
															clustPrototypeIntInvImages[i],
															clustPrototypeDistTransformImages[i]);
	}

//	std::cout << "2..." << std::endl;

	// compute distance matrix:
	std::cout << "computing distance matrix..." << std::endl;
	StopWatch watch;
	watch.start();
	mDistMat.resize( nClusts, nClusts );
	#pragma omp parallel for
	for (int i=0; i<nClusts; ++i) {
//		#pragma omp parallel for
		for (int j=i; j<nClusts; ++j) {
			if (i==j) {
				mDistMat(i,j) = 0;
				continue;
			}

			float dist = signatureDistance(	clustPrototypeImages[i],
											clustPrototypeImages[j],

											clustPrototypeDistTransformImages[i],
											clustPrototypeDistTransformImages[j],

											clustPrototypeIntInvImages[i],
											clustPrototypeIntInvImages[j] );

			mDistMat(i,j) = dist;
			mDistMat(j,i) = dist;
		}
	}
	std::cout << "computed prototype distance matrix, time = " << watch.stop() << std::endl;
//	std::cout << mDistMat << std::endl;

    mIsDistMatUpToDate = true;
} // end computePrototypeDistanceMatrix()

void ClusteringResult::reClusterHierarchical(int nClusts)
{
	std::cout << "reclustering clustering result!" << std::endl;
	// first: remove empty clusters:
	this->removeEmptyClusters();

	if (!isDistMatUpToDate()) {
		std::cout << "have to recompute prototype-distance-matrix..." << std::endl;

		computePrototypeDistanceMatrix();
	}

	ublas::matrix<float> &protoDistMat = this->prototypeDistanceMatrix();
	int nSamples = protoDistMat.size1();

	if (nClusts >= nSamples) {
//		std::cerr << "error: nr of clusters greater than number of clusters!" << std::endl;
		throw Exception("Nr. of new clusters greater than number of old clusters!");
	}

	double **tmpdistmatrix;

	// Allocate memory for distance matrix
	tmpdistmatrix = new double*[nSamples];
	for (int i = 0; i < nSamples; ++i)
		tmpdistmatrix[i] = new double[nSamples];
	// copy distance matrix
	for (int i=0;i<nSamples;++i){
		for (int j=0; j<=i; ++j) {
			tmpdistmatrix[i][j] = protoDistMat(i,j);
			tmpdistmatrix[j][i] = protoDistMat(i,j);
		}
	}
	std::vector<int> labels(nSamples);

	// method = 's' (single-linkage), 'm' (complete-linkage), 'a' (average-linkage) or 'c' (centroid-linkage):
	char methodChar='a';

	Node* tree = treecluster(nSamples, 1, NULL, NULL, NULL, 0, '_', methodChar, tmpdistmatrix);
	if (tree==NULL)
		std::cerr << "FATAL ERROR - NULL POINTER IN CLUSTER RESULT!" << std::endl;

	// cut hierarchical cluster tree at specified nr of clusters:
	cuttree(nSamples, tree, nClusts, &labels[0]);
	delete [] tree;

	// De-Allocate memory for temporary distance matrix:
	for (int i = 0; i < nSamples; ++i)
		delete [] tmpdistmatrix[i];
	delete [] tmpdistmatrix;
	std::cout << "finished agglomerative clustering with cluster library!!" << std::endl;

	this->mergeClustersByLabelVector(labels);

	return;
}

void ClusteringResult::mergeClustersByLabelVector(const std::vector<int>& labels)
{
	std::vector<std::pair<int, CharClustIt> > label_clusterit_pairings;

	// create pairings of labels and the corresponding cluster iterators:
	int i=0;
	for (CharClustIt clust_it = begin(); clust_it!= end(); ++clust_it) {
		label_clusterit_pairings.push_back(std::pair<int, CharClustIt>(labels[i], clust_it));
		++i;
	}
	// sort pairing according to their labels (i.e. first entry in each pair):
	std::sort(label_clusterit_pairings.begin(), label_clusterit_pairings.end(),
			compareIntCharClustItPairings);

	int current_cluster_index = -1e6;
	CharClustIt currentClustIt;
	for (int i=0; i<label_clusterit_pairings.size(); ++i) {
		std::pair<int, CharClustIt> p = label_clusterit_pairings[i];
		// if new list of labels begins, store index and the iterator to the first cluster:
		if (p.first != current_cluster_index) {
			current_cluster_index = p.first;
			currentClustIt = p.second;
		}
		// else: we are currently in a list of equal labels --> merge into first cluster of this label!
		else {
			this->mergeClusters(p.second , currentClustIt);
		}
	}

	std::cout << "merged clusters by label!" << std::endl;

	return;
}


void ClusteringResult::createClusteringResultFromLabelVector(const std::vector<int>& labels, Document *pDocument)
{
	this->deleteClustering();
	std::vector<ImageChar*> &imageCharVecRef = pDocument->getImageCharsVecRef();

	std::vector<std::pair<int, int> > label_index_pairings;

	// create pairings of labels with their indices:
	for (int i=0; i<labels.size(); ++i) {
		label_index_pairings.push_back(std::pair<int, int>(labels[i], i));
	}
	// sort pairing according to their labels (i.e. first entry in each pair):
	std::sort(label_index_pairings.begin(), label_index_pairings.end());

	int current_cluster_index = -1e6;
	CharCluster *pCurrCluster = NULL;
	for (int i=0; i<label_index_pairings.size(); ++i) {
		std::pair<int, int> p = label_index_pairings[i];
		if (current_cluster_index != p.first) { // new cluster!
			current_cluster_index = p.first;//FIXME!!!!!!!!!!!!!!!!!!!!!
			if (current_cluster_index >= 0) { // only add cluster if this is not the trash cluster!
				pCurrCluster = this->addEmptyCluster(pDocument);
			}
		}
		if (current_cluster_index >= 0) {
			pCurrCluster->addChar(imageCharVecRef[p.second]);
		}
		else {
			std::cout << "index = " << p.first << " index 2 = " << p.second << std::endl;
			mTrashClusterList.front()->addChar(imageCharVecRef[p.second]);
		}
	}

	this->updatePrototypes();

	return;
} // end createClusteringResultFromLabelVector

} // end of namespace impact_tr4
