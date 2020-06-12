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
#include "FeatureVectorWidget.hpp"

#include "clustering/CharFeatureCollection.hpp"
#include "clustering/features/ImageCharFeatureI.hpp"
#include "clustering/features/ImageIntegralInvariantFeature.hpp"
#include "clustering/features/AMIFeature.hpp"
#include "clustering/features/HuMomentsFeature.hpp"
#include "clustering/features/BasicMomentsFeature.hpp"
#include "clustering/features/ProfilesFeatures.hpp"
#include "clustering/DataStructs.hpp"

namespace impact_tr4
{

FeatureVectorWidget::FeatureVectorWidget(CharFeatureCollection *pCharFeatureCollection, QWidget *parent /*= 0*/, Qt::WFlags f /*= Qt::Widget*/)
: QWidget(parent, f), mpCharFeatureCollection(pCharFeatureCollection)
{
	// setup ui of corresponging ui-file-variable
	// NOTE: setupUi also connects all signals of all child objects of the widget with the slots named by the on_<object-name>_<signal-name>(...) pattern!
	_ui.setupUi(this);

	// set visibility of feature vector boxes:
	_ui.integralInvariantGroupBox->setVisible(false);

	// set outer profiles as default:
	this->setDefaultFeatures();

	return;
}

FeatureVectorWidget::~FeatureVectorWidget()
{

}

void FeatureVectorWidget::setDefaultFeatures()
{
	this->on_removeAllButton_clicked(); // remove all preset features first
	this->setDefaultProfileParameters(); // set default values for profile features
	this->on_addProfilesButton_clicked(); // add it to feature vector
	return;
}

void FeatureVectorWidget::on_addIntegralInvariantButton_clicked()
{
	try {
		QString text;
		// parse parameters:
		const int radius = _ui.radiusSpinBox->value();
		const bool oddSize = _ui.oddKernelSizeCheckBox->isChecked();
		QString typeStr = _ui.integralInvariantTypeComboBox->currentText();
		Kernel<> mask;
		if (!typeStr.compare("Circle-Area")) {
		    Kernel<>::createSphereKernel( mask, radius, oddSize ); // TEST
		    text = "Integral Invariant, Type=Circle-Area, r=" + QString::number(radius) + ", oddSize=" + QString::number(oddSize);
		}
		else if (!typeStr.compare("Circle")) {
			std::cout << "creating circle invariant kernel" << std::endl;
		    Kernel<>::createCircleKernel( mask, radius, oddSize ); // TEST
		    text = "Integral Invariant, Type=Circle, r=" + QString::number(radius) + ", oddSize=" + QString::number(oddSize);
		}
		else if (!typeStr.compare("Rotation")) {
			std::cout << "Invariant not implemented yet!" << std::endl;
			return;
		}
		else {
			throw Exception("Wrong type specified in FeatureVectorWidget::on_addIntegralInvariantButton_clicked()");
		}
		// determine historgram type:
		int histogramType = 0;
		if (_ui.binsRadioButton->isChecked()) histogramType = 0;
		else if (_ui.normalizedBinsRadioButton->isChecked()) histogramType = 1;
		else if (_ui.cumulativeBinsRadioButton->isChecked()) histogramType = 2;
		// create feature:
		ImageIntegralInvariantFeature* pFeature = new ImageIntegralInvariantFeature(mask, histogramType);
		// add to list:
		this->addFeature(pFeature, text);
	} // end try
	catch (Exception e) {
		QMessageBox::critical ( this, "Error creating integral invariant feature", e.what() );
	}

	return;
}

void FeatureVectorWidget::on_addMomentButton_clicked()
{
	QString text;
	ImageCharFeatureI *pFeature = 0x0;
	if (_ui.basicMomentsRadioButton->isChecked()) {
		text = "Basic moments";
		pFeature = new BasicMomentsFeature();
	}
	else if (_ui.amiMomentsRadioButton->isChecked()) {
		text = "AMI moments";
		pFeature = new AMIFeature();
	}
	else if (_ui.huMomentsRadioButton->isChecked()) {
		text = "HU moments";
		pFeature = new HuMomentsFeature();
	}

	this->addFeature(pFeature, text);

	return;
}

void FeatureVectorWidget::on_addProfilesButton_clicked()
{
	QString text;

	const int nHor = _ui.nHorizontalBlocksSpinBox->value();
	const int nVer = _ui.nVerticalBlocksSpinBox->value();
	const bool useHorOuter = _ui.horizontalUseOuterCheckBox->isChecked();
	const bool useHorInner = _ui.horizontalUseInnerCheckBox->isChecked();
	const bool useVerOuter = _ui.verticalUseOuterCheckBox->isChecked();
	const bool useVerInner = _ui.verticalUseInnerCheckBox->isChecked();

	std::cout << "Adding profile feature, pars: nHor = " << nHor << ", nVer = " << nVer << std::endl;
	std::cout << "useHorOuter = " << useHorOuter << ", useHorInner = " << useHorInner << std::endl;
	std::cout << "useVerOuter = " << useVerOuter << ", useVerInner = " << useVerInner << std::endl;

	// Create the feature:
	ImageCharFeatureI *pFeature = new ProfilesFeatures(nHor, nVer, useHorOuter, useVerOuter, useHorInner, useVerInner);

	// compute dimension of feature vector:
	const int dim = pFeature->vectorSize();
	std::cout << "Resulting feature vector has size " << dim << std::endl;

	text = "Profile Feature";
	this->addFeature(pFeature, text);

	return;
} // on_addProfilesButton_clicked()

void FeatureVectorWidget::setDefaultProfileParameters()
{
	_ui.nHorizontalBlocksSpinBox->setValue(10);
	_ui.nVerticalBlocksSpinBox->setValue(10);
	_ui.horizontalUseOuterCheckBox->setChecked(true);
	_ui.verticalUseOuterCheckBox->setChecked(true);
	_ui.horizontalUseInnerCheckBox->setChecked(false);
	_ui.verticalUseInnerCheckBox->setChecked(false);

	return;
}

void FeatureVectorWidget::on_addOtherFeatureButton_clicked()
{
#if 0
	QString text;
//	ImageCharFeatureI *pFeature = 0x0;

	if (_ui.densityCheckBox->isChecked()) {
//		ImageCharFeatureI *pFeature = ... // TODO
	}
	if (_ui.regionalDensityCheckBox->isChecked()) {
		int nWidth = _ui.regionalDensityWidthSpinBox->value();
		int nHeight = _ui.regionalDensityHeightSpinBox->value();
		//		ImageCharFeatureI *pFeature = ... // TODO
	}

	this->addFeature(pFeature, text);

#endif
	return;
}

void FeatureVectorWidget::addFeature(ImageCharFeatureI* pFeature, QString text)
{
	text += " (dim = " + QString::number(pFeature->vectorSize()) + ")";
	QListWidgetItem *newItem = new QListWidgetItem( text );
	newItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled );
	_ui.featureVectorListWidget->addItem( newItem );
	mpCharFeatureCollection->addFeature(pFeature);

