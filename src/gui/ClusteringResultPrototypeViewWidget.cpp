/*
 * ClusteringResultPrototypeViewWidget.cpp
 *
 *  Created on: 20.07.2010
 *      
 */

#include "ClusteringResultPrototypeViewWidget.hpp"

#include "ext/cluster/cluster.h"

namespace impact_tr4 {

ClusteringResultPrototypeViewWidget::ClusteringResultPrototypeViewWidget( QWidget *parent /*= 0*/ )
: QWidget(parent)
{
	mpClusteringResult = 0x0;
	mpDocument = 0x0;
	// create global layout:
	mpGlobalLayout = new QVBoxLayout();

	// add top Label for cluster prototype widget:
	QLabel *label = new QLabel("Cluster Prototypes");
	QFont labelFont("Arial", 12, QFont::Bold);
	label->setFont(labelFont);
	mpGlobalLayout->addWidget(label);

	// add prototype list widget:
	mpPrototypeViewListWidget = new PrototypeViewListWidget(0x0, this);
	connect(mpPrototypeViewListWidget, SIGNAL(showClusterViewWidget(CharClustIt)), this, SLOT(showClusterViewWidget(CharClustIt)));
	connect(mpPrototypeViewListWidget, SIGNAL(mergeWithCluster(CharClustIt, CharClustIt)), this, SLOT(mergeWithCluster(CharClustIt, CharClustIt)));
	connect(mpPrototypeViewListWidget, SIGNAL(mergeClusters(std::vector<CharClustIt>, CharClustIt)), this, SLOT(mergeClusters(std::vector<CharClustIt>, CharClustIt)));
	connect(mpPrototypeViewListWidget, SIGNAL(moveToCluster(CharClustIt, CharClustIt, std::vector<ImageCharIt>&)), this, SLOT(moveToCluster(CharClustIt, CharClustIt,std::vector<ImageCharIt>&)));
//	mpGlobalLayout->addWidget(mpPrototypeViewListWidget);

	// add cluster summary widget:
	mpClusterSummaryLayout = new QVBoxLayout();
	scrollareaClusterSummary = new QScrollArea(this);
	mpInternalClusterSummaryWidget = new QWidget;
	mpInternalClusterSummaryWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	mpInternalClusterSummaryWidget->setLayout(mpClusterSummaryLayout);
	scrollareaClusterSummary->setWidget(mpInternalClusterSummaryWidget);
	scrollareaClusterSummary->setMaximumWidth(420);

	QHBoxLayout *pTopLayout = new QHBoxLayout();
	pTopLayout->addWidget(mpPrototypeViewListWidget);
	pTopLayout->addWidget(scrollareaClusterSummary);
	scrollareaClusterSummary->setVisible(false);
	mpGlobalLayout->addLayout(pTopLayout);

	// add buttons:
	mpMergeSelectedClustersButton = new QPushButton("Merge selected clusters");
	mpMergeSelectedClustersButton->setFixedSize(mpMergeSelectedClustersButton->sizeHint());
	connect(mpMergeSelectedClustersButton, SIGNAL(clicked()), this, SLOT(mergeSelectedClusters()));

	mpRemoveSelectedClustersButton = new QPushButton("Remove selected clusters");
	mpRemoveSelectedClustersButton->setFixedSize(mpRemoveSelectedClustersButton->sizeHint());
	connect(mpRemoveSelectedClustersButton, SIGNAL(clicked()), this, SLOT(removeSelectedClusters()));

	mpAddEmptyClusterButton = new QPushButton("Add empty cluster");
	mpAddEmptyClusterButton->setFixedSize(mpAddEmptyClusterButton->sizeHint());
	connect(mpAddEmptyClusterButton, SIGNAL(clicked()), this, SLOT(addEmptyCluster()));

	mpRemoveEmptyClustersButton = new QPushButton("Remove empty clusters");
	mpRemoveEmptyClustersButton->setFixedSize(mpRemoveEmptyClustersButton->sizeHint());
	connect(mpRemoveEmptyClustersButton, SIGNAL(clicked()), this, SLOT(removeEmptyClusters()));

	QPushButton *pShowTrashButton = new QPushButton("Show trash cluster");
	pShowTrashButton->setFixedSize(pShowTrashButton->sizeHint());
	connect(pShowTrashButton, SIGNAL(clicked()), this, SLOT(showTrashCluster()));

	mpShowUnsortedButton = new QPushButton("Show clusters unsorted");
	mpShowUnsortedButton->setFixedSize(mpShowUnsortedButton->sizeHint());
	connect(mpShowUnsortedButton, SIGNAL(clicked()), this, SLOT(showUnsorted()));

	mpSortByNearestNeighborButton = new QPushButton("Sort clusters by nearest neighbor on selected item");
	mpSortByNearestNeighborButton->setFixedSize(mpSortByNearestNeighborButton->sizeHint());
	connect(mpSortByNearestNeighborButton, SIGNAL(clicked()), this, SLOT(sortByNearestNeighbor()));

	mpSortBySizeDescendingButton = new QPushButton("Sort clusters by their size (descending)");
	mpSortBySizeDescendingButton->setFixedSize(mpSortBySizeDescendingButton->sizeHint());
	connect(mpSortBySizeDescendingButton, SIGNAL(clicked()), this, SLOT(sortBySizeDescending()));

	mpSortBySizeAscendingButton = new QPushButton("Sort clusters by their size (ascending)");
	mpSortBySizeAscendingButton->setFixedSize(mpSortBySizeAscendingButton->sizeHint());
	connect(mpSortBySizeAscendingButton, SIGNAL(clicked()), this, SLOT(sortBySizeAscending()));

	QLabel *pIconSizeLabel = new QLabel("Icon Size: ");
	pIconSizeLabel->setFixedSize(pIconSizeLabel->sizeHint());
	mpIconSizeSpinBox = new QSpinBox();
	mpIconSizeSpinBox->setValue(60);
	mpIconSizeSpinBox->setRange(1, 99);
	connect(mpIconSizeSpinBox, SIGNAL(valueChanged(int)), mpPrototypeViewListWidget, SLOT(updateIconSize(int)));
	mpIconSizeSlider = new QSlider(Qt::Horizontal);
	mpIconSizeSlider->setRange(1, 99);
	mpIconSizeSlider->setValue(60);
	connect(mpIconSizeSlider, SIGNAL(valueChanged(int)), mpPrototypeViewListWidget, SLOT(updateIconSize(int)));
	connect(mpIconSizeSlider, SIGNAL(valueChanged(int)), mpIconSizeSpinBox, SLOT(setValue(int)));
	connect(mpIconSizeSpinBox, SIGNAL(valueChanged(int)), mpIconSizeSlider, SLOT(setValue(int)));

	// Labelling stuff:
	QLabel *pLabelLabel = new QLabel("Unicode Label = ");
	pLabelLabel->setFixedSize(pLabelLabel->sizeHint());
	mpLabelLineEdit = new QLineEdit("0");
	mpLabelLineEdit->setFixedWidth(100);
	mpLabelLineEdit->setToolTip("Specify unicode as 4 digit hexadecimal unicode number");
	QRegExp rx( "[0-9A-Fa-f]{1,4}" ); // regular expression for unicode
	QValidator *validator = new QRegExpValidator( rx, this );
	mpLabelLineEdit->setValidator(validator);
	connect(mpLabelLineEdit, SIGNAL(returnPressed()), this, SLOT(labelCurrentCluster()));
	mpLabelButton = new QPushButton("Label!");
	connect(mpLabelButton, SIGNAL(clicked()), this, SLOT(labelCurrentCluster()));

	// CONFIGURE EDITING WIDGETS GROUPBOX:
	QGroupBox *pEditGroupBox = new QGroupBox("Editing options");

	QHBoxLayout *pEditLayout1 = new QHBoxLayout();
	pEditLayout1->addWidget(pLabelLabel);
	pEditLayout1->addWidget(mpLabelLineEdit);
	pEditLayout1->addWidget(mpLabelButton);
	pEditLayout1->addWidget(mpMergeSelectedClustersButton);
	pEditLayout1->addWidget(mpAddEmptyClusterButton);
	pEditLayout1->addWidget(mpRemoveEmptyClustersButton);
	pEditLayout1->addWidget(mpRemoveSelectedClustersButton);

	QHBoxLayout *pEditLayout2 = new QHBoxLayout();
	QLabel *pReclusterLabel = new QLabel("Re-cluster by prototypes ");
	pReclusterLabel->setFixedSize(pReclusterLabel->sizeHint());

	mpReclusterHierarchicalSpinBox = new QSpinBox();
	mpReclusterHierarchicalSpinBox->setMinimum(1);
	mpReclusterHierarchicalSpinBox->setMaximum(999999);
	mpReclusterHierarchicalSpinBox->setValue(60);
	mpReclusterHierarchicalSpinBox->setPrefix("New nr. of clusters = ");

	mpReclusterHierarchicalButton = new QPushButton("Re-cluster!");
	mpReclusterHierarchicalButton->setToolTip(("Re-clusters the clustering result using the "
			"prototype images and an agglomerative hierarchical clustering scheme with average linking to a specified"
			" nr. of clusters"));
	connect(mpReclusterHierarchicalButton, SIGNAL(clicked()), this, SLOT(reClustering()));

	pEditLayout2->addWidget(pReclusterLabel);
	pEditLayout2->addWidget(mpReclusterHierarchicalSpinBox);
	pEditLayout2->addWidget(mpReclusterHierarchicalButton);
	QPushButton *pRestoreButton = new QPushButton("Restore old clustering");
	pRestoreButton->setToolTip("Restores the clustering result to the state before the last editing operation.");
	connect(pRestoreButton, SIGNAL(clicked()), this, SLOT(restoreBackupClustering()));

	pEditLayout2->addWidget(pRestoreButton);

	QVBoxLayout *pGlobalEditLayout = new QVBoxLayout();
	pGlobalEditLayout->addLayout(pEditLayout1);
	pGlobalEditLayout->addLayout(pEditLayout2);

	pEditGroupBox->setLayout(pGlobalEditLayout);

	// CONFIGURE VIEWING WIDGETS GROUPBOX:
	QGroupBox *pViewGroupBox = new QGroupBox("Viewing options");

	QHBoxLayout *pViewLayout1 = new QHBoxLayout();
	pViewLayout1->addWidget(pIconSizeLabel);
	pViewLayout1->addWidget(mpIconSizeSpinBox);
	pViewLayout1->addWidget(mpIconSizeSlider);

	QHBoxLayout *pViewLayout2 = new QHBoxLayout();
	pViewLayout2->addWidget(pShowTrashButton);
	pViewLayout2->addWidget(mpShowUnsortedButton);
	pViewLayout2->addWidget(mpSortByNearestNeighborButton);
	pViewLayout2->addWidget(mpSortBySizeDescendingButton);
	pViewLayout2->addWidget(mpSortBySizeAscendingButton);

	QHBoxLayout *pViewLayout3 = new QHBoxLayout();
	mpSizeThreshLowSpinBox = new QSpinBox();
	mpSizeThreshLowSpinBox->setRange(0,1e3);
	mpSizeThreshLowSpinBox->setValue(1);
	mpSizeThreshLowSpinBox->setEnabled(false);

	mpSizeThreshHighSpinBox = new QSpinBox();
	mpSizeThreshHighSpinBox->setRange(0,1e3);
	mpSizeThreshHighSpinBox->setValue(10);
	mpSizeThreshHighSpinBox->setEnabled(false);
//	mpSizeThreshSpinBox->setPrefix("Show only clusters with size <= ");
	mpSizeCheckBox = new QCheckBox("Show all");
	mpSizeCheckBox->setChecked(true);
	connect(mpSizeCheckBox, SIGNAL(toggled(bool)), mpSizeThreshLowSpinBox, SLOT(setDisabled(bool)));
	connect(mpSizeCheckBox, SIGNAL(toggled(bool)), mpSizeThreshHighSpinBox, SLOT(setDisabled(bool)));
	pViewLayout3->addWidget(new QLabel("Show only clusters w. size from"));
	pViewLayout3->addWidget(mpSizeThreshLowSpinBox);
	pViewLayout3->addWidget(new QLabel("to"));
	pViewLayout3->addWidget(mpSizeThreshHighSpinBox);
	pViewLayout3->addWidget(mpSizeCheckBox);

	mpReloadViewsButton = new QPushButton("Reload views");
	connect(mpReloadViewsButton, SIGNAL(clicked()), this, SLOT(reloadViews()));
	pViewLayout3->addWidget(mpReloadViewsButton);

	QVBoxLayout *pGlobalViewLayout = new QVBoxLayout();
	pGlobalViewLayout->addLayout(pViewLayout1);
	pGlobalViewLayout->addLayout(pViewLayout2);
	pGlobalViewLayout->addLayout(pViewLayout3);

	pViewGroupBox->setLayout(pGlobalViewLayout);

	// ADD STUFF TO GLOBAL LAYOUT:
	mpGlobalLayout->addWidget(pEditGroupBox);
	mpGlobalLayout->addWidget(pViewGroupBox);

	mDummyClustering.addEmptyCluster(&mDummyDocument);

	// add cluster view widget:
	mpClusterViewWidget = new ClusterViewWidget(mDummyClustering.begin(), this);

//	connect(mpClusterViewWidget, SIGNAL(moveToCluster(CharClustIt, CharClustIt, std::vector<std::list<ImageChar*>::iterator>&)), this, SLOT(moveToCluster(CharClustIt, CharClustIt,std::vector<std::list<ImageChar*>::iterator>&)));
//	connect(mpClusterViewWidget, SIGNAL(mergeWithCluster(CharClustIt, CharClustIt)), this, SLOT(mergeWithCluster(CharClustIt, CharClustIt)));
	connect(mpClusterViewWidget, SIGNAL(removeCluster(CharClustIt)), this, SLOT(removeCluster(CharClustIt)));
	connect(mpClusterViewWidget, SIGNAL(moveToTrashCluster(CharClustIt, std::vector<ImageCharIt>&)), this, SLOT(moveToTrashCluster(CharClustIt, std::vector<ImageCharIt>&)));
	mpGlobalLayout->addWidget(mpClusterViewWidget);

	this->setLayout(mpGlobalLayout);
	this->show();
}

int ClusteringResultPrototypeViewWidget::findClusterSummaryWidgetIndex(CharClustIt clustIt)
{
	for (int i=0; i<mClusterSummaryList.count(); ++i) {
		ClusterSummaryWidget* curr_item = (ClusterSummaryWidget*)(mClusterSummaryList.at(i));

		if (curr_item->clustIt() == clustIt)
			return i;
	}
	return -1;
//	std::cerr << "Item has not been found!" << std::endl;
//	throw Exception("Item has not been found!");
}

void ClusteringResultPrototypeViewWidget::openClusteringResult(ClusteringResult *pClusteringResult, Document *pDocument)
{
	// clear clustering result
	clearClusteringResult(); // necessary?? (...most probably yes!)

	// store and display new clustering result
	mpClusteringResult = pClusteringResult;
	mpDocument = pDocument;

	if (mpClusteringResult->isEmpty()) {
		mpPrototypeViewListWidget->setClusteringResult(&this->mDummyClustering);
		mpClusterViewWidget->setCluster(this->mDummyClustering.begin());
		return;
	}

	std::cout << "opening clustering result, nr of clusters = " << mpClusteringResult->nClusters() << std::endl;

	int i=0;
	for (CharClustIt clust_it = mpClusteringResult->begin(); clust_it != mpClusteringResult->end(); ++clust_it) {
//		std::cout << "adding cluster summary widget " << std::endl;
		addClusterSummaryWidget(i, clust_it);
		++i;
	} // end for all clusters i
	mpPrototypeViewListWidget->setClusteringResult(mpClusteringResult);
	mpClusterViewWidget->setCluster(mpClusteringResult->begin());

	if (pDocument != mpDocument) { // if new document: store backup
		mClusteringResultBackup.deleteClustering();
		mClusteringResultBackup = *mpClusteringResult;
	}

	return;
}

void ClusteringResultPrototypeViewWidget::clearClusteringResult()
{
	while (!mClusterSummaryList.empty()) {
		delete mClusterSummaryList.takeFirst();
//		std::cout << mClusterSummaryList.size() << std::endl;
	}
	// clear items of cluster summary widgets:
	scrollareaClusterSummary->takeWidget();
	delete mpInternalClusterSummaryWidget;
	mpClusterSummaryLayout = new QVBoxLayout();
	mpInternalClusterSummaryWidget = new QWidget;
	mpInternalClusterSummaryWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	mpInternalClusterSummaryWidget->setLayout(mpClusterSummaryLayout);
	scrollareaClusterSummary->setWidget(mpInternalClusterSummaryWidget);


	// clear prototype widget:
	mpPrototypeViewListWidget->setClusteringResult(&this->mDummyClustering);
	// clear cluster view widget:
	mpClusterViewWidget->setCluster(this->mDummyClustering.begin());

	// clear data:
	mpClusteringResult = 0x0;
	mpDocument = 0x0;

	return;
} // end clearClusteringResult()

ClusterSummaryWidget* ClusteringResultPrototypeViewWidget::addClusterSummaryWidget(int number, CharClustIt clustIt)
{
	ClusterSummaryWidget *pClusterSummaryWidget = new ClusterSummaryWidget(clustIt, mpInternalClusterSummaryWidget);
	// connect SIGNALS/SLOTS:
	connect(pClusterSummaryWidget, SIGNAL(showClusterViewWidget(CharClustIt)), this, SLOT(showClusterViewWidget(CharClustIt)));
	connect(pClusterSummaryWidget, SIGNAL(moveToCluster(CharClustIt, CharClustIt, std::vector<ImageCharIt>&)), this, SLOT(moveToCluster(CharClustIt, CharClustIt,std::vector<ImageCharIt>&)));

	mClusterSummaryList.append(pClusterSummaryWidget);
	mpClusterSummaryLayout->addWidget(pClusterSummaryWidget);
	mpInternalClusterSummaryWidget->resize(mpInternalClusterSummaryWidget->sizeHint());

	return pClusterSummaryWidget;
}

void ClusteringResultPrototypeViewWidget::showClusterViewWidget(CharClustIt clustIt)
{
//	std::cout << "showClusterViewWidget, index = " << i << std::endl;
	if (!hasClusteringResult()) return;

//	printf("address of mpClusteringResult = 0x%x, mCurrentClusterIndex=%d\n", mpClusteringResult, mCurrentClusterIndex);

	mpClusterViewWidget->setCluster(clustIt);

	return;
} // showClusterViewWidget(int i)

void ClusteringResultPrototypeViewWidget::showTrashCluster()
{
	showClusterViewWidget(mpClusteringResult->getTrashCluster());

	return;
}


ClusteringResult* ClusteringResultPrototypeViewWidget::getClusteringResult()
{
	return mpClusteringResult;
}

void ClusteringResultPrototypeViewWidget::reClustering()
{
	// FIXME: move code to ClusteringResult class
//	std::cout << "reclustering!" << std::endl;

	try {
		// make backup:
		mClusteringResultBackup = *mpClusteringResult;

		int nClusters = mpReclusterHierarchicalSpinBox->value();
		std::cout << "nr of new clusters = " << nClusters << std::endl;

		mpClusteringResult->reClusterHierarchical(nClusters);
		std::cout << "opening clustering result" << std::endl;

		this->openClusteringResult(mpClusteringResult, mpDocument);
		emit refreshInfoGroupBox();
	}
	 catch (Exception e) {
		 std::cerr << "An exception has occured in ClusteringResultPrototypeViewWidget::reClustering: " << e.what() << std::endl;
		 QMessageBox::critical(this,"An error has occured re-clustering!", e.what() );
	 }

	return;
}

void ClusteringResultPrototypeViewWidget::labelCurrentCluster()
{
//	std::cout << "labelling current clustering" << std::endl;
	QString labelText = mpLabelLineEdit->text();
	if (labelText.isEmpty()) {
//		std::cout << "empty label text... jump out" << std::endl;
		return;
	}
	labelText.toUpper(); // not needed ... testing purpose

	bool ok;
	unsigned int unicode = labelText.toUInt(&ok, 16);
	if (!ok) {
		throw Exception("Fatal error converting string to unicode - should not happen...");
	}

	CharClustIt currentClustIt = mpClusterViewWidget->getClusterIterator();
	(*currentClustIt)->setUnicodeLabel(unicode);

	// UPDATE VIEWS IN CLUSTER PROTOTYPE AND CLUSTER SUMMARY WIDGET:
	int prototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(currentClustIt);
	if (prototypeIndex != -1) {
		mpPrototypeViewListWidget->updatePrototype(prototypeIndex);
		mpPrototypeViewListWidget->updateLabel(prototypeIndex);
		int summaryIndex = findClusterSummaryWidgetIndex(currentClustIt);
		mClusterSummaryList.at(summaryIndex)->reload();
	}

	return;
}

void ClusteringResultPrototypeViewWidget::moveToCluster(CharClustIt from, CharClustIt to, std::vector<ImageCharIt>& iterators)
{
	if (from == to) return;

	// make backup:
	mClusteringResultBackup = *mpClusteringResult;

	mpClusteringResult->moveElements(from, to, iterators);
//std::cout << "moved elements, now updating!!" << std::endl;
	// update prototype widgets:
	int fromPrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(from);
	if (fromPrototypeIndex != -1) mpPrototypeViewListWidget->updatePrototype(fromPrototypeIndex);
	int toPrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(to);
	if (toPrototypeIndex != -1) mpPrototypeViewListWidget->updatePrototype(toPrototypeIndex);

	// update cluster summary widgets:
#if 1
	int fromSummaryIndex = findClusterSummaryWidgetIndex(from);
	if (fromSummaryIndex != -1) mClusterSummaryList.at(fromSummaryIndex)->reload();
	int toSummaryIndex = findClusterSummaryWidgetIndex(to);
	if (toSummaryIndex != -1) mClusterSummaryList.at(toSummaryIndex)->reload();
#endif

	emit showClusterViewWidget(from);
	emit refreshInfoGroupBox();

	return;
}

void ClusteringResultPrototypeViewWidget::moveToTrashCluster(CharClustIt from, std::vector<ImageCharIt>& iterators)
{
	std::cout << "moving to trash cluster" << std::endl;
	if (from == mpClusteringResult->getTrashCluster()) {
		return;
	}
	mpClusteringResult->moveToTrash(from, iterators);

	int fromPrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(from);
	if (fromPrototypeIndex != -1) mpPrototypeViewListWidget->updatePrototype(fromPrototypeIndex);

	int fromSummaryIndex = findClusterSummaryWidgetIndex(from);
	if (fromSummaryIndex != -1) mClusterSummaryList.at(fromSummaryIndex)->reload();

	emit showClusterViewWidget(from);
	emit refreshInfoGroupBox();

	return;
}

void ClusteringResultPrototypeViewWidget::mergeWithCluster(CharClustIt from, CharClustIt to)
{
	if (from == to) return;

	mpClusteringResult->mergeClusters(from, to);

	// update prototype widget:
	int fromPrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(from);
	mpPrototypeViewListWidget->takeItem(fromPrototypeIndex);
	int toPrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(to);
	mpPrototypeViewListWidget->updatePrototype(toPrototypeIndex);

	// update cluster summary widgets:
	int fromSummaryIndex = findClusterSummaryWidgetIndex(from);
	delete mClusterSummaryList.takeAt(fromSummaryIndex);
	int toSummaryIndex = findClusterSummaryWidgetIndex(to);
	mClusterSummaryList.at(toSummaryIndex)->reload();

	showClusterViewWidget(to);
	mpInternalClusterSummaryWidget->resize(mpInternalClusterSummaryWidget->sizeHint());

	emit refreshInfoGroupBox();

	return;
}

void ClusteringResultPrototypeViewWidget::mergeClusters(std::vector<CharClustIt> from, CharClustIt to)
{
	// make backup:
	mClusteringResultBackup = *mpClusteringResult;

#if 0 // old version -> unefficient for many from clusters
	for (int i=0; i<from.size(); ++i) {
		if (from[i] != to) {
			this->mergeWithCluster(from[i], to);
//			if (from[i] < to) to--; // adopt to index if removed cluster was before
//			// adopt rest of from indices if removed cluster was before:
//			for (int j=i+1; j<from.size(); ++j) {
//				if (from[i] < from[j]) from[j]--;
//			}
		}
	}
	mpInternalClusterSummaryWidget->resize(mpInternalClusterSummaryWidget->sizeHint());
	emit refreshInfoGroupBox();
#else
	for (int i=0; i<from.size(); ++i) {
		if (from[i] != to) {
			mpClusteringResult->mergeClusters(from[i], to);
			// take from item from prototype view widget:
			int fromPrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(from[i]);
			mpPrototypeViewListWidget->takeItem(fromPrototypeIndex);
			// take from item from cluster summary widget:
			int fromSummaryIndex = findClusterSummaryWidgetIndex(from[i]);
			delete mClusterSummaryList.takeAt(fromSummaryIndex);
		}
	}

	// update prototype for to element in prototype view widget:
	int toPrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(to);
	mpPrototypeViewListWidget->updatePrototype(toPrototypeIndex);
	// update cluster summary widget for to item:
	int toSummaryIndex = findClusterSummaryWidgetIndex(to);
	mClusterSummaryList.at(toSummaryIndex)->reload();
	// show cluster view widget of to item:
	showClusterViewWidget(to);

	mpInternalClusterSummaryWidget->resize(mpInternalClusterSummaryWidget->sizeHint());
	emit refreshInfoGroupBox();
#endif

	return;
}

void ClusteringResultPrototypeViewWidget::mergeSelectedClusters()
{
	if (mpClusteringResult==0x0) return;

	// determine selected clusters:
	QList<QListWidgetItem*> selItems = mpPrototypeViewListWidget->selectedItems();
	if (selItems.size()<=1) return;
    QList<QListWidgetItem*>::iterator it = selItems.begin();

    CharClustIt to = ((ClusterListWidgetItem *)(*it))->mClustIt;
    ++it;
    std::vector<CharClustIt> from;
    for (; it!=selItems.end(); ++it) {
    	from.push_back(((ClusterListWidgetItem *)(*it))->mClustIt);
    }
    this->mergeClusters(from, to);

	return;
}

void ClusteringResultPrototypeViewWidget::removeSelectedClusters()
{
	if (mpClusteringResult==0x0) return;

	// determine selected clusters:
	QList<QListWidgetItem*> selItems = mpPrototypeViewListWidget->selectedItems();
	if (selItems.size()<1) return;
	if (selItems.size() == mpClusteringResult->nClusters()) {
		QMessageBox::information(this, "Cannot remove all clusters", "Cannot remove all clusters from clustering!");
		return;
	}

	// make backup:
	mClusteringResultBackup = *mpClusteringResult;

    QList<QListWidgetItem*>::iterator it = selItems.begin();
    for (; it!=selItems.end(); ++it) {
    	// update prototype widget:
    	int removePrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(((ClusterListWidgetItem *)(*it))->mClustIt);
    	mpPrototypeViewListWidget->takeItem(removePrototypeIndex);
    	// update cluster summary widgets:
    	int removeSummaryIndex = this->findClusterSummaryWidgetIndex(((ClusterListWidgetItem *)(*it))->mClustIt);
    	delete mClusterSummaryList.takeAt(removeSummaryIndex);
    	// actually remove cluster from clustering:
    	mpClusteringResult->removeCluster(((ClusterListWidgetItem *)(*it))->mClustIt);
    }
    // show content of first cluster:
	showClusterViewWidget(mpClusteringResult->begin());
	mpInternalClusterSummaryWidget->resize(mpInternalClusterSummaryWidget->sizeHint());
	emit refreshInfoGroupBox();

	return;
}

void ClusteringResultPrototypeViewWidget::removeCluster(CharClustIt clustIt)
{
	// make backup:
	mClusteringResultBackup = *mpClusteringResult;

	// ask if seriously want to remove an empty(!) cluster:
	if ( !(*clustIt)->empty() )
		if ( QMessageBox::question(this, "Cluster not empty", "Do really want to remove this non-empty cluster?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes) {
			return;
		}
	mpClusteringResult->removeCluster(clustIt);

	// update prototype widget:
	int removePrototypeIndex = mpPrototypeViewListWidget->findClusterItemIndex(clustIt);
	mpPrototypeViewListWidget->takeItem(removePrototypeIndex);
	// update cluster summary widgets:
	int removeSummaryIndex = this->findClusterSummaryWidgetIndex(clustIt);
	delete mClusterSummaryList.takeAt(removeSummaryIndex);

	showClusterViewWidget(mpClusteringResult->begin());
	mpInternalClusterSummaryWidget->resize(mpInternalClusterSummaryWidget->sizeHint());
	emit refreshInfoGroupBox();

	return;
} // end removeCluster

void ClusteringResultPrototypeViewWidget::addEmptyCluster()
{
	// add empty cluster to clustering result:
	if (hasClusteringResult() && mpDocument != 0x0) {
		mpClusteringResult->addEmptyCluster(mpDocument);
		// update prototype widget:
		mpPrototypeViewListWidget->addEmptyCluster();
		// update cluster summary widgets:
		int lastIndex = mpClusteringResult->nClusters()-1;
		CharClustIt clustItEnd = mpClusteringResult->end();
		--clustItEnd;
		addClusterSummaryWidget(lastIndex, clustItEnd);
	}
	mpInternalClusterSummaryWidget->resize(mpInternalClusterSummaryWidget->sizeHint());
	emit refreshInfoGroupBox();

	return;
} // end addEmptyCluster()

void ClusteringResultPrototypeViewWidget::removeEmptyClusters()
{
	if (hasClusteringResult()) {
		mpClusteringResult->removeEmptyClusters();
		this->openClusteringResult(mpClusteringResult, mpDocument);

	} // end if hasClusteringResult()
	return;
} // end removeEmptyClusters()

void ClusteringResultPrototypeViewWidget::showUnsorted()
{
    int sizeThreshLow = mpSizeThreshLowSpinBox->value();
    int sizeThreshHigh = mpSizeThreshHighSpinBox->value();
    if (mpSizeCheckBox->isChecked()) { // if show all check box is checked, set thresh to 0, s.t. all clusters are shown!
    	sizeThreshLow = -1;
    	sizeThreshHigh = -1;
    }

    mpPrototypeViewListWidget->updateList(sizeThreshLow, sizeThreshHigh);
    return;
}

void ClusteringResultPrototypeViewWidget::sortByNearestNeighbor()
{
	// determine selected clusters:
	QList<QListWidgetItem*> selItems = mpPrototypeViewListWidget->selectedItems();
	if (selItems.size()<1) return; // of none selected, jump out
    QList<QListWidgetItem*>::iterator it = selItems.begin(); // sort by first in list

    this->mpClusteringResult->computePrototypeFeatures(); // recompute features; FIXME: unefficient: on demand??
    std::vector<CharClustIt> kNNSorting;
    mpClusteringResult->computekNNSorting(((ClusterListWidgetItem*)(*it))->mClustIt, kNNSorting);

    int sizeThreshLow = mpSizeThreshLowSpinBox->value();
    int sizeThreshHigh = mpSizeThreshHighSpinBox->value();
    if (mpSizeCheckBox->isChecked()) { // if show all check box is checked, set thresh to 0, s.t. all clusters are shown!
    	sizeThreshLow = -1;
    	sizeThreshHigh = -1;
    }

    mpPrototypeViewListWidget->updateListWithOrdering(kNNSorting, sizeThreshLow, sizeThreshHigh);

    return;
}

void ClusteringResultPrototypeViewWidget::sortBySizeDescending()
{
	std::cout << "sorting by size descending" << std::endl;

    std::vector<CharClustIt> sizeSorting;
    mpClusteringResult->computeSizeSorting(sizeSorting);
	// reverse sorting, s.t. largest cluster is first:
	std::reverse(sizeSorting.begin(), sizeSorting.end());

    int sizeThreshLow = mpSizeThreshLowSpinBox->value();
    int sizeThreshHigh = mpSizeThreshHighSpinBox->value();
    if (mpSizeCheckBox->isChecked()) { // if show all check box is checked, set thresh to 0, s.t. all clusters are shown!
    	sizeThreshLow = -1;
    	sizeThreshHigh = -1;
    }

    mpPrototypeViewListWidget->updateListWithOrdering(sizeSorting, sizeThreshLow, sizeThreshHigh);

    return;
}

void ClusteringResultPrototypeViewWidget::sortBySizeAscending()
{
	std::cout << "sorting by size descending" << std::endl;

    std::vector<CharClustIt> sizeSortingAscending;
    mpClusteringResult->computeSizeSorting(sizeSortingAscending);

    int sizeThreshLow = mpSizeThreshLowSpinBox->value();
    int sizeThreshHigh = mpSizeThreshHighSpinBox->value();
    if (mpSizeCheckBox->isChecked()) { // if show all check box is checked, set thresh to 0, s.t. all clusters are shown!
    	sizeThreshLow = -1;
    	sizeThreshHigh = -1;
    }

    mpPrototypeViewListWidget->updateListWithOrdering(sizeSortingAscending, sizeThreshLow, sizeThreshHigh);

    return;
}

void ClusteringResultPrototypeViewWidget::restoreBackupClustering()
{
	if (mpClusteringResult != 0x0 && !mClusteringResultBackup.isEmpty() ) {
//		std::cout << "n of c in backup = " << mClusteringResultBackup.nClusters() << std::endl;

		mpClusteringResult->deleteClustering();
		*mpClusteringResult = mClusteringResultBackup;

		this->openClusteringResult(mpClusteringResult, mpDocument);
		emit refreshInfoGroupBox();
	}
	return;
}

void ClusteringResultPrototypeViewWidget::reloadViews()
{
	std::cout << "reloading views..." << std::endl;
	for (int i=0; i < mpPrototypeViewListWidget->count(); ++i) {
		std::cout << "i = " << i << std::endl;
		((ClusterListWidgetItem *)(mpPrototypeViewListWidget->item(i)))->updatePrototypeImage();
	}
	for (int i=0; i<this->mClusterSummaryList.size(); ++i) {
		std::cout << "i = " << i << std::endl;
		mClusterSummaryList.at(i)->updatePrototypeImage();
	}

	return;
}


} // end of namespace impact_tr4
