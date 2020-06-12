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
#ifndef FEATUREVECTORWIDGET_HPP_
#define FEATUREVECTORWIDGET_HPP_

#include <iostream>
#include <QWidget>
#include <QMessageBox>

#include "ui_FeatureVectorWidget.h"

#include "core_headers.h"
#include "image_headers.h"

namespace impact_tr4
{

class CharFeatureCollection;
class ImageCharFeatureI;
struct PCAReductionParams;

class FeatureVectorWidget : public QWidget
{
	Q_OBJECT
public:
	FeatureVectorWidget(CharFeatureCollection *pCharFeatureCollection, QWidget *parent = 0, Qt::WFlags f = Qt::Widget);
	virtual ~FeatureVectorWidget();

	const PCAReductionParams dimReductionParameters() const;

public slots:
	void on_addIntegralInvariantButton_clicked();
	void on_removeSelectedButton_clicked();
	void on_removeAllButton_clicked();
	void on_addMomentButton_clicked();
	void on_addProfilesButton_clicked();
	void on_addOtherFeatureButton_clicked();

signals:

private:
	void addFeature(ImageCharFeatureI* pFeature, QString text);
	void setDefaultFeatures();
	void setDefaultProfileParameters();

	QString constructIntegralInvariantText();
	Ui::FeatureVectorWidget _ui;
	CharFeatureCollection *mpCharFeatureCollection;
};

} // end of namespace impact_tr4

#endif /*FEATUREVECTORWIDGET_HPP_*/
