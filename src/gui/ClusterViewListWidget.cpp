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
#include <QtGui>

#include "ClusterViewListWidget.hpp"

namespace impact_tr4 {

//ImageCharListWidgetItem::ImageCharListWidgetItem(const ImageCharListWidgetItem& source)
//: QListWidgetItem(source), /*mImageChar(source.mImageChar),*/ mpImageChar(source.mpImageChar)
//{
//	std::cout << "copy constructor of ImageCharListWidgetItem called!" << std::endl;
//}

ImageCharListWidgetItem::ImageCharListWidgetItem(const ImageCharListWidgetItem& other)
: QListWidgetItem(other)
{
	this->mIcon = other.mIcon;
	this->mpImageChar = other.mpImageChar;
}

ImageCharListWidgetItem::ImageCharListWidgetItem(ImageChar *pImageChar, ImageCharIt it, QListWidget * parent /*= 0x0*/)
	: QListWidgetItem(parent, QListWidgetItem::Type), mpImageChar(pImageChar), mIt(it)
{
	// TODO: fixme: memory leak!!!
    this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
    mpImageChar->pImage->setRoi(mpImageChar->bBox);
    QImage imageCharQImage(20, 20, QImage::Format_RGB32);
    mpImageChar->pImage->computeQImage(imageCharQImage);
    int newWidth = 35;
    int newHeight = 35;
    QImage imageCharQImageScaled = imageCharQImage.scaled( newWidth, newHeight, Qt::KeepAspectRatio );
//    QIcon icon(QPixmap::fromImage(imageCharQImageScaled));
    QPixmap pixmap = QPixmap::fromImage(imageCharQImageScaled);
    mIcon = QIcon(pixmap);
    this->setIcon(mIcon);
    mpImageChar->pImage->releaseRoi();
    this->setBackground(QBrush(Qt::gray));
}

ImageCharListWidgetItem::~ImageCharListWidgetItem()
{
//	std::cout << "destroying ImageCharListWidgetItem" << std::endl;
	this->setIcon(QIcon());
//	mIcon.detach();
}

ImageCharListWidgetItem* ImageCharListWidgetItem::clone () const
{
	return (new ImageCharListWidgetItem(*this));
}

ClusterViewListWidget::ClusterViewListWidget(CharClustIt clustIt, QWidget *parent /*=0*/ )
: QListWidget(parent), mClustIt(clustIt)
{
//	pParent = (ClusterViewWidget*)(parent);

//    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
//    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    this->set
//    this->setMinimumWidth(500);
//    this->resize(700,100);

//	this->setFixedHeight(100);
//	this->setFixedWidth(800);
//	this->setGridSize(QSize(126, 116));
	this->setIconSize(QSize(80, 80));
	this->setSpacing(5);

	this->setFlow(QListView::LeftToRight);
//	this->setFlow(QListView::TopToBottom);
//  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewMode(QListView::IconMode);
    this->setResizeMode(QListView::Adjust);
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);

	this->setDragEnabled(true);
	this->viewport()->setAcceptDrops(true);
	this->setDropIndicatorShown(true);
	this->setDragDropMode(QAbstractItemView::DragDrop);
	//this->setDragDropMode(QAbstractItemView::InternalMove);

	this->setWrapping(true);

//	connect(this, SIGNAL(itemEntered(QListWidgetItem*)), this, SLOT(setCurrentItemEntered(QListWidgetItem*)));

//	this->updateList();
	// Register stream operators for class ImageChar so that it can be streamed:
//	qRegisterMetaTypeStreamOperators<ImageChar>("ImageChar");
}


//ClusterViewListWidget::~ClusterViewListWidget()
//{
//	std::cout << "destroying ClusterViewListWidget!" << std::endl;
////	this->clear();
//
//};

void ClusterViewListWidget::setCluster(CharClustIt clustIt)
{
	mClustIt = clustIt;
	updateList();
}

void ClusterViewListWidget::updateList()
{
#if 1
	// clear current list first:
	this->clear();

	CharCluster *pCharCluster = *mClustIt;
	// check for empty or null cluster:
	if (pCharCluster==NULL) { return; }
	if (pCharCluster->empty()) { return; }

//	std::list<ImageChar*>& charList = pCharCluster->charList();
	std::cout << "updating list, nr of elements = " << pCharCluster->size() << std::endl;
	ImageCharIt it = pCharCluster->begin();

	while ( it!=pCharCluster->end() ) {
//		std::cout << "creating ImageCharListWidgetItem" << std::endl;
		#if 0
		// method 1: do NOT specify parent, since this done by adding item using addItem later!
		ImageCharListWidgetItem *item = new ImageCharListWidgetItem(*it, posCount++, it, this);
		this->addItem(item);
		#else
		// method 2: specify parent, s.t. item is automatically added to list:
		ImageCharListWidgetItem *item = new ImageCharListWidgetItem(*it, it, this);
		#endif

		++it;
	} // end while
#endif
//	this->resize(this->sizeHint());

	return;
}

