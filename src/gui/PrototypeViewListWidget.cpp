/*
 * PrototypeViewListWidget.cpp
 *
 *  Created on: 20.07.2010
 *      
 */

#include <QListWidget>
#include <QtGui>

#include "PrototypeViewListWidget.hpp"

namespace impact_tr4 {

ClusterListWidgetItem::ClusterListWidgetItem(const ClusterListWidgetItem& other)
: QListWidgetItem(other)
{
	this->mIcon = other.mIcon;
	this->mClustIt = other.mClustIt;
}

ClusterListWidgetItem::ClusterListWidgetItem(CharClustIt clustIt, QListWidget * parent /*= 0x0*/)
	: QListWidgetItem(parent, QListWidgetItem::Type), mClustIt(clustIt)
//QListWidgetItem::UserType
{

//	QFont itemFont(); this->setFont(itemFont);
	this->setFlags( Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
					Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);
//	this->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
//					Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);

	updatePrototypeImage();
	updateLabel();
}

void ClusterListWidgetItem::updateLabel()
{
	unsigned int unicode = (*mClustIt)->unicodeLabel();
	QString hexStr;
	hexStr.setNum(unicode, 16);
//	hexStr.toUpper();

//	QString testHexStr; testHexStr.setNum(15, 16);
//	std::cout << testHexStr.toStdString() << std::endl;

	this->setText(hexStr);

	return;
}

void ClusterListWidgetItem::updatePrototypeImage()
{
	const int maxIconSize = 99;

	if ((*mClustIt)->empty()) {
		this->setIcon(QIcon());
//		this->setText("E M P T Y");
		QImage image(maxIconSize, maxIconSize, QImage::Format_RGB32);
		unsigned int gray = 0xa0a0a4;
		image.fill(gray);
		QPixmap pixmap = QPixmap::fromImage(image);
	    mIcon = QIcon(pixmap);
	    this->setIcon(mIcon);
	    this->setBackground(QBrush(Qt::gray));
		return;
	}

//	(*mClustIt)->recomputePrototypeImage();
//	std::cout << "recomputing prototype image in ClusterListWidgetItem::updatePrototypeImage..." << std::endl;
	QImage imageCharQImage;
	(*mClustIt)->getPrototypeImagePointer()->computeQImage(imageCharQImage);
//	imageCharQImage.invertPixels();
#if 0
	QImage prototypeImage;
	convertGrayscaleToJet(imageCharQImage, prototypeImage);
#else
	QImage prototypeImage = imageCharQImage;
#endif
    QImage prototypeImageScaled = prototypeImage.scaled( maxIconSize, maxIconSize, Qt::KeepAspectRatio );

	QPixmap pixmap = QPixmap::fromImage(prototypeImageScaled);
    mIcon = QIcon(pixmap);
    this->setIcon(mIcon);
    this->setBackground(QBrush(Qt::gray));

	return;
}

ClusterListWidgetItem::~ClusterListWidgetItem()
{
//	std::cout << "destroying ImageCharListWidgetItem" << std::endl;
	this->setIcon(QIcon());
//	mIcon.detach();
}

ClusterListWidgetItem* ClusterListWidgetItem::clone () const
{
	return (new ClusterListWidgetItem(*this));
}

PrototypeViewListWidget::PrototypeViewListWidget(ClusteringResult *pClusteringResult, QWidget *parent /*=0*/ )
: QListWidget(parent), mpClusteringResult(pClusteringResult)
{
//    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
//    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    this->set
//    this->setMinimumWidth(500);
//    this->resize(700,100);

//	this->setFixedHeight(100);
//	this->setFixedWidth(800);
//	this->setGridSize(QSize(126, 116));

	this->setIconSize(QSize(60, 60));
	this->setSpacing(10);

	this->setFlow(QListView::LeftToRight);
//	this->setFlow(QListView::TopToBottom);
//	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setViewMode(QListView::IconMode);
    this->setResizeMode(QListView::Adjust);
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);

	this->setDragEnabled(true);
	this->viewport()->setAcceptDrops(true);
	this->setDropIndicatorShown(true);
	this->setDragDropMode(QAbstractItemView::DragDrop);
	//this->setDragDropMode(QAbstractItemView::InternalMove);

	this->setWrapping(true);

	this->updateList();

