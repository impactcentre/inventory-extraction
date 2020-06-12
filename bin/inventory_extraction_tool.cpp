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
// tests the image class

#include <QApplication>
#include <QImage>

//#include <qwt_plot.h>
//#include <qwt_plot_curve.h>
//#include <qwt_legend.h>

#include <iostream>
#include <exception>

#include <omp.h>

#include "core/core.h"
#include "core/Exception.hpp"
#include "core/image/GrayImage.hpp"

#include "gui/gui.h"
#include "gui/MainWindow.hpp"

#include "util/RandomNumber.hpp"

using namespace impact_tr4;

void test_sth()
{
	for (int j=0; j<10; j++) {
	StopWatch watch;
	watch.start();
	std::vector<int> rand_tuple = RandomNumber::random_unique_integer_tuple(10);
	watch.stop(true);

	std::cout << "random tuple: " << std::endl;
	for (int i=0; i<rand_tuple.size(); ++i) {
		std::cout << rand_tuple[i] << " ";
	}
	std::cout << std::endl;
	}
	return;
}

void test_knn()
{
	StopWatch watch;

	// create artificial data:
	int nSamples = 10;
	int nVars = 2;

	ublas::matrix<float> dataMatrix(nSamples, nVars);
	int nTestSamples = 1;
	ublas::matrix<float> testSampleMatrix(nTestSamples, nVars);
	ublas::vector<int> labelVec(nSamples);

	for (int i=0; i<dataMatrix.size1(); ++i) {
		for (int j=0; j<dataMatrix.size2(); ++j) {
//			dataMatrix(i,j) = i*j;
			dataMatrix(i,j) = float(RandomNumber::random_integer(1,1000)) / float(RandomNumber::random_integer(1,1000));
//			dataMatrix(i,j) = float(RandomNumber::random_integer(1,1000));
		}
		if (i < nTestSamples) {
			for (int j=0; j<dataMatrix.size2(); ++j) {
				testSampleMatrix(i,j) = float(RandomNumber::random_integer(1,1000)) / float(RandomNumber::random_integer(1,1000));
			}
		}
//		labelVec(i) = i % 5;
		labelVec(i) = i;
//		std::cout << labelVec(i) << " ";
	}
	std::cout << std::endl;
	std::cout << dataMatrix << std::endl;
	// convert to CvMat's:
	CvMat *train_data = OpenCV::cvMatFromBoostMat(dataMatrix);
	CvMat *samples = OpenCV::cvMatFromBoostMat(testSampleMatrix);
	CvMat *responses = OpenCV::cvMatFromBoostVec(labelVec, COLUMN_VECTOR);

	CvKNearest knearest;
	int max_k = 32;
	try {
		bool validknearest = knearest.train(train_data, responses, NULL, false, max_k);
		if (!validknearest)
			std::cerr << "ERROR IN KNEARTEST!!" << std::endl;
		else
			std::cout << "Juhuuu!!" << std::endl;

		int k = 11;
		ublas::vector<int> results(nTestSamples); results(0) = -1;
		CvMat *results_mat = OpenCV::cvMatFromBoostVec(results, ROW_VECTOR);
//		float **neighbors = alloc()
		const float **neighbors = NULL;
		neighbors = new const float*[k*nTestSamples];
		ublas::vector<float> neighbor_responses(k*nTestSamples);
		ublas::vector<float> dist(k*nTestSamples);

		for (int i=0; i<k; ++i) neighbors[i] = new float[nVars];

		int c = knearest.find_nearest(samples, k, results_mat, neighbors,
				OpenCV::cvMatFromBoostVec(neighbor_responses, ROW_VECTOR),
				OpenCV::cvMatFromBoostVec(dist, ROW_VECTOR)
				);
		std::cout << "predicted class = " << c << std::endl;
		std::cout << results << std::endl;
		std::cout << "pointers:" << std::endl;
		for (int i=0; i<k*nTestSamples; ++i) {
			for (int j=0; j<nVars; ++j) {
				std::cout << neighbors[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "neighbor_responses = " << std::endl << neighbor_responses << std::endl;
		std::cout << "dist = " << std::endl << dist << std::endl;

	}
	catch (cv::Exception e) {
		std::cout << "KNEAREEST ERROR: " << e.err << std::endl;
	}
}

void test_svm_read_write()
{
	StopWatch watch;

	// create artificial data:
	int nSamples = 46;
	int nVars = 40;

	ublas::matrix<float> dataMatrix(nSamples, nVars);
	ublas::vector<int> labelVec(nSamples);

	for (int i=0; i<dataMatrix.size1(); ++i) {
		for (int j=0; j<dataMatrix.size2(); ++j) {
//			dataMatrix(i,j) = i*j;
			dataMatrix(i,j) = float(RandomNumber::random_integer(1,1000)) / float(RandomNumber::random_integer(1,1000));
//			dataMatrix(i,j) = float(RandomNumber::random_integer(1,1000));
		}
//		labelVec(i) = i % 5;
		labelVec(i) = 0;
//		std::cout << labelVec(i) << " ";
	}
	std::cout << std::endl;
	// convert to CvMat's:
	CvMat *train_data = OpenCV::cvMatFromBoostMat(dataMatrix);
	CvMat *responses = OpenCV::cvMatFromBoostVec(labelVec, COLUMN_VECTOR);

	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::RBF;
	params.degree = 3;
	params.gamma = 1;
	params.coef0 = 2;
	params.C = 10;
	params.nu = 1;
	params.p = 0;
	params.class_weights = NULL;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1e6, 1e-6);

	std::string filename = "test_svm.xml";

//	watch.start();
//	// ergebnis: constructor mit parametern trainiert automatisch!!
//	CvSVM cvsvm_constructor(train_data, responses, 0, 0, params);
//	// ergebnis: man kann 2 verschiedene svm's NICHT in 1 file speichern -> name dann eher sinnlos...
//	cvsvm_constructor.save(filename.c_str(), "cullis_svm_2");
//	watch.stop();

	CvSVM cvsvm;
	watch.start();
//	bool flag = cvsvm.train(train_data, responses, 0, 0, params);
	bool flag = cvsvm.train_auto(train_data, responses, 0, 0, params);
	watch.stop();
	std::cout << "trained test svm, flag = " << flag << std::endl;
	std::cout << "nr. of supp.vecs. = " << cvsvm.get_support_vector_count() << std::endl;
	cvsvm.save(filename.c_str());
	std::cout << "saved test svm" << std::endl;

	CvSVM cvsvm2;
	try {
		cvsvm2.load(filename.c_str());
	}
	catch (cv::Exception e) {
		std::cout << e.err << std::endl;
	}
	std::cout << "loaded test svm" << std::endl;
	std::cout << "nr. of supp.vecs. = " << cvsvm2.get_support_vector_count() << std::endl;


	return;
}

int main(int argc, char **argv)
{
//#if DEBUG
//	my_out.rdbuf(std::cout.rdbuf());
//#endif

//	test_sth();
//	test_svm_read_write();
//	test_knn();

#if 1 // set zero, s.t. gui is not started (for testing purposes e.g.)
	QApplication app(argc, argv); // for gui-apps

	std::cout << "Starting clustering application..." << std::endl;

	MainWindow mainwindow;
	mainwindow.show();

	return app.exec();
#else
	return 0;
#endif
}