std::vector<ImageCharIt> ClusterViewListWidget::getSelectedItemsIterators()
{
	std::vector<ImageCharIt> vec;

    QList<QListWidgetItem*> selItems = selectedItems();
    QList<QListWidgetItem*>::iterator it = selItems.begin();

    for (; it!=selItems.end(); ++it) {
    	vec.push_back( ((ImageCharListWidgetItem *)(*it))->mIt );
    }

    return vec;
}

void ClusterViewListWidget::dragEnterEvent(QDragEnterEvent *event)
{
#if 1
	event->ignore();
#else
	if (event->mimeData()->hasFormat("clustering/ImageChar"))
		event->accept();
	else
		event->ignore();
#endif
}

void ClusterViewListWidget::dragMoveEvent(QDragMoveEvent *event)
{
#if 1
	event->ignore();
#else
    if (event->mimeData()->hasFormat("clustering/ImageChar")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
//        std::cout << event->pos().x() << ", " << event->pos().y() << std::endl;
    }
    else
        event->ignore();
#endif
}

void ClusterViewListWidget::dropEvent(QDropEvent *event)
{
#if 1
	event->ignore();
#else // old and retarded...
    if (event->mimeData()->hasFormat("clustering/ImageChar")) {
        QByteArray pieceData = event->mimeData()->data("clustering/ImageChar");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);

        int nElements;
        dataStream >> nElements;
        for (int i=0; i<nElements; ++i) {
//        	ImageChar imageChar;
//        	dataStream >> imageChar;

        	ImageChar *pImageChar;
        	int address;
        	dataStream >> address;
        	pImageChar = (ImageChar *) address;

			this->addImageChar(pImageChar);
        }

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
#endif

    emit releaseMouseEvents();
}

void ClusterViewListWidget::startDrag(Qt::DropActions supportedActions)
{
    QListWidgetItem *item = currentItem();
    QList<QListWidgetItem*> selItems = selectedItems();

//    ImageCharListWidgetItem *listWidgetItem = (ImageCharListWidgetItem *)(item);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    // stream nr of elements and elements themselves into datastream:
    dataStream << (quint64)(&mClustIt);
    dataStream << selItems.size();
    QList<QListWidgetItem*>::iterator it = selItems.begin();
    for (; it!=selItems.end(); ++it) {
    	dataStream << (quint64)(&((ImageCharListWidgetItem *)(*it))->mIt);
    }

    QPixmap pixmap = qVariantValue<QPixmap>(item->data(Qt::UserRole));
    QPoint location = item->data(Qt::UserRole+1).toPoint();
//    dataStream << pixmap << location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("clustering/ImageChar", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);

    emit grabMouseEvents();

    // start drag:
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
    	// do nothing, since widget is updated anyway from outside
#if 0
    	it = selItems.begin();
    	for (; it!=selItems.end(); ++it) {
    		delete takeItem(row(*it));
    	} // end for
#endif
    }
    return;
} // end startDrag


#if 0
QDataStream &operator<<(QDataStream &out, const ImageChar &myObj)
{
//	std::cout << "calling operator <<!" << std::endl;

	int address;
	// Stream pImage pointer as casted int:
	address = (int) myObj.pImage;
//	printf("0x%x\n", myObj.pImage);
	out << address;
	// Stream bounding box:
	out << myObj.bBox.x(); out << myObj.bBox.y(); out << myObj.bBox.width(); out << myObj.bBox.height();
	// Stream imageID:
	out << myObj.imageID;
	// Stream ID:
	out << myObj.id;
	// steam text:
	out << QString::fromStdString(myObj.text);

	// Stream pDataMat pointer as casted int:
	address = (int) myObj.pDataMat;
//	printf("0x%x\n", myObj.pDataMat);
	out << address;
	// Stream dataRow
	out << myObj.dataRow;

	return out;
} // end operator<<

QDataStream &operator>>(QDataStream &in, ImageChar &myObj)
{
//	std::cout << "calling operator >>!" << std::endl;

	int address;

	in >> address;
	myObj.pImage = (GrayImage<>*)address;
//	printf("0x%x\n", myObj.pImage);

	int x, y, width, height;
	in >> x; in >> y; in >> width; in >> height;
	BBox bBox(x, y, width, height);
	myObj.bBox = bBox;
	in >> myObj.imageID;
	in >> myObj.id;
	QString textQStr;
	in >> textQStr;
	myObj.text = textQStr.toStdString();

	in >> address;
	myObj.pDataMat = (ublas::matrix<float> *) address;
//	printf("0x%x\n", myObj.pDataMat);

	in >> myObj.dataRow;

	return in;
}
#endif

} // end of namespace impact_tr4
