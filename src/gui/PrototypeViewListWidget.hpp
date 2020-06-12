/*
 * PrototypeViewListWidget.hpp
 *
 *  Created on: 20.07.2010
 *      
 */


#ifndef PROTOTYPEVIEWLISTWIDGET_HPP_
#define PROTOTYPEVIEWLISTWIDGET_HPP_

#include <QListWidget>

#include "clustering/ClusteringResult.hpp"
#include "clustering/DataStructs.hpp"
#include "clustering/CharCluster.hpp"

namespace impact_tr4 {

//class ClusterViewWidget;

class ClusterListWidgetItem : public QListWidgetItem
{
//	Q_OBJECT
public:
	ClusterListWidgetItem(const ClusterListWidgetItem& other);
	ClusterListWidgetItem(CharClustIt clustIt, QListWidget * parent = 0x0);
	virtual ~ClusterListWidgetItem();

	ClusterListWidgetItem* clone() const;

	void updatePrototypeImage();
	void updateLabel();


	CharClustIt mClustIt;

	QIcon mIcon;
};

class PrototypeViewListWidget : public QListWidget
{
        Q_OBJECT

public:
	PrototypeViewListWidget(ClusteringResult *pClusteringResult, QWidget *parent = 0);
	int findClusterItemIndex(CharClustIt clustIt);

public slots:
	void setClusteringResult(ClusteringResult *pClusteringResult);
	void updateList(int sizeThreshLow=-1, int sizeThreshHigh=-1);
	void updateListWithOrdering(const std::vector<CharClustIt> &ordering, int sizeThreshLow, int sizeThreshHigh);
	void addEmptyCluster();
	void updatePrototype(int i);
	void updateLabel(int i);
	void on_itemDoubleClicked(QListWidgetItem  * item);
	void updateIconSize(int size);

signals:
//	void itemDoubleClicked (QListWidgetItem  * item);
	void showClusterViewWidget(CharClustIt clustIt);

	void moveToCluster(CharClustIt from, CharClustIt to, std::vector<ImageCharIt>& iterators);
	void mergeWithCluster(CharClustIt from, CharClustIt to);
	void mergeClusters(std::vector<CharClustIt> from, CharClustIt to);

	void grabMouseEvents();
	void releaseMouseEvents();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);
private:
	ClusteringResult *mpClusteringResult;
}; // end class PrototypeViewListWidget


} // end of namespace impact_tr4


#endif /* PROTOTYPEVIEWLISTWIDGET_HPP_ */