	connect(this, SIGNAL(itemDoubleClicked (QListWidgetItem  *)), this, SLOT(on_itemDoubleClicked(QListWidgetItem  *)));
	// Register stream operators for class ImageChar so that it can be streamed:
//	qRegisterMetaTypeStreamOperators<ImageChar>("ImageChar");
}

int PrototypeViewListWidget::findClusterItemIndex(CharClustIt clustIt)
{
	for (int i=0; i<this->count(); ++i) {
		ClusterListWidgetItem* curr_item = (ClusterListWidgetItem*)(this->item(i));
		if (curr_item->mClustIt == clustIt)
			return i;
	}
	return -1;
//	std::cerr << "Item has not been found!" << std::endl;
//	throw Exception("Item has not been found!");
}

void PrototypeViewListWidget::updateIconSize(int size)
{
//	std::cout << "updating icon size to " << size << std::endl;
	this->setIconSize(QSize(size, size)); this->update();

	return;
}

//PrototypeViewListWidget::~PrototypeViewListWidget()
//{
//	std::cout << "destroying PrototypeViewListWidget!" << std::endl;
////	this->clear();
//
//};

void PrototypeViewListWidget::on_itemDoubleClicked(QListWidgetItem  *item)
{
//	std::cout << "item double clicked, index = " << ((ClusterListWidgetItem*)item)->mClusterID << std::endl;
	emit showClusterViewWidget(((ClusterListWidgetItem*)item)->mClustIt);

	return;
}

void PrototypeViewListWidget::setClusteringResult(ClusteringResult *pClusteringResult)
{
	mpClusteringResult = pClusteringResult;
#if 1
	updateList();
#else
	// TEST: list cluster elements according to specified ordering
	std::cout << "TEST: list clusters accoring to ordering!" << std::endl;
	updateListWithOrdering(mpClusteringResult->getOrderedClusters());
#endif
}

void PrototypeViewListWidget::updateListWithOrdering(const std::vector<CharClustIt> &ordering, int sizeThreshLow, int sizeThreshHigh)
{ // FIXME ??
#if 1
	this->clear();
	if (mpClusteringResult == 0x0) return;

	// add clusters to list widget:
	for (int i=0; i<ordering.size(); ++i) {
//		int clusterIndex = mpClusteringResult->getClusterIndex(clust_it);
//		std::cout << "ordering[i]=" << ordering[i] << std::endl;
		int s = (*ordering[i])->size();
		if ( (s >=sizeThreshLow &&  s <= sizeThreshHigh) || sizeThreshLow == -1 || sizeThreshHigh == -1 ) { // insert only when size threshold is satisfied or set to -1
			ClusterListWidgetItem *item = new ClusterListWidgetItem(ordering[i], this);
		}
	}
//	this->resize(this->sizeHint());
#endif
	return;
}

void PrototypeViewListWidget::updateList(int sizeThreshLow/*=-1*/, int sizeThreshHigh/*=-1*/)
{
	this->clear();
	if (mpClusteringResult == 0x0) return;

	// add clusters to list widget:
	for (CharClustIt clust_it = mpClusteringResult->begin(); clust_it!=mpClusteringResult->end(); ++clust_it) {
		int s = (*clust_it)->size();
		if ( (s >=sizeThreshLow &&  s <= sizeThreshHigh) || sizeThreshLow == -1 || sizeThreshHigh == -1 ) { // insert only when size threshold is satisfied or set to -1
			ClusterListWidgetItem *item = new ClusterListWidgetItem(clust_it, this);
		}
//		ClusterListWidgetItem *item = new ClusterListWidgetItem(clust_it, this);
	}
//	this->resize(this->sizeHint());

	return;
}

void PrototypeViewListWidget::addEmptyCluster()
{
	CharClustIt clustItEnd = mpClusteringResult->end();
	--clustItEnd;
	ClusterListWidgetItem *item = new ClusterListWidgetItem(clustItEnd, this);

	return;
}

void PrototypeViewListWidget::updatePrototype(int i)
{
	((ClusterListWidgetItem*)this->item(i))->updatePrototypeImage();
	return;
}

void PrototypeViewListWidget::updateLabel(int i)
{
	((ClusterListWidgetItem*)this->item(i))->updateLabel();
	return;
}