	return;
} // end addFeature(...)

void FeatureVectorWidget::on_removeSelectedButton_clicked()
{
	QList<QListWidgetItem *> selectedItemsList = _ui.featureVectorListWidget->selectedItems();
	std::cout << selectedItemsList.size() << std::endl;
	for (int i=0; i<selectedItemsList.size(); ++i){
		int rowI = _ui.featureVectorListWidget->row ( selectedItemsList.at(i) );
		delete _ui.featureVectorListWidget->takeItem ( rowI );
		mpCharFeatureCollection->deleteFeature(rowI);
	}

	return;
}

void FeatureVectorWidget::on_removeAllButton_clicked()
{
	_ui.featureVectorListWidget->selectAll();
	on_removeSelectedButton_clicked();
	return;
}

QString FeatureVectorWidget::constructIntegralInvariantText()
{
	QString text;

	text += "II, ";
	QString type = _ui.integralInvariantTypeComboBox->currentText();
	text += type;
	if (!type.compare("Circle-Area")) {
		text += ", Radius=" + QString::number(_ui.radiusSpinBox->value());
		text += ", OddKernelSize=";
		text += (_ui.oddKernelSizeCheckBox->isChecked())?"true":"false";
		text += ", Weighting=";
		text += QString::number(1);
	}

	return text;
}

const PCAReductionParams FeatureVectorWidget::dimReductionParameters() const
{
	PCAReductionParams params;
	params.threshold = -1;
	params.newDim = -1;
	if (_ui.dimReductionGroupBox->isChecked()) {
		if (_ui.pcaThresholdRadioButton->isChecked()) {
			params.threshold = _ui.dimReductionThresholdSpinBox->value();
		}
		else if (_ui.pcaNewDimRadioButton->isChecked()) {
			params.newDim = _ui.dimReductionNewDimSpinBox->value();
		}
	}
	return params;
}

} // end of namespace impact_tr4
