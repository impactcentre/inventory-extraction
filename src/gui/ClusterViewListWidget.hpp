/*
 * ClusterViewListWidget.hpp
 *
 *  Created on: 04.08.2009
 *      
 */

#ifndef CLUSTERVIEWLISTWIDGET_HPP_
#define CLUSTERVIEWLISTWIDGET_HPP_

#include <QListWidget>

//#include "gui/ClusterViewWidget.hpp"

#include "clustering/ClusteringResult.hpp"
#include "clustering/DataStructs.hpp"
#include "clustering/CharCluster.hpp"

namespace impact_tr4 {

//class ClusterViewWidget;

class ImageCharListWidgetItem : public QListWidgetItem
{
//	Q_OBJECT
public:
	ImageCharListWidgetItem(const ImageCharListWidgetItem& other);
	ImageCharListWidgetItem(ImageChar *pImageChar, ImageCharIt it, QListWidget * parent = 0x0);
	virtual ~ImageCharListWidgetItem();

	ImageCharListWidgetItem* clone() const;

//	ImageChar mImageChar;
	ImageChar *mpImageChar;
	int mPosition;
	ImageCharIt mIt;
	QIcon mIcon;
};

class ClusterViewListWidget : public QListWidget
{
        Q_OBJECT

public:
	ClusterViewListWidget(CharClustIt clustIt, QWidget *parent = 0);
//	virtual ~ClusterViewListWidget();

public slots:
	void setCluster(CharClustIt clustIt);
	void updateList();
//        void addImage(QString filename);
//        void addImageChar(const ImageChar& imageChar);
//        void addImageChar(ImageChar *pImageChar);

	std::vector<ImageCharIt> getSelectedItemsIterators();

signals:
	void grabMouseEvents();
	void releaseMouseEvents();
//	void moveToTrashCluster(CharClustIt from, std::vector<ImageCharIt>& iterators);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);
private:
    CharClustIt mClustIt;
};

// Stream operators for ImageChar class:
#if 0
QDataStream &operator<<(QDataStream &out, const ImageChar &myObj);
QDataStream &operator>>(QDataStream &in, ImageChar &myObj);
#endif

} // end of namespace impact_tr4

// Declare metatype ImageChar so that it can be streamed by above stream operators:
//Q_DECLARE_METATYPE(impact_tr4::ImageChar);

#endif /* CLUSTERVIEWLISTWIDGET_HPP_ */