void PrototypeViewListWidget::dragEnterEvent(QDragEnterEvent *event)
{
#if 0
	event->ignore();
#else
	if ( event->mimeData()->hasFormat("clustering/Cluster") || event->mimeData()->hasFormat("clustering/ImageChar") )
		event->accept();
	else
		event->ignore();
#endif
}

void PrototypeViewListWidget::dragMoveEvent(QDragMoveEvent *event)
{
#if 0
	event->ignore();
#else
    if ( event->mimeData()->hasFormat("clustering/Cluster") || event->mimeData()->hasFormat("clustering/ImageChar") ) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
//        std::cout << event->pos().x() << ", " << event->pos().y() << std::endl;
    }
    else
        event->ignore();
#endif
}

void PrototypeViewListWidget::dropEvent(QDropEvent *event)
{ // FIXME?
	if (event->mimeData()->hasFormat("clustering/Cluster")) {
//		std::cout << "dropping!!" << std::endl;
		// retrieve item that was dropped on:
		QListWidgetItem *pItem = this->itemAt(event->pos());
		if (pItem==0x0) { // of no item at this position, reject
			event->ignore();
			return;
		}
		ClusterListWidgetItem *pClusterItem = (ClusterListWidgetItem*)pItem;
		// receive sender cluster id:
        QByteArray pieceData = event->mimeData()->data("clustering/Cluster");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        int nElements;
        dataStream >> nElements;
        quint64 address;
        CharClustIt fromIt;
        std::vector<CharClustIt> from;
        for (int i=0; i<nElements; ++i) {
        	dataStream >> address;
        	fromIt = *((CharClustIt *) address);
        	from.push_back(fromIt);
//        	std::cout << "merging from " << fromID << " to " << pClusterItem->mClusterID << std::endl;
        }

        emit mergeClusters(from, pClusterItem->mClustIt);

        event->setDropAction(Qt::MoveAction);
        event->accept();
	} // end if clustering/Cluster
	else if (event->mimeData()->hasFormat("clustering/ImageChar")) {
		// retrieve item that was dropped on:
		QListWidgetItem *pItem = this->itemAt(event->pos());
		if (pItem==0x0) { // of no item at this position, reject
			event->ignore();
			return;
		}
		ClusterListWidgetItem *pClusterItem = (ClusterListWidgetItem*)pItem;

        QByteArray pieceData = event->mimeData()->data("clustering/ImageChar");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);

        CharClustIt senderIt;
        quint64 address;
        dataStream >> address;
        senderIt = *((CharClustIt *) address);

        int nElements;
        dataStream >> nElements;

        std::vector<ImageCharIt> iterators;
        for (int i=0; i<nElements; ++i) {
        	dataStream >> address;

        	ImageCharIt *pIt = (ImageCharIt *) address;
        	iterators.push_back(*pIt);
        }
        // move elemnts:
//        std::cout << "moving elements from " << senderIt << " to " << pClusterItem->mClustIt << std::endl;
        emit moveToCluster(senderIt, pClusterItem->mClustIt, iterators);

        // accept event
    	event->setDropAction(Qt::MoveAction);
    	event->accept();
    } // end if clustering/ImageChar
	else {
		event->ignore();
	}

    emit releaseMouseEvents();
}

void PrototypeViewListWidget::startDrag(Qt::DropActions supportedActions)
{ // FIXME?

//	std::cout << "dragging!!" << std::endl;
    QList<QListWidgetItem*> selItems = selectedItems();
    // stream nr of elements and elements themselves into datastream:
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << selItems.size();
    QList<QListWidgetItem*>::iterator it = selItems.begin();
    for (; it!=selItems.end(); ++it) {
    	dataStream << (quint64)(&((ClusterListWidgetItem *)(*it))->mClustIt);
//    	dataStream << ((ClusterListWidgetItem *)(*it))->mClusterID;
    }

    QPixmap pixmap = qVariantValue<QPixmap>(selItems.at(0)->data(Qt::UserRole));
    QPoint location = selItems.at(0)->data(Qt::UserRole+1).toPoint();
//    dataStream << pixmap << location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("clustering/Cluster", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);

//    emit grabMouseEvents();

    // start drag:
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
    	return;
    	// delete item if moved:
//    	delete takteItem(row(item));
//    	updateClusterIDs(); // update cluster id's
    } // end if item moved

    return;
} // end startDrag

} // end of namespace impact_tr4

