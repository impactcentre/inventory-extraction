/*
 * ClusteringResultPrototypeViewWidget.hpp
 *
 *  Created on: 20.07.2010
 *      
 */

#ifndef CLUSTERINGRESULTPROTOTYPEVIEWWIDGET_HPP_
#define CLUSTERINGRESULTPROTOTYPEVIEWWIDGET_HPP_

#include <QtGui>

#include "ClusterViewWidget.hpp"
#include "PrototypeViewListWidget.hpp"
#include "ClusterSummaryWidget.hpp"
#include "clustering/ClusteringResult.hpp"
#include "clustering/Document.hpp"

namespace impact_tr4 {

class ClusteringResultPrototypeViewWidget : public QWidget {
	Q_OBJECT
public:
	ClusteringResultPrototypeViewWidget( QWidget *parent = 0 );
	~ClusteringResultPrototypeViewWidget() {};

	int findClusterSummaryWidgetIndex(CharClustIt clustIt);

public slots:
	// OPENING / CLOSING CLUSTERINGS SLOTS:
	void openClusteringResult(ClusteringResult *pClusteringResult, Document *pDocument);
	void clearClusteringResult();
	bool hasClusteringResult() const { return (mpClusteringResult != 0x0); }
	ClusteringResult* getClusteringResult();
	void showClusterViewWidget(CharClustIt clustIt);
	void showTrashCluster();

	// EDITING FUNCTION SLOTS:
	void labelCurrentCluster();
	void moveToCluster(CharClustIt from, CharClustIt to, std::vector<ImageCharIt>& iterators);
	void moveToTrashCluster(CharClustIt from, std::vector<ImageCharIt>& iterators);

	void mergeClusters(std::vector<CharClustIt> from, CharClustIt to);
	void mergeSelectedClusters();
	void removeSelectedClusters();
	void removeCluster(CharClustIt i);
	void addEmptyCluster();
	void removeEmptyClusters();
	void reClustering();

	// VIEWING FUNCTION SLOTS:
	void showUnsorted();
	void sortByNearestNeighbor();
	void sortBySizeDescending();
	void sortBySizeAscending();
	void reloadViews();

	void restoreBackupClustering();

	// OTHER SLOTS:
	ClusterSummaryWidget* addClusterSummaryWidget(int number, CharClustIt clustIt);

private slots:
	void mergeWithCluster(CharClustIt from, CharClustIt to);

signals:
	void refreshInfoGroupBox();


private:
	ClusteringResult *mpClusteringResult;
	ClusteringResult mClusteringResultBackup;

	Document *mpDocument;

	ClusteringResult mDummyClustering;
	Document mDummyDocument;

	// Label buttons:
	QPushButton *mpLabelButton;
	QLineEdit *mpLabelLineEdit;

	// Edit buttons, sliders etc:
	QPushButton *mpMergeSelectedClustersButton, *mpAddEmptyClusterButton,
				*mpRemoveEmptyClustersButton, *mpRemoveSelectedClustersButton;

	QPushButton *mpReclusterHierarchicalButton;
	QSpinBox *mpReclusterHierarchicalSpinBox;

	// view buttons etc:
	QPushButton *mpShowUnsortedButton, *mpSortByNearestNeighborButton,
				*mpSortBySizeDescendingButton, *mpSortBySizeAscendingButton;
	QPushButton *mpReloadViewsButton;
	QSpinBox *mpIconSizeSpinBox;
	QSlider *mpIconSizeSlider;
	QSpinBox *mpSizeThreshLowSpinBox;
	QSpinBox *mpSizeThreshHighSpinBox;
	QCheckBox *mpSizeCheckBox;

//	ClusterViewWidget *mpCurrentClusterViewWidget;

	// Prototype view list widget:
	PrototypeViewListWidget *mpPrototypeViewListWidget;

	// Current cluster view widget:
	ClusterViewWidget *mpClusterViewWidget;

	// Cluster summary list widgets:
	QList<ClusterSummaryWidget*> mClusterSummaryList;
	QWidget *mpInternalClusterSummaryWidget;
	QScrollArea *scrollareaClusterSummary;
	QVBoxLayout *mpClusterSummaryLayout;

	// Global layout:
	QVBoxLayout *mpGlobalLayout;
};

} // end of namespace impact_tr4

#endif /* CLUSTERINGRESULTPROTOTYPEVIEWWIDGET_HPP_ */
