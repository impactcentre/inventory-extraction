/*
 * ClusterViewWidget.hpp
 *
 *  Created on: 04.08.2009
 *      
 */

#ifndef CLUSTERVIEWWIDGET_HPP_
#define CLUSTERVIEWWIDGET_HPP_

#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

#include "ClusterViewListWidget.hpp"
#include "clustering/ClusteringResult.hpp"
#include "clustering/DataStructs.hpp"
#include "clustering/CharCluster.hpp"

namespace impact_tr4 {

class ClusteringResultViewWidget;

class ClusterViewWidget : public QWidget
{
	Q_OBJECT
public:
	ClusterViewWidget( CharClustIt clustIt, QWidget *parent = 0 );
//	virtual ~ClusterViewWidget();

//	int selectedClusterIndex() const;
public slots:
	void setCluster(CharClustIt clustIt);
	CharClustIt getClusterIterator() const;
	void clear();
//	void addImage();
//	void addImageChar(const ImageChar& imageChar);
//	void addImageChar(ImageChar *pImageChar);
//	std::vector<ImageChar*> getSelectedImageChars();
//	std::vector<int> getSelectedImageCharsPositions();
	std::vector<ImageCharIt> getSelectedImageCharsIterators();
//	void removeSelectedItems();
	void moveSelectedItemsToTrash();
//	void removeSelectedItems();
	void selectAllItems();
	void deselectAllItems();

	int nItems() const { return mpClusterViewListWidget->count(); }
	ImageChar& getImageChar(const int i) { return *(((ImageCharListWidgetItem *)mpClusterViewListWidget->item(i))->mpImageChar); }
	ImageChar* getImageCharPointer(const int i) { return (((ImageCharListWidgetItem *)mpClusterViewListWidget->item(i))->mpImageChar); }

//	void moveToButtonClicked();
//	void mergeWithButtonClicked();
	void removeButtonClicked();

protected:
//	void wheelEvent(QWheelEvent *event);
signals:
	void grabMouseEvents();
	void releaseMouseEvents();

	void updateClusterView();
//	void moveToCluster(int senderID, int receiverID, std::vector<std::list<ImageChar*>::iterator>& iterators);
//	void mergeWithCluster(int from, int to);
//	void removeCluster(int senderID);
	void removeCluster(CharClustIt clustIt);
	void moveToTrashCluster(CharClustIt from, std::vector<ImageCharIt>& iterators);

private:
	// MEMBERS:
	CharClustIt mClustIt;

	QLabel *mpClusterIDLabel;
	QPushButton *mpMoveToButton, *mpMergeWithButton, *mpRemoveButton;
	QLabel *charInfoLabel;
//	QComboBox *mpClusterSelectComboBox;
	QSpinBox *mpClusterSelectSpinBox;
	ClusterViewListWidget *mpClusterViewListWidget;

private slots:
	void displayCharInfo(QListWidgetItem *item);

signals:

}; // end of class ClusterViewWidget.hpp

} // end of namespace impact_tr4


#endif /* CLUSTERVIEWWIDGET_HPP_ */
