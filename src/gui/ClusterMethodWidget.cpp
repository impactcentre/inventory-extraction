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
#include "ClusterMethodWidget.hpp"

#include <QWidget>
#include <QString>
#include <QMessageBox>

#include "clustering/DataStructs.hpp"

namespace impact_tr4
{

ClusterMethodWidget::ClusterMethodWidget(QWidget *parent /*= 0*/, Qt::WFlags f /*= Qt::Widget*/)
: QWidget(parent, f)
{
	// setup ui of corresponging ui-file-variable
	// NOTE: setupUi also connects all signals of all child objects of the widget with the slots named by the on_<object-name>_<signal-name>(...) pattern!
	_ui.setupUi(this);

	// set string-list for all available cluster methods:
//	mAllClusterMethodsList << "K-Means" << "Leader-Follower (Default)" << "Agglomerative" << "DBSCAN (experimental)" << "CLARANS (experimental)";
//	mDistanceBasedClusterMethodsList << "Leader-Follower (Default)" << "Agglomerative" << "DBSCAN (experimental)" << "CLARANS (experimental)";
	mAllClusterMethodsList << "K-Means" << "Leader-Follower (Default)" << "Agglomerative" << "DBSCAN (experimental)";
	mDistanceBasedClusterMethodsList << "Leader-Follower (Default)" << "Agglomerative" << "DBSCAN (experimental)";

	// set string-list for distance based cluster methods:
	on_distanceComputationComboBox_currentIndexChanged(_ui.distanceComputationComboBox->currentIndex());

	setClusterMethodParsVisibility( clusterMethodType() );
//	_ui.dimReductionGroupBox->setVisible(false);

//	_ui.clusterMethodComboBox->setCurrentIndex ( 1 );
}

ClusterMethodWidget::~ClusterMethodWidget()
{

}

void ClusterMethodWidget::on_distanceComputationComboBox_currentIndexChanged(int index)
{
	if (index == 0) {
		disconnect(_ui.clusterMethodComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_clusterMethodComboBox_currentIndexChanged(const QString &)));
		_ui.clusterMethodComboBox->clear();
		_ui.clusterMethodComboBox->addItems(mDistanceBasedClusterMethodsList);
		setClusterMethodParsVisibility( clusterMethodType() );
		connect(_ui.clusterMethodComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_clusterMethodComboBox_currentIndexChanged(const QString &)));
	}
	else if (index == 1) {
		disconnect(_ui.clusterMethodComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_clusterMethodComboBox_currentIndexChanged(const QString &)));
		_ui.clusterMethodComboBox->clear();
		_ui.clusterMethodComboBox->addItems(mAllClusterMethodsList);
		setClusterMethodParsVisibility( clusterMethodType() );
		connect(_ui.clusterMethodComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_clusterMethodComboBox_currentIndexChanged(const QString &)));
	}
	else {
		throw Exception("Unknown cluster distance computation type in ClusterMethodWidget::setAvailableClusterMethods()");
	}

	return;
}

ClusterMethodType ClusterMethodWidget::clusterMethodType() const
{

	QString text = _ui.clusterMethodComboBox->currentText();
//	std::cout << text.toStdString() << std::endl;
	return impact_tr4::clusterMethodType(text.toStdString());
}

ClusteringDataType ClusterMethodWidget::clusterDataType() const
{
	if (_ui.distanceComputationComboBox->currentIndex() == 0) {
		return DISTANCE_BASED;
	}
	else if (_ui.distanceComputationComboBox->currentIndex() == 1) {
		return FEATURES_BASED;
	}
	else {
		return UNKNOWN_CLUSTER_DISTANCE_TYPE;
	}
}

ClusterDistanceType ClusterMethodWidget::clusterDistanceType() const
{
	QString text = _ui.aggloClusterDistComboBox->currentText();
	return impact_tr4::clusterDistanceType(text.toStdString());
}

void ClusterMethodWidget::on_clusterMethodComboBox_currentIndexChanged ( const QString & text )
{
//	std::cout<< "current index changed, selected item = " << text.toStdString() << std::endl;
	ClusterMethodType clusterMethod = clusterMethodType();
	setClusterMethodParsVisibility(clusterMethod);

	return;
} // end on_clusterMethodComboBox_currentIndexChanged(...)

