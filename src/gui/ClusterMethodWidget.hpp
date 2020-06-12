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
#ifndef CLUSTERMETHODWIDGET_HPP_
#define CLUSTERMETHODWIDGET_HPP_

#include "ui_ClusterMethodWidget.h"

#include <iostream>
#include <QMainWindow>
#include <QStringList>

#include "clustering/DataStructs.hpp"

namespace impact_tr4
{

class ClusterMethodWidget : public QWidget
{
Q_OBJECT
public:
	// CONSTRUCTORS:
	ClusterMethodWidget(QWidget *parent = 0, Qt::WFlags f = Qt::Widget);
	~ClusterMethodWidget();

public:
	/// Returns the parameters  of this clustering method
	ClusterMethodParameters* parameters() const;
	ClusterMethodType clusterMethodType() const;
	ClusteringDataType clusterDataType() const;
	ClusterDistanceType clusterDistanceType() const;

public slots:
	void on_clusterMethodComboBox_currentIndexChanged ( const QString & text );
	void on_distanceComputationComboBox_currentIndexChanged ( int index );
	void setClusterMethodParsVisibility(const ClusterMethodType& clusterMethod);
signals:

private:
	// PRIVATE MEMBERS:
	Ui::ClusterMethodWidget _ui;
	QStringList mDistanceBasedClusterMethodsList;
	QStringList mAllClusterMethodsList;

}; // end class

} // end of namespace impact_tr4

#endif /*CLUSTERMETHODWIDGET_HPP_*/
