/*
 * ClusterViewWidget.cpp
 *
 *  Created on: 04.08.2009
 *      
 */

#include <QHBoxLayout>
#include <QSpinBox>

#include "ClusterViewWidget.hpp"

#include "clustering/CharCluster.hpp"

namespace impact_tr4 {

ClusterViewWidget::ClusterViewWidget(CharClustIt clustIt, QWidget *parent /*=0*/ )
: QWidget(parent), mClustIt(clustIt)
{
	mpClusterIDLabel = new QLabel("Cluster content - nr. of elements: "+QString::number((*mClustIt)->size()));
	QFont labelFont("Arial", 12, QFont::Bold);
	mpClusterIDLabel->setFont(labelFont);
	mpClusterIDLabel->setFixedHeight(mpClusterIDLabel->sizeHint().height());

//	// create move button:
//	mpMoveToButton = new QPushButton(tr("Move to"));
//	mpMoveToButton->setToolTip("Moves the selected items to the specified cluster");
//	mpMoveToButton->setFixedSize(mpMoveToButton->sizeHint());
////	mpMoveToButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//	connect(mpMoveToButton, SIGNAL(clicked()), this, SLOT(moveToButtonClicked()));
//
//	// create merge button:
//	mpMergeWithButton = new QPushButton(tr("Merge with"));
//	mpMergeWithButton->setToolTip("Merges this cluster with the specified cluster");
//	mpMergeWithButton->setFixedSize(mpMergeWithButton->sizeHint());
////	mpMergeWithButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//	connect(mpMergeWithButton, SIGNAL(clicked()), this, SLOT(mergeWithButtonClicked()));

	// create remove button:
	mpRemoveButton = new QPushButton(tr("Remove this cluster"));
	mpRemoveButton->setToolTip("Removes this cluster. Only allowed for empty clusters!");
	mpRemoveButton->setFixedSize(mpRemoveButton->sizeHint());
//	mpRemoveButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	connect(mpRemoveButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));

	QPushButton *pRemoveSelectedElementsButton = new QPushButton("Move selected elements to trash");
	pRemoveSelectedElementsButton->setFixedSize(pRemoveSelectedElementsButton->sizeHint());
	connect(pRemoveSelectedElementsButton, SIGNAL(clicked()), this, SLOT(moveSelectedItemsToTrash()));

	QHBoxLayout *hboxlayout1 = new QHBoxLayout();
//	hboxlayout1->addWidget(mpMoveToButton);
//	hboxlayout1->addWidget(mpMergeWithButton);
	hboxlayout1->addWidget(mpRemoveButton);
	hboxlayout1->addWidget(pRemoveSelectedElementsButton);
//	vboxlayout1->setFixedHeight(100);

//	mpClusterSelectSpinBox = new QSpinBox();
//	mpClusterSelectSpinBox->setRange(1, 100000000);
//	mpClusterSelectSpinBox->setValue(1);
//	mpClusterSelectSpinBox->setFixedSize(mpClusterSelectSpinBox->sizeHint());
////	mpClusterSelectSpinBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	// create cluster select combo box:
//	mpClusterSelectComboBox = new QComboBox();
//	mpClusterSelectComboBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

//	QHBoxLayout *hboxlayout2 = new QHBoxLayout();
//	QLabel *label = new QLabel("Cluster"); label->setFixedSize(label->sizeHint());
//	hboxlayout2->addWidget(label);
//	vboxlayout2->addWidget(mpClusterSelectComboBox);
//	hboxlayout2->addWidget(mpClusterSelectSpinBox);
//	vboxlayout2->setFixedHeight(100);

	mpClusterViewListWidget = new ClusterViewListWidget(mClustIt, this);
	mpClusterViewListWidget->updateList();
//	mpClusterViewListWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	connect(mpClusterViewListWidget, SIGNAL(grabMouseEvents()), this, SIGNAL(grabMouseEvents()));
	connect(mpClusterViewListWidget, SIGNAL(releaseMouseEvents()), this, SIGNAL(releaseMouseEvents()));

	// create info label:
	charInfoLabel = new QLabel("<b>Char Info:</b><br>BBox = [x,y,w,h]<br>ID = <br>Image-ID = <br>Text = ");
//	charInfoLabel->setFixedWidth(150);
	connect(mpClusterViewListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(displayCharInfo(QListWidgetItem *)));


//	QHBoxLayout *layout = new QHBoxLayout();
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(mpClusterIDLabel);
	layout->addWidget(mpClusterViewListWidget);

	layout->addLayout(hboxlayout1);
//	layout->addLayout(hboxlayout2);
	layout->addWidget(charInfoLabel);

//	hboxlayout->addWidget(mpMoveToButton);
//	hboxlayout->addWidget(mpMergeWithButton);
//	hboxlayout->addWidget(mpRemoveButton);
//	hboxlayout->addWidget(mpClusterSelectComboBox);

	this->setLayout(layout);
//	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
}

void ClusterViewWidget::clear()
{
	mpClusterIDLabel->setText("Cluster content - nr. elements: 0");
	mpClusterViewListWidget->clear();
}

void ClusterViewWidget::setCluster(CharClustIt clustIt)
{
	mClustIt = clustIt;
//	std::cout << "cluster id is: " << mClusterID << std::endl;
	mpClusterIDLabel->setText("Cluster content - nr. of elements: "+QString::number((*mClustIt)->size()));
	mpClusterViewListWidget->setCluster(mClustIt);

	return;
}

CharClustIt ClusterViewWidget::getClusterIterator() const
{
	return mClustIt;
}

//ClusterViewWidget::~ClusterViewWidget()
//{
//	std::cout << "destroying ClusterViewWidget!" << std::endl;
////	mpClusterViewListWidget->clear();
////	delete mpClusterViewListWidget;
//}

void ClusterViewWidget::displayCharInfo(QListWidgetItem * item)
{
	ImageCharListWidgetItem *pItem = (ImageCharListWidgetItem *)item;
	QString info("<b>Char Info:</b><br>");
	info += "BBox = ["+QString::number(pItem->mpImageChar->bBox.x())+","+
				QString::number(pItem->mpImageChar->bBox.y())+","+
				QString::number(pItem->mpImageChar->bBox.width())+","+
				QString::number(pItem->mpImageChar->bBox.height())+"]<br>";
	info += "ID = "+QString::number(pItem->mpImageChar->id)+"<br>";
	info += "Image-ID = "+QString::number(pItem->mpImageChar->imageID)+"<br>";
	QString text = QString::fromStdString(pItem->mpImageChar->text);
	text.prepend("<b>&gt;</b>");
	text.append("<b>&lt;</b>");
	info += "Text = "+text;
	info += QString("<br>") + "Dist to center = " + QString::number(pItem->mpImageChar->distToClusterCenter);
	charInfoLabel->setText(info);

	return;
}

//std::vector<ImageChar*> ClusterViewWidget::getSelectedImageChars()
//{
//	std::vector<ImageChar*> selectedImageChars;
//    QList<QListWidgetItem*> selItems = mpClusterViewListWidget->selectedItems();
//
//    QList<QListWidgetItem*>::iterator it = selItems.begin();
//    for (; it!=selItems.end(); ++it) {
//    	selectedImageChars.push_back(((ImageCharListWidgetItem *)(*it))->mpImageChar);
//    }
//    return selectedImageChars;
//}
//
//std::vector<int> ClusterViewWidget::getSelectedImageCharsPositions()
//{
//	std::vector<int> selectedImageChars;
//    QList<QListWidgetItem*> selItems = mpClusterViewListWidget->selectedItems();
//
//    QList<QListWidgetItem*>::iterator it = selItems.begin();
//    for (; it!=selItems.end(); ++it) {
//    	selectedImageChars.push_back(((ImageCharListWidgetItem *)(*it))->mPosition);
//    }
//    return selectedImageChars;
//}

std::vector<ImageCharIt> ClusterViewWidget::getSelectedImageCharsIterators()
{
	std::vector<ImageCharIt> selectedImageChars;
    QList<QListWidgetItem*> selItems = mpClusterViewListWidget->selectedItems();

    QList<QListWidgetItem*>::iterator it = selItems.begin();
    for (; it!=selItems.end(); ++it) {
    	selectedImageChars.push_back(((ImageCharListWidgetItem *)(*it))->mIt);
    }
    return selectedImageChars;
}

//void ClusterViewWidget::removeSelectedItems()
//{
//	// remove items from list:
//	QList<QListWidgetItem*> selItems = mpClusterViewListWidget->selectedItems();
//	QList<QListWidgetItem*>::iterator it = selItems.begin();
//	for (; it!=selItems.end(); ++it) {
//		delete mpClusterViewListWidget->takeItem(mpClusterViewListWidget->row(*it));
//	}
//	return;
//}

void ClusterViewWidget::moveSelectedItemsToTrash()
{
	std::vector<ImageCharIt> iterators = getSelectedImageCharsIterators();
	emit moveToTrashCluster(mClustIt, iterators);

    return;
}

void ClusterViewWidget::selectAllItems()
{
	for (int i=0; i<mpClusterViewListWidget->count(); ++i) {
		mpClusterViewListWidget->item(i)->setSelected(true);
	}

    return;
}

void ClusterViewWidget::deselectAllItems()
{
	for (int i=0; i<mpClusterViewListWidget->count(); ++i) {
		mpClusterViewListWidget->item(i)->setSelected(false);
	}

    return;
}

//int ClusterViewWidget::selectedClusterIndex() const
//{
//	int val = mpClusterSelectSpinBox->value();
//	if (val >=1 && val <= 1e36) {
//		return val-1;
//	}
//	else
//		return -1;
//
////	if (mpClusterSelectComboBox->currentIndex() != -1)
////		return mpClusterSelectComboBox->currentText().toInt()-1;
////	else
////		return -1;
//}

//void ClusterViewWidget::moveToButtonClicked()
//{
//	std::vector<std::list<ImageChar*>::iterator> iterators = getSelectedImageCharsIterators();
//	emit moveToCluster(mClusterID, selectedClusterIndex(), iterators);
//	return;
//}
//
//void ClusterViewWidget::mergeWithButtonClicked()
//{
//	emit mergeWithCluster(selectedClusterIndex(), mClusterID);
//	return;
//}

void ClusterViewWidget::removeButtonClicked()
{
	emit removeCluster(mClustIt);
	return;
}

//void ClusterViewWidget::wheelEvent(QWheelEvent *event)
//{
//	std::cout << "wheeling" << std::endl;
//}

} // end of namespace impact_tr4