void ClusterMethodWidget::setClusterMethodParsVisibility(const ClusterMethodType& clusterMethod)
{
	// initially, set all group boxes non-visible:
	_ui.kMeansParametersGroupBox->setVisible(false);
	_ui.leaderFollowerParametersGroupBox->setVisible(false);
	_ui.agglomerativeParametersGroupBox->setVisible(false);
	_ui.dbscanParametersGroupBox->setVisible(false);
	_ui.claransParametersGroupBox->setVisible(false);
	// parse method and extract parameters
	switch (clusterMethod) {
		case KMEANS:
			_ui.kMeansParametersGroupBox->setVisible(true);
			break;
		case LEADER_FOLLOWER:
			_ui.leaderFollowerParametersGroupBox->setVisible(true);
			break;
		case AGGLOMERATIVE:
			_ui.agglomerativeParametersGroupBox->setVisible(true);
			break;
		case DBSCAN:
			_ui.dbscanParametersGroupBox->setVisible(true);
			break;
		case CLARANS:
			_ui.claransParametersGroupBox->setVisible(true);
			break;
		default:
			throw Exception("Unknown type at ClusterMethodWidget::setClusterMethodParsVisibility!");
			break;
	} // end switch

	return;
}

ClusterMethodParameters* ClusterMethodWidget::parameters() const
{
	ClusterMethodParameters *pParameters=0x0;
	ClusterMethodType clusterMethod = clusterMethodType();
	// parse method and extract parameters
	switch (clusterMethod) {
		case KMEANS:
			pParameters = new KMeansParameters();
			pParameters->dataType = clusterDataType();
			((KMeansParameters*)pParameters)->nClusters = _ui.kmeansNClusterSpinBox->value();
			((KMeansParameters*)pParameters)->maxIts = _ui.kmeansMaxItsSpinBox->value();
			((KMeansParameters*)pParameters)->eps = _ui.kmeansEpsSpinBox->value();
			break;
		case LEADER_FOLLOWER:
			pParameters = new LeaderFollowerParameters();
			pParameters->dataType = clusterDataType();
			((LeaderFollowerParameters*)pParameters)->threshold = _ui.lfThresholdSpinBox->value();
			((LeaderFollowerParameters*)pParameters)->stopOnFirstCluster = _ui.stopOnFirstClusterCheckBox->isChecked();
			((LeaderFollowerParameters*)pParameters)->reassignThreshold = _ui.lfReassignThresholdSpinBox->value();
			((LeaderFollowerParameters*)pParameters)->distToCenterThreshold = _ui.lfDistanceToCenterThresholdSpinBox->value();
			((LeaderFollowerParameters*)pParameters)->clusterSizeThreshold = _ui.lfClusterSizeThresholdSpinBox->value();
			break;
		case AGGLOMERATIVE:
			pParameters = new AgglomerativeParameters();
			pParameters->dataType = clusterDataType();
			((AgglomerativeParameters*)pParameters)->nClusters = _ui.agglomerativeNClusterSpinBox->value();
			((AgglomerativeParameters*)pParameters)->clusterDistType = clusterDistanceType();
			break;
		case DBSCAN:
			pParameters = new DBSCANParameters();
			pParameters->dataType = clusterDataType();
			((DBSCANParameters*)pParameters)->minPts = _ui.minPtsSpinBox->value();
			((DBSCANParameters*)pParameters)->eps = _ui.epsSpinBox->value();
			break;
		case CLARANS:
			pParameters = new CLARANSParameters();
			pParameters->dataType = clusterDataType();
			((CLARANSParameters*)pParameters)->numClust = _ui.claransNClusterSpinBox->value();
			((CLARANSParameters*)pParameters)->numLocal = _ui.numlocalSpinBox->value();
			((CLARANSParameters*)pParameters)->maxNeighbor = _ui.maxneighborSpinBox->value();
			break;
		default:
			throw Exception("Unknown type at ClusterMethodWidget::parameters()!");
			break;
	} // end switch
	return pParameters;
}

} // end namespace impact_tr4
