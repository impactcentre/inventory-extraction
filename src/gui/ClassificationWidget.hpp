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
#ifndef CLASSIFICATIONWIDGET_H
#define CLASSIFICATIONWIDGET_H

#include <QtGui/QWidget>
#include "ui_ClassificationWidget.h"

#include "core/OpenCV.hpp"
#include "clustering/ClusteringController.hpp"

namespace impact_tr4 {

class ClassificationWidget : public QWidget
{
    Q_OBJECT

public:
    ClassificationWidget(ClusteringController *pClusteringController, QWidget *parent = 0);
    ~ClassificationWidget();

    CvSVMParams getTrainingParameters() const;

    bool isModelLoaded() const { return (mSVM.get_support_vector_count()!=0); }
    void clearModel() { mSVM.clear(); }

public slots:
	void on_trainPushButton_clicked();
	void on_loadTrainingSetFromFilePushButton_clicked();
	void on_saveTrainingSetFromFilePushButton_clicked();
	void on_kernelComboBox_currentIndexChanged(int i);
	void on_setDefaultGridsPushButton_clicked();

signals:
	void recomputeFeatures();

private:
	void loadTrainingDataFromClusteringResult(const bool doRandomPermutation=true);
	void updateTrainedModelInfo();
	void updateCrossValidationInfo();
	QString getKernelDependentParamString();
	CvParamGrid getDegreeGrid() const;
	CvParamGrid getCoef0Grid() const;
	CvParamGrid getGammaGrid() const;
	CvParamGrid getCGrid() const;

    Ui::ClassificationWidgetClass ui;
	ClusteringController *mpClusteringController;

    ublas::matrix<float> mSVMDataMatrix;
    ublas::vector<int> mLabelVector;

    CvSVM mSVM;
};

} // end of namespace impact_tr4

#endif // CLASSIFICATIONWIDGET_H
