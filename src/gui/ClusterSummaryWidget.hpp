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
#ifndef CLUSTERSUMMARYWIDGET_H
#define CLUSTERSUMMARYWIDGET_H

#include "ui_ClusterSummaryWidget.h"

#include <QtGui/QWidget>
#include <QListWidget>

#include "clustering/CharCluster.hpp"
#include "clustering/ClusteringResult.hpp"
//#include "gui/ClusteringResultViewWidget.hpp"

namespace impact_tr4
{

class ClusteringResultViewWidget;

class ClusterSummaryWidget : public QWidget
{
    Q_OBJECT

public:
    ClusterSummaryWidget(CharClustIt clustIt, QWidget *parent = 0);
    ~ClusterSummaryWidget();

    CharClustIt clustIt() { return mClustIt; }

public slots:
	void reload();
	void updateClusterInfo();
	void updatePrototypeImage();

	// button slots:
	void on_unicodeLineEdit_textChanged();
	void on_infoTextLineEdit_textChanged();

	void on_showButton_clicked();
	void on_assignUnicodeButton_clicked();
	void on_computePrototypeButton_clicked();

signals:
	void showClusterViewWidget(CharClustIt n);
	void moveToCluster(CharClustIt from, CharClustIt to, std::vector<ImageCharIt>& iterators);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
//    void startDrag(Qt::DropActions supportedActions);


private:
    Ui::ClusterSummaryWidgetClass _ui;

    CharClustIt mClustIt;
    ClusteringResultViewWidget *mpClusteringResultViewWidget;
};

}

#endif // CLUSTERSUMMARYWIDGET_H
