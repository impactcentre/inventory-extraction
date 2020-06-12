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
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui>

#include "ImageViewer.hpp"
#include "ClusteringResultPrototypeViewWidget.hpp"
#include "ClassificationWidget.hpp"
#include "gui.h"
#include "clustering/ClusteringController.hpp"

namespace impact_tr4 {

// forward references:
class ClusteringController;

class InputFilesWidget;
class ClusterMethodWidget;
class FeatureVectorWidget;
class PreprocessingWidget;
class CentralWidget;
class ImageViewer;
class ClassificationWidget;

// class definition:
 class MainWindow : public QMainWindow
 {
     Q_OBJECT

 public:
	 MainWindow();

	 ClusteringController* getClusterControllerPointer() { return  &mClusterController; }

 public slots:
 	void imageLoaded(bool loaded);
 	void refreshInfoGroupBox();
 	void statusBarMessage(QString msg, int timeoutMs = 0)
		{ this->statusBar()->showMessage(msg, timeoutMs); }

 	bool hasClusteringResult() { return (!mClusterController.clusteringResultPointer()->isEmpty()); }

 private slots:
 	void computeAndStoreDataMatrix();
 	void close();
 	void print();
 	void saveAs();
 	void saveClusterResult();
 	void loadClusterResult();
 	void loadClusterResultImage();
 	void createClusterResultFromInputFiles();

 	void parseInputImages();
 	void parsingFinished(bool success);
 	void startClustering();
 	void clusteringFinished(bool success);

 	void preprocessImages();
 	void computeFeatures();
 	void quickClustering();

 	void quickFeatureVectorClustering();
// 	void batchProcessAll();
 	void setBusy(bool isBusy, QString busyText = QString("Busy..."));

 private:
	 void createDockAndTabWidgets();
	 void createZoomSlider();
     void createActions();
     void createMenus();
     void connectStuff();

     void scaleImage(double factor);
     void adjustScrollBar(QScrollBar *scrollBar, double factor);
     void displayClusterResult();

     static const QString WINDOW_TITLE_BASE;

     // The central widget and its components:
//     QWidget *centralWidget;
//     QSlider *mpZoomSlider;
//     QPushButton *mpDefaultZoomButton;

     // Actions:
//     QAction *openAct;
     QAction *closeAct;
     QAction *saveAsAct;
     QAction *saveClusterResultAct;
     QAction *loadClusterResultAct;
     QAction *loadClusterResultImageAct;
     QAction *printAct;
//     QAction *parseXMLAct;
     QAction *exitAct;

//     QAction *fitToWindowAct;
//     QAction *aboutAct;
//     QAction *aboutQtAct;

     // Menus:
     QMenu *fileMenu;
     QMenu *imageMenu;
     QMenu *clusteringMenu;
//     QMenu *viewMenu;
//     QMenu *helpMenu;

     // dockwidget and tabwidgets:
     QDockWidget *mpDockWidget;
     QWidget *mpLeftWidget;
     QTabWidget *mpControlTabWidget, *mpImageViewTabWidget/*, *mpClusteringResultTabWidget*/;

     // the widgets for the right tab-view area:
     ImageViewer *mpInputImageViewer, *mpOutputImageViewer;
//     CentralWidget *mpInputCentralWidget, *mpOutputCentralWidget;
//     ClusteringResultViewWidget *mpClusteringResultViewWidget, *mpGroundTruthViewWidget;
     ClusteringResultPrototypeViewWidget *mpClusteringResultPrototypeViewWidget;
     ClassificationWidget *mpClassificationWidget;

     int mIndexInputCentralWidget, mIndexOutputCentralWidget,
     	 mIndexClusteringResultTabWidget, mIndexGroundTruthViewWidget,
     	 mIndexClassificationWidget;

     // the widgets for the control area:
	 ClusterMethodWidget *mpClusterMethodWidget;
	 InputFilesWidget *mpInputFilesWidget;
	 FeatureVectorWidget *mpFeatureVectorWidget;
	 PreprocessingWidget *mpPreprocessingWidget;

	 int mIndexClusterMethodWidget, mIndexInputFilesWidget, mIndexPreprocessingWidget, mIndexFeatureVectorWidget;

	 QPushButton *mpComputeAndStoreDataMatrixButton, *mpStartClusteringButton;
	 QPushButton *mpParseInputButton, *mpPreprocessImageButton, *mpComputeFeaturesButton;
	 QPushButton *mpQuickClusteringButton, *mpQuickFeatureVectorClusteringButton;

	 QGroupBox *mpInfoGroupBox;
	 QLabel *mpParsingResultsInfo, *mpPreprocessingResultsInfo, *mpFeaturesResultsInfo,*mpClusteringResultsInfo;
	 QProgressBar *mpProgressBar;
	 QProgressDialog *mpProgressDialog;

	 QScrollArea *mpRightScrollArea;
	 QScrollArea *mpLeftScrollArea;


	 QLabel *dummyLabel;

	 // cluster controller:
	 ClusteringController mClusterController;

	 // StopWatch's:
	 StopWatch watchParsing;
	 StopWatch watchClustering;
	 StopWatch watch;
 };
} // end of namespace impact_tr4
#endif /*MAINWINDOW_H_*/

