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
#include "ClassificationWidget.hpp"

#include "core/OpenCV.hpp"
#include "util/StopWatch.hpp"
#include "util/RandomNumber.hpp"
#include <QtGui>

namespace impact_tr4 {

ClassificationWidget::ClassificationWidget(ClusteringController *pClusteringController, QWidget *parent)
    : QWidget(parent)
{
	mpClusteringController = pClusteringController;
	ui.setupUi(this);

	QFont infoFont("Arial", 11, QFont::Bold);
	ui.loadedTrainingSetInfoLabel->setFont(infoFont);
	ui.crossValidationInfoLabel->setFont(infoFont);
	this->updateTrainedModelInfo();
	this->updateCrossValidationInfo();
	this->on_kernelComboBox_currentIndexChanged(ui.kernelComboBox->currentIndex());

	this->on_setDefaultGridsPushButton_clicked();
}

ClassificationWidget::~ClassificationWidget()
{

}

CvParamGrid ClassificationWidget::getDegreeGrid() const
{
	CvParamGrid grid;
	grid.min_val = ui.degreeMinSpinBox->value();
	grid.max_val = ui.degreeMaxSpinBox->value();
	grid.step = ui.degreeStepSpinBox->value();

	return grid;
}

CvParamGrid ClassificationWidget::getCoef0Grid() const
{
	CvParamGrid grid;
	grid.min_val = ui.coef0MinSpinBox->value();
	grid.max_val = ui.coef0MaxSpinBox->value();
	grid.step = ui.coef0StepSpinBox->value();

	return grid;
}

CvParamGrid ClassificationWidget::getGammaGrid() const
{
	CvParamGrid grid;
	grid.min_val = ui.gammaMinSpinBox->value();
	grid.max_val = ui.gammaMaxSpinBox->value();
	grid.step = ui.gammaStepSpinBox->value();

	return grid;
}

CvParamGrid ClassificationWidget::getCGrid() const
{
	CvParamGrid grid;
	grid.min_val = ui.CMinSpinBox->value();
	grid.max_val = ui.CMaxSpinBox->value();
	grid.step = ui.CStepSpinBox->value();

	return grid;
}

void ClassificationWidget::on_setDefaultGridsPushButton_clicked()
{
	ui.nFoldSpinBox->setValue(10);

	CvParamGrid degree_grid = CvSVM::get_default_grid(CvSVM::DEGREE);
	CvParamGrid coef_grid   = CvSVM::get_default_grid(CvSVM::COEF);
	CvParamGrid gamma_grid  = CvSVM::get_default_grid(CvSVM::GAMMA);
	CvParamGrid C_grid      = CvSVM::get_default_grid(CvSVM::C);

	ui.degreeMinSpinBox->setValue(degree_grid.min_val);
	ui.degreeMaxSpinBox->setValue(degree_grid.max_val);
	ui.degreeStepSpinBox->setValue(degree_grid.step);

	ui.coef0MinSpinBox->setValue(coef_grid.min_val);
	ui.coef0MaxSpinBox->setValue(coef_grid.max_val);
	ui.coef0StepSpinBox->setValue(coef_grid.step);

	ui.gammaMinSpinBox->setValue(gamma_grid.min_val);
	ui.gammaMaxSpinBox->setValue(gamma_grid.max_val);
	ui.gammaStepSpinBox->setValue(gamma_grid.step);

	ui.CMinSpinBox->setValue(C_grid.min_val);
	ui.CMaxSpinBox->setValue(C_grid.max_val);
	ui.CStepSpinBox->setValue(C_grid.step);

	return;
}

void ClassificationWidget::loadTrainingDataFromClusteringResult(const bool doRandomPermutation/*=true*/)
{
	ClusteringResult *pClusteringResult = mpClusteringController->clusteringResultPointer();
	Document *pDocument = mpClusteringController->documentPointer();
	CharFeatureCollection *pCharFeatureCollection = pDocument->getCharFeatureCollectionPointer();
//    std::vector<ImageChar*> *imageCharVecPointer = pDocument->getImageCharsVecPointer();
    ublas::matrix<float>& totalDataMatrix = pCharFeatureCollection->dataMatrixRef();
    //	std::cout << dataMatrix << std::endl;
    //	std::cout << dataMatrix.size1() << ", " << dataMatrix.size2() << std::endl;
    const int nTotalSamples = totalDataMatrix.size1();
    const int nFeatures = totalDataMatrix.size2();
    if (nTotalSamples != pDocument->nParsedImages()) {
    	throw Exception("Number of features does not correspond to number of loaded image chars");
    }
    std::cout << "nTotalSamples = " << nTotalSamples << std::endl;

	// --- first: look, who many samples are actually labelled:
    int nLabelledSamples = 0;
//    std::cout << "nclusters = " << pClusteringResult->nClusters() << std::endl;
    for (CharClustIt clust_it = pClusteringResult->begin(); clust_it != pClusteringResult->end(); ++clust_it) {
    	CharCluster *pActCluster = (*clust_it);
//    	std::cout << "unicode of cluster " << i << " = " << pActCluster->unicodeLabel() << std::endl;
    	if (pActCluster->unicodeLabel() != 0) {
			nLabelledSamples += pActCluster->size();
		}
    } // end for all clusters i
    if (nLabelledSamples == 0) {
    	throw NoDataException("No labelled samples available!");
    }

    std::cout << "nLabelledSamples = " << nLabelledSamples << std::endl;
    // --- second: make room for data matrix and labels and store them seperately:
    mSVMDataMatrix.resize(nLabelledSamples, nFeatures);
    mLabelVector.resize(nLabelledSamples);

    int actRowCount = 0;
    for (CharClustIt clust_it = pClusteringResult->begin(); clust_it != pClusteringResult->end(); ++clust_it) {
    	CharCluster *pActCluster = (*clust_it);
    	if (pActCluster->unicodeLabel() != 0) {
    		ImageCharIt it = pActCluster->begin();
			while (it!=pActCluster->end()){
				ublas::matrix_row<ublas::matrix<float> > matRow(*(*it)->pDataMat, (*it)->dataRow);
				for (int j=0; j<matRow.size(); ++j) {
					mSVMDataMatrix(actRowCount, j) = matRow(j);
				}
				mLabelVector(actRowCount) = pActCluster->unicodeLabel(); // <---------------
				++actRowCount;
				++it;
			}
		}
    } // end for all clusters i
    assert(actRowCount==nLabelledSamples);

    if (doRandomPermutation) {
    	ublas::matrix<float> tmpMatrix = mSVMDataMatrix;
    	ublas::vector<int> tmpLabels = mLabelVector;

    	std::vector<int> random_tuple;
    	permuteBoostMatrixRandomlyRowwise(tmpMatrix, mSVMDataMatrix, random_tuple);
    	permuteBoostVectorRandomlyRowwise(tmpLabels, mLabelVector, random_tuple);
    }

    // TEST: fill matrix with random values:
#if 0
	for (int i=0; i<mSVMDataMatrix.size1(); ++i) {
		for (int j=0; j<mSVMDataMatrix.size2(); ++j) {
			mSVMDataMatrix(i,j) = float(RandomNumber::random_integer(1,1000)) / float(RandomNumber::random_integer(1,1000));
		}
		mLabelVector(i) = i % 50;
	}
#endif

    return;
}

void ClassificationWidget::on_kernelComboBox_currentIndexChanged(int i)
{
	if (i==0) { // linear kernel
		ui.degreeDoubleSpinBox->setEnabled(false);
		ui.coef0DoubleSpinBox->setEnabled(false);
		ui.gammaDoubleSpinBox->setEnabled(false);
	}
	else if (i==1) { // polynomial kernel
		ui.degreeDoubleSpinBox->setEnabled(true);
		ui.coef0DoubleSpinBox->setEnabled(true);
		ui.gammaDoubleSpinBox->setEnabled(true);
	}
	else if (i==2) { // rbf kernel
		ui.degreeDoubleSpinBox->setEnabled(false);
		ui.coef0DoubleSpinBox->setEnabled(false);
		ui.gammaDoubleSpinBox->setEnabled(true);
	}
	else if (i==3) { // sigmoid kernel
		ui.degreeDoubleSpinBox->setEnabled(false);
		ui.coef0DoubleSpinBox->setEnabled(true);
		ui.gammaDoubleSpinBox->setEnabled(true);
	}
	else {
		throw Exception("Fatal error: wrong kernel in combo box!!");
	}
	return;
}

void ClassificationWidget::on_trainPushButton_clicked()
{
	try {
		StopWatch watch;
		std::cout << "Start training SVM with labelled samples!" << std::endl;
		// gather parameters:
		CvSVMParams params = getTrainingParameters();
		std::cout << "kernel_type is : " << params.kernel_type << std::endl;
		std::cout << "degree is: " << params.degree << std::endl;
		std::cout << "gamma is: " << params.gamma << std::endl;
		std::cout << "coef0 is: " << params.coef0 << std::endl;
		std::cout << "C is: " << params.C << std::endl;
		// load data from clustering result:
		loadTrainingDataFromClusteringResult(true);
		std::cout << "data matrix = " << std::endl;
		std::cout << mSVMDataMatrix << std::endl;
		std::cout << "label vector = " << std::endl;
		std::cout << mLabelVector << std::endl;
		// train the svm:
		CvMat *train_data = OpenCV::cvMatFromBoostMat(mSVMDataMatrix);
		CvMat *responses = OpenCV::cvMatFromBoostVec(mLabelVector, COLUMN_VECTOR);
		CvMat *var_idx = NULL; // indices of variables to be used; not used till today
		CvMat *sample_idx = NULL; // indices of samples to be used; not used till today
		mSVM.clear();
		bool flag = false;
		if (!ui.crossValidationGroupBox->isChecked()) { // if no cross validation -> train with parameters of gui
			flag = mSVM.train(train_data, responses, var_idx, sample_idx, params);
		}
		else { // if cross validation -->
			int nFold = ui.nFoldSpinBox->value();
			CvParamGrid degreeGrid = getDegreeGrid();
			CvParamGrid coef0Grid = getCoef0Grid();
			CvParamGrid gammaGrid = getGammaGrid();
			CvParamGrid cGrid = getCGrid();
			std::cout << "Performing cross validation, fold = " << nFold << std::endl;
			std::cout << "degreeGrid: min, max, l-step = " << degreeGrid.min_val << ", "
															<< degreeGrid.max_val << ", "
															<< degreeGrid.step << std::endl;
			std::cout << "coef0Grid: min, max, l-step = " << coef0Grid.min_val << ", "
															<< coef0Grid.max_val << ", "
															<< coef0Grid.step << std::endl;
			std::cout << "gammaGrid: min, max, l-step = " << gammaGrid.min_val << ", "
															<< gammaGrid.max_val << ", "
															<< gammaGrid.step << std::endl;
			std::cout << "cGrid: min, max, l-step = " << cGrid.min_val << ", "
															<< cGrid.max_val << ", "
															<< cGrid.step << std::endl;

			// get parameters:
			flag = mSVM.train_auto(train_data, responses, var_idx, sample_idx, params,
					nFold,
					cGrid,
					gammaGrid,
					CvSVM::get_default_grid(CvSVM::P),
					CvSVM::get_default_grid(CvSVM::NU),
					coef0Grid,
					degreeGrid);
		}

		if (!flag) {
			throw Exception("Training error in OpenCV funtion train!");
		}
		updateTrainedModelInfo();
	}
	catch (Exception e) {
		 std::cerr << "An exception has occured in ClassificationWidget::on_trainPushButton_clicked: " << e.what() << std::endl;
		 QMessageBox::critical(this,"An error has occured while performing training", e.what() );
	}
	catch (cv::Exception e) {
		 std::cerr << "An exception has occured in ClassificationWidget::on_saveTrainingSetFromFilePushButton_clicked: " << e.what() << std::endl;
		QMessageBox::critical(this,"An opencv-error has occured while saving training set", e.what() );
	}

	return;
}

void ClassificationWidget::on_loadTrainingSetFromFilePushButton_clicked()
{
	try {
//		QString fn = QFileDialog::getSaveFileName ( this, "Specify filename", QDir::currentPath(), ".xml" );
		QString fn = QFileDialog::getOpenFileName(	this,
													tr("Load traing set from XML file"),
													QDir::currentPath(),
													tr("Training set XML files (*.xml)"));
		if (fn.isEmpty()) return;
		QFile file(fn);
		if (!file.open(QFile::ReadOnly | QFile::Text)) {
			throw Exception("Error openening xml-file while loading svm training set!");
		}
		mSVM.clear();
		mSVM.load(fn.toStdString().c_str(), NULL);
	}
	catch (Exception e) {
		std::cerr << "An exception has occured in ClassificationWidget::on_saveTrainingSetFromFilePushButton_clicked: " << e.what() << std::endl;
		QMessageBox::critical(this,"An error has occured while saving training set", e.what() );
	}
	catch (cv::Exception e) {
		 std::cerr << "An exception has occured in ClassificationWidget::on_saveTrainingSetFromFilePushButton_clicked: " << e.what() << std::endl;
		QMessageBox::critical(this,"An opencv-error has occured while saving training set", e.what() );
	}

	updateTrainedModelInfo();
	return;
}

void ClassificationWidget::on_saveTrainingSetFromFilePushButton_clicked()
{
//	std::cout << "save training svm model to xml file: to implement..." << std::endl;
	try {
		if (mSVM.get_support_vector_count()==0) {
			throw NoDataException("No training set available to save!");
		}
		QString fn = QFileDialog::getSaveFileName ( this, "Specify training set XML result filename", QDir::currentPath(), ".xml" );
		if (fn.isEmpty()) return;
		if (!fn.endsWith(".xml")) { fn.append(".xml"); }
		mSVM.save(fn.toStdString().c_str(), NULL);
	}
	catch (Exception e) {
		std::cerr << "An exception has occured in ClassificationWidget::on_saveTrainingSetFromFilePushButton_clicked: " << e.what() << std::endl;
		QMessageBox::critical(this,"An error has occured while saving training set", e.what() );
	}
	catch (cv::Exception e) {
		 std::cerr << "An exception has occured in ClassificationWidget::on_saveTrainingSetFromFilePushButton_clicked: " << e.what() << std::endl;
		QMessageBox::critical(this,"An opencv-error has occured while saving training set", e.what() );
	}

	updateTrainedModelInfo();
	return;
}

void ClassificationWidget::updateTrainedModelInfo()
{
	QString info;
	if (!isModelLoaded()) {
		ui.loadedTrainingSetInfoLabel->setText("No model currently loaded!");
		return;
	}

	info += "Currently Loaded model:\n";
	info += "\tNr. of support vectors = "+QString::number(mSVM.get_support_vector_count()) + "\n";
	info += "\tDimension of dataset = "+QString::number(mSVM.get_var_count()) + "\n";
	info += getKernelDependentParamString();

	ui.loadedTrainingSetInfoLabel->setText(info);
	ui.loadedTrainingSetInfoLabel->resize(ui.loadedTrainingSetInfoLabel->sizeHint());

	return;
}

QString ClassificationWidget::getKernelDependentParamString()
{
	QString paramStr;
	CvSVMParams params = mSVM.get_params();
	if (params.kernel_type == CvSVM::LINEAR) {
		paramStr += "\tLinear Kernel\n";
	}
	else if (params.kernel_type == CvSVM::POLY) {
		paramStr += "\tPolynomial Kernel\n";
		paramStr += "\tDegree = "+QString::number(params.degree)+"\n";
		paramStr += "\tGamma = "+QString::number(params.gamma)+"\n";
		paramStr += "\tCoef0 = "+QString::number(params.coef0)+"\n";
	}
	else if (params.kernel_type == CvSVM::RBF) {
		paramStr += "\tRadial Basis Function (RBF) Kernel\n";
		paramStr += "\tGamma = "+QString::number(params.gamma)+"\n";
	}
	else if (params.kernel_type == CvSVM::SIGMOID) {
		paramStr += "\tSigmoid Kernel\n";
		paramStr += "\tGamma = "+QString::number(params.gamma)+"\n";
		paramStr += "\tCoef0 = "+QString::number(params.coef0)+"\n";
	}
	else {
		throw Exception("Unknown kernel type!");
	}
	// add string for parameter C, which is used in every kernel:
	paramStr += "\tCost parameter C = "+QString::number(params.C)+"\n";

	return paramStr;
}

CvSVMParams ClassificationWidget::getTrainingParameters() const
{
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC; // only multi-class case here
	params.kernel_type = ui.kernelComboBox->currentIndex();
	params.degree = ui.degreeDoubleSpinBox->value();
	params.gamma = ui.gammaDoubleSpinBox->value();
	params.coef0 = ui.coef0DoubleSpinBox->value();
	params.C = ui.CDoubleSpinBox->value();

	params.nu = 0; // not used
	params.p = 0; // not used
	params.class_weights = NULL; // not used (maybe I should...?)
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1e6, 1e-6); // defaults ???

	return params;
}

void ClassificationWidget::updateCrossValidationInfo()
{
	ui.crossValidationInfoLabel->setText("");
}

} // end of namespace impact_tr4
