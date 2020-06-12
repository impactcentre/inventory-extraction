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

//#include <boost/bind.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
//#include <boost/shared_ptr.hpp>

#include "MainWindow.hpp"

#include "CentralWidget.hpp"
#include "ClusterMethodWidget.hpp"
#include "InputFilesWidget.hpp"
#include "FeatureVectorWidget.hpp"
#include "PreprocessingWidget.hpp"

#include "clustering/ClusteringController.hpp"
#include "clustering/data_reader/DatFileDataReader.hpp"

#include "xml/ClusteringResultXMLWriter.hpp"
#include "xml/ClusteringResultOutputHandler.hpp"

namespace impact_tr4 {

/*static*/ const QString MainWindow::WINDOW_TITLE_BASE = "IMPACT Inventory-Extraction-Tool v0.2";

 MainWindow::MainWindow()
 {
	 // FIXME: indices needed?
	 mIndexInputFilesWidget = mIndexClusterMethodWidget
	 	= mIndexFeatureVectorWidget = mIndexGroundTruthViewWidget = mIndexPreprocessingWidget = -1;
	 mIndexInputCentralWidget = mIndexOutputCentralWidget = mIndexClusteringResultTabWidget = mIndexClassificationWidget = -1;


	 // create widgets:
	 mpInputImageViewer = new ImageViewer(this);
	 mpOutputImageViewer = new ImageViewer(this);
//	 mpClusteringResultViewWidget = new ClusteringResultViewWidget(this);
	 mpClusteringResultPrototypeViewWidget = new ClusteringResultPrototypeViewWidget(this);
	 connect(mpClusteringResultPrototypeViewWidget, SIGNAL(refreshInfoGroupBox()), this, SLOT(refreshInfoGroupBox()));
//	 mpGroundTruthViewWidget = new ClusteringResultViewWidget(this);
	 mpClassificationWidget = new ClassificationWidget(&mClusterController, this);

	 createDockAndTabWidgets();
	 mpImageViewTabWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	 mpRightScrollArea = new QScrollArea;
	 mpRightScrollArea->setWidget(mpImageViewTabWidget);
	 mpRightScrollArea->setWidgetResizable(true);
	 setCentralWidget(mpRightScrollArea);
//	 setCentralWidget(mpImageViewTabWidget);

     createActions();
     createMenus();
     connectStuff();
     setWindowTitle(WINDOW_TITLE_BASE);
     resize(1200, 800);

     imageLoaded(false);
     refreshInfoGroupBox();

     statusBar()->showMessage(tr("Ready"));
 } // end MainWindow

 void MainWindow::createActions()
 {
	 // open:
//     openAct = new QAction(tr("&Open..."), this);
//     openAct->setShortcut(tr("Ctrl+O"));
//     connect(openAct, SIGNAL(triggered()), mpInputImageViewer, SLOT(open()));
     // close:
     closeAct = new QAction(tr("&Close..."), this);
     closeAct->setShortcut(tr("Ctrl+C"));
     connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));
     closeAct->setEnabled(false);
     // save as:
     saveAsAct = new QAction(tr("&Save As..."), this);
     saveAsAct->setShortcut(tr("Ctrl+S"));
     connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
     saveAsAct->setEnabled(false);
     // save cluster resuls:
     saveClusterResultAct = new QAction(tr("Save &Cluster Result..."), this);
     saveClusterResultAct->setShortcut(tr("Ctrl+C"));
     connect(saveClusterResultAct, SIGNAL(triggered()), this, SLOT(saveClusterResult()));
     saveClusterResultAct->setEnabled(true);
     // load cluster resuls:
     loadClusterResultAct = new QAction(tr("&Load Cluster Result..."), this);
     loadClusterResultAct->setShortcut(tr("Ctrl+L"));
     connect(loadClusterResultAct, SIGNAL(triggered()), this, SLOT(loadClusterResult()));
     loadClusterResultAct->setEnabled(true);
     // load cluster result image:
     loadClusterResultImageAct = new QAction(tr("Load Cluster Result Image... (for smaller clusterings only!)"), this);
     connect(loadClusterResultImageAct, SIGNAL(triggered()), this, SLOT(loadClusterResultImage()));
     loadClusterResultImageAct->setEnabled(true);
     // print:
     printAct = new QAction(tr("&Print..."), this);
     printAct->setShortcut(tr("Ctrl+P"));
     printAct->setEnabled(false);
     connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
     // exit:
     exitAct = new QAction(tr("E&xit"), this);
     exitAct->setShortcut(tr("Ctrl+Q"));
     connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

//     aboutAct = new QAction(tr("&About"), this);
//     connect(aboutAct, SIGNAL(triggered()), mpImageViewer, SLOT(about()));

//     aboutQtAct = new QAction(tr("About &Qt"), this);
//     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
 }

 void MainWindow::createMenus()
 {
     fileMenu = new QMenu(tr("File"), this);
//     fileMenu->addAction(openAct);
//     fileMenu->addSeparator();
     fileMenu->addAction(exitAct);

     imageMenu = new QMenu(tr("Image"), this);
     imageMenu->addAction(closeAct);
     imageMenu->addAction(saveAsAct);
     imageMenu->addAction(printAct);

     clusteringMenu = new QMenu(tr("Clustering"), this);
     clusteringMenu->addAction(saveClusterResultAct);
     clusteringMenu->addAction(loadClusterResultAct);
     clusteringMenu->addAction(loadClusterResultImageAct);

//     viewMenu = new QMenu(tr("&View"), this);
//     viewMenu->addAction(zoomInAct);
//     viewMenu->addAction(zoomOutAct);
//     viewMenu->addAction(normalSizeAct);
//     viewMenu->addSeparator();
//     viewMenu->addAction(fitToWindowAct);

//     helpMenu = new QMenu(tr("&Help"), this);
//     helpMenu->addAction(aboutAct);
//     helpMenu->addAction(aboutQtAct);

     menuBar()->addMenu(fileMenu);
     menuBar()->addMenu(imageMenu);
     menuBar()->addMenu(clusteringMenu);
//     menuBar()->addMenu(viewMenu);
//     menuBar()->addMenu(helpMenu);
 }

 void MainWindow::createDockAndTabWidgets()
 {
	 // create a dock widget for the control tab widget:
	 mpDockWidget = new QDockWidget( this );
	 mpDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	 mpDockWidget->setFeatures( QDockWidget::DockWidgetMovable );

	 // create the control tab widget and attach it to the dock widget:
//	 mpClusterProgressBar = new QProgressBar( this );
//	 mpClusterProgressBar->setRange(0, 1);
//	 mpClusterProgressBar->setTextVisible(false);
//	 mpClusterProgressBar->setValue(0);
//	 mpClusterProgressBar->setEnabled( false );
//	 mpClusterProgressBar->reset();
	 mpControlTabWidget = new QTabWidget( this );

	 mpStartClusteringButton = new QPushButton("Start Clustering");

	 mpComputeAndStoreDataMatrixButton = new QPushButton("Compute and Store Data Matrix");
	 connect( mpComputeAndStoreDataMatrixButton, SIGNAL( clicked() ), this, SLOT( computeAndStoreDataMatrix()) );

	 mpQuickClusteringButton = new QPushButton("Quick Clustering");
	 connect( mpQuickClusteringButton, SIGNAL( clicked() ), this, SLOT( quickClustering()) );
	 mpQuickClusteringButton->setToolTip(tr("Performs the chain from loading segmentation results, preprocessing images and clustering using an image based distance function."));
	 mpQuickClusteringButton->setVisible(false);

	 mpQuickFeatureVectorClusteringButton = new QPushButton("Quick Clustering using Feature Vectors");
	 connect( mpQuickFeatureVectorClusteringButton, SIGNAL( clicked() ), this, SLOT( quickFeatureVectorClustering()) );
	 mpQuickFeatureVectorClusteringButton->setToolTip(tr("Performs the chain from loading segmentation results, computing featureas and clustering based on those features"));
	 mpQuickFeatureVectorClusteringButton->setVisible(false);

	 QVBoxLayout *pVBoxLayout = new QVBoxLayout();
	 pVBoxLayout->addWidget(mpControlTabWidget);
	 mpInfoGroupBox = new QGroupBox("Data Info", this);
	 QFont infoGroupBoxFont("Arial", 12, QFont::Bold);
	 QFont infoFont("Arial", 11, QFont::Bold);
	 mpInfoGroupBox->setFont(infoGroupBoxFont);
	 QVBoxLayout *pVGroupBoxLayout = new QVBoxLayout;
	 mpParsingResultsInfo = new QLabel("Nr. of parsed glyphs = 0, Nr. of pages = 0");
	 mpParsingResultsInfo->setFont(infoFont);
	 mpPreprocessingResultsInfo = new QLabel("Nr. of prepocessed glyphs = 0, size = 0 x 0");
	 mpPreprocessingResultsInfo->setFont(infoFont);
	 mpPreprocessingResultsInfo->setVisible(false);
	 mpFeaturesResultsInfo = new QLabel("Nr. of feature vectors = 0, dimension = 0");
	 mpFeaturesResultsInfo->setFont(infoFont);
	 mpFeaturesResultsInfo->setVisible(false);
	 mpClusteringResultsInfo = new QLabel("No clustering available");
	 mpClusteringResultsInfo->setFont(infoFont);

	 pVGroupBoxLayout->addWidget(mpParsingResultsInfo);
	 pVGroupBoxLayout->addWidget(mpPreprocessingResultsInfo);
	 pVGroupBoxLayout->addWidget(mpFeaturesResultsInfo);
	 pVGroupBoxLayout->addWidget(mpClusteringResultsInfo);
	 mpInfoGroupBox->setLayout(pVGroupBoxLayout);

	 mpParseInputButton = new QPushButton("Parse Input Data");
//	 mpParseInputButton->setFixedSize(mpParseInputButton->sizeHint());
	 mpPreprocessImageButton = new QPushButton("Preprocess Parsed Data");
	 mpPreprocessImageButton->setVisible(false);
	 mpComputeFeaturesButton = new QPushButton("Compute Features");
	 mpComputeFeaturesButton->setFixedSize(mpComputeFeaturesButton->sizeHint());
	 mpComputeFeaturesButton->setVisible(false);
//	 mpProgressBar = new QProgressBar(this);
	 mpProgressDialog = NULL;

	 pVBoxLayout->addWidget(mpInfoGroupBox);
//	 pVBoxLayout->addWidget(mpComputeAndStoreDataMatrixButton);
	 pVBoxLayout->addWidget(mpParseInputButton);
	 pVBoxLayout->addWidget(mpPreprocessImageButton);
	 pVBoxLayout->addWidget(mpComputeFeaturesButton);
	 pVBoxLayout->addWidget(mpStartClusteringButton);
	 pVBoxLayout->addWidget(mpQuickClusteringButton);
	 pVBoxLayout->addWidget(mpQuickFeatureVectorClusteringButton);
//	 pVBoxLayout->addWidget(mpProgressBar);

	 // create the image view tab widget:
	 mpImageViewTabWidget = new QTabWidget( this );
//	 mpClusteringResultTabWidget = new QTabWidget( this );
//	 mpClusteringResultTabWidget->addTab(mpClusteringResultViewWidget, "Cluster Result List View");
//	 mpClusteringResultTabWidget->addTab(mpClusteringResultPrototypeViewWidget, "Cluster Result Prototype View");

	 // create widget for input files:
	 mpInputFilesWidget = new InputFilesWidget(this);
	 connect( mpInputFilesWidget, SIGNAL( openImage(QString) ), mpInputImageViewer, SLOT( open(QString) ) );
	 // create widget for cluster method selection:
	 mpClusterMethodWidget = new ClusterMethodWidget(this);
	 // create widget for feature vector:
	 mpFeatureVectorWidget = new FeatureVectorWidget(mClusterController.documentPointer()->getCharFeatureCollectionPointer(), this);
	 // create widget for preprocessing:
	 mpPreprocessingWidget = new PreprocessingWidget(this);

	 // add widgets to control tab widget:
	 mIndexInputFilesWidget = mpControlTabWidget->addTab( mpInputFilesWidget, "Input Images" ); // index 0
	 mIndexPreprocessingWidget = mpControlTabWidget->addTab( mpPreprocessingWidget, "Preprocessing"); // index 1
	 mIndexFeatureVectorWidget = mpControlTabWidget->addTab( mpFeatureVectorWidget, "Feature Vector" ); // index 2
	 mIndexClusterMethodWidget = mpControlTabWidget->addTab( mpClusterMethodWidget, "Cluster Method" ); // index 3
//	 std::cout << mIndexInputFilesWidget << ", " << mIndexClusterMethodWidget << ", " << mIndexFeatureVectorWidget << std::endl;

	 // create left widget and scrollarea:
	 mpLeftWidget = new QWidget(this);
	 mpLeftWidget->setLayout(pVBoxLayout);
	 mpLeftWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	 mpLeftScrollArea = new QScrollArea(this);
	 mpLeftScrollArea->setWidget(mpLeftWidget);
//	 mpLeftScrollArea->setWidgetResizable(true);
//	 mpDockWidget->setWidget( mpLeftWidget );
	 mpDockWidget->setWidget( mpLeftScrollArea );
//	 mpLeftScrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
//	 mpLeftScrollArea->setMinimumWidth(500);
//	 mpLeftScrollArea->resize(mpLeftScrollArea->sizeHint());

	 // add widgets to image viewer tab widget:
	 mIndexInputCentralWidget = mpImageViewTabWidget->addTab(mpInputImageViewer, "Input Image");
//	 mIndexClusteringResultTabWidget = mpImageViewTabWidget->addTab(mpClusteringResultTabWidget, "Clustering Result");
	 mIndexClusteringResultTabWidget = mpImageViewTabWidget->addTab(mpClusteringResultPrototypeViewWidget, "Clustering Result");
//	 mIndexOutputCentralWidget = mpImageViewTabWidget->addTab(mpOutputImageViewer, "Cluster Result Image");
	 mIndexClassificationWidget = mpImageViewTabWidget->addTab(mpClassificationWidget, "Training and Classification");
//	 mIndexGroundTruthViewWidget = mpImageViewTabWidget->addTab(mpGroundTruthViewWidget, "Ground Truth");

	 // add dock widget to the main widget:
	 this->addDockWidget( Qt::LeftDockWidgetArea, mpDockWidget );

	 return;
 } // end createDockAndTabWidget()

 void MainWindow::connectStuff()
 {
	 // mpInputImageViewer:
	 connect(mpInputImageViewer, SIGNAL(statusBarMessage(QString, int)), this, SLOT(statusBarMessage(QString, int)));
	 connect(mpInputImageViewer, SIGNAL(imageLoaded(bool)), this, SLOT(imageLoaded(bool)));
     connect(mpInputFilesWidget, SIGNAL(showSegmentation(ParsingParameters, bool)), mpInputImageViewer, SLOT(showSegmentation(ParsingParameters, bool)));
     connect(mpInputFilesWidget, SIGNAL(createClusterResult()), this, SLOT(createClusterResultFromInputFiles()));

     // mpOutputImageViewer:
     connect(mpOutputImageViewer, SIGNAL(statusBarMessage(QString, int)), this, SLOT(statusBarMessage(QString, int)));
	 connect(mpOutputImageViewer, SIGNAL(imageLoaded(bool)), this, SLOT(imageLoaded(bool)));

	 // buttons:
	 connect(mpParseInputButton, SIGNAL(clicked()), this, SLOT(parseInputImages()));
	 connect(mpStartClusteringButton, SIGNAL( clicked() ), this, SLOT( startClustering()) );
	 connect(mpPreprocessImageButton, SIGNAL(clicked()), this, SLOT(preprocessImages()));
	 connect(mpComputeFeaturesButton, SIGNAL(clicked()), this, SLOT(computeFeatures()));

	 // clustering-controller:
	 connect(&mClusterController, SIGNAL(parsingFinished(bool)), this, SLOT(parsingFinished(bool)));
	 connect(&mClusterController, SIGNAL(clusteringFinished(bool)), this, SLOT(clusteringFinished(bool)));
 }

 void MainWindow::refreshInfoGroupBox()
 {
	 // retrieve document pointer:
	Document *pDocument = mClusterController.documentPointer();
	// set parsed image info text:
	const int nParsedImages = pDocument->nParsedImages();
	const int nParsedPages = pDocument->nFiles();
	mpParsingResultsInfo->setText("Nr. of parsed glyphs = "+QString::number(nParsedImages)+" in " + QString::number(nParsedPages) + " loaded pages.");
#if 0
	// set parsing result info text:
	const int nProcessedImages = pDocument->nProcessedImages();
	int widthProcessedIms = 0; int heightProcessedIms = 0;
	if (nProcessedImages > 0) {
		widthProcessedIms = (*pDocument->getProcessedImageVecPointer())[0]->width();
		heightProcessedIms = (*pDocument->getProcessedImageVecPointer())[0]->height();
	}
	mpPreprocessingResultsInfo->setText("Nr. of prepocessed glyphs = "+QString::number(nProcessedImages)+", Size = "+QString::number(widthProcessedIms) + ", " + QString::number(heightProcessedIms) );
#endif
#if 0
	// set features info text:
	int nFeatVecs = 0;
	int nDims = 0;
	CharFeatureCollection* pCharFeatureCollection = pDocument->getCharFeatureCollectionPointer();
	if (pCharFeatureCollection != NULL) {
		nFeatVecs = pCharFeatureCollection->dataMatrixRef().size1();
		nDims = pCharFeatureCollection->dataMatrixRef().size2();
	}
	mpFeaturesResultsInfo->setText("Nr. of feature vectors = "+QString::number(nFeatVecs)+ ", dimension = "+QString::number(nDims));
#endif
	// set clustering result info text:
	ClusteringResult *pClusterResult = mClusterController.clusteringResultPointer();
	const int nClusts = pClusterResult->nClusters();
	if (nClusts <= 0) {
		mpClusteringResultsInfo->setText("No clustering available");
	}
	else {
		const int nTotalElements = pClusterResult->nTotalElements();
		QString clusteringInfo;
		clusteringInfo += "Nr. of clusters: "+QString::number(nClusts) +", nr of samples: "+QString::number(nTotalElements);
		mpClusteringResultsInfo->setText(clusteringInfo);
	}

	return;
 }

 void MainWindow::imageLoaded(bool loaded)
 {
     closeAct->setEnabled(loaded);
     saveAsAct->setEnabled(loaded);
     printAct->setEnabled(loaded);

     if (loaded) setWindowTitle(WINDOW_TITLE_BASE + " -  " + mpInputImageViewer->fileName());
     else setWindowTitle(WINDOW_TITLE_BASE);
 } // end of imageLoaded

 void MainWindow::computeAndStoreDataMatrix()
 { // FIXME: obsolete????
	 std::cout << "MainWindow: compute and store data matrix..." << std::endl;
	 std::vector<std::string> inputFileList = mpInputFilesWidget->inputFileList();
	 try {
		QString fn = QFileDialog::getSaveFileName ( this, "Specify filename", QDir::currentPath(), ".txt" );
		if (fn.isEmpty()) return;
		if (!fn.endsWith(".txt")) { fn.append(".txt"); }

	    QProgressDialog progress("Storing feature matrix...", "", 0, 2, this);
	    progress.setCancelButton(0x0);
	    progress.setWindowModality(Qt::ApplicationModal);
	    progress.show();
	    progress.setValue(0);

		StopWatch watch;
		watch.start();

		mClusterController.parseInputFiles(	mpInputFilesWidget->parsingParameters(),
											mpInputFilesWidget->inputFileList());
		mClusterController.createFeatures(	mpPreprocessingWidget->parameters(),
											mpFeatureVectorWidget->dimReductionParameters() );

		double timeClustering = watch.stop();
//		std::cout << "successfully created clustering controller!" << std::endl;
		progress.setValue(1);
		mClusterController.storeDataMatrix( fn.toStdString() );
		progress.setValue(2);
		progress.reset();
		// display result image:
		QMessageBox::information(this, "Success", "Computed data matrix successfully!\nElapsed time: "+QString::number(timeClustering)+" seconds");
	 }
	 catch (Exception e) {
		 std::cout << "An exception has occured in MainWindow::computeAndStoreDataMatrix: " << e.what() << std::endl;
		 QMessageBox::critical(this,"An exception has occured in MainWindow::computeAndStoreDataMatrix", e.what() );
	 }
//	 mpClusterProgressBar->setEnabled(false);
//	 mpClusterProgressBar->setRange(0, 1);

	 return;
 } // end computeAndStoreDataMatrix()

 void MainWindow::parseInputImages()
 {
	 if (hasClusteringResult())
	 if (QMessageBox::question(this, "Clearing clustering result", "Current clustering result will be cleared - proceed?", QMessageBox::Yes | QMessageBox::No)==QMessageBox::No) {
		 return;
	 }

	 try {
//	 std::cout << "starting parsing in MainWindow!" << std::endl;
	 const int nInputFiles = mpInputFilesWidget->inputFileList().size();
	 if (nInputFiles > 0) {
		 this->setBusy(true, "Parsing input files...");
		 mClusterController.clear();
#if 0 // use threaded version
		 boost::thread thread(boost::bind(	&ClusteringController::parseInputFiles,
				 	 	 	 	 	 	 	 &mClusterController,
				 	 	 	 	 	 	 	 mpInputFilesWidget->parsingParameters(),
				 	 	 	 	 	 	 	 mpInputFilesWidget->inputFileList()));
#else // use non-threaded version
		 mClusterController.parseInputFiles(mpInputFilesWidget->parsingParameters(),
									mpInputFilesWidget->inputFileList());
#endif
	 }
	 else {
		 QMessageBox::critical(this,"No Input Data", "No files specified as input!" );
	 }

	 } // end try
	 catch (Exception e) {
		 std::cout << "An exception has occurred in MainWindow::parseInputImages: " << e.what() << std::endl;
		 QMessageBox::critical(this,"An error has occurred while parsing the images.", e.what() );
		 this->setBusy(false);
	 }

	return;
 } // end preprocessImages

 void MainWindow::parsingFinished(bool success)
 {
	 if (success) {
		 try {
			 this->displayClusterResult();
		 }
		 catch (Exception e) {
			 std::cout << "An exception has occurred in MainWindow::parsingFinished: " << e.what() << std::endl;
			 QMessageBox::critical(this,"An error has occurred while parsing the images.", e.what() );
			 this->setBusy(false);
		 }
		 this->setBusy(false);
	 }
	 else {
		 this->mClusterController.documentPointer()->clearDocumentInputData();
		 this->setBusy(false);
	 }
	 this->refreshInfoGroupBox();
	 return;
 } // end parsingFinished()

 void MainWindow::preprocessImages()
 {
	 try {
	//	 std::cout << "starting preprocessing in MainWindow!" << std::endl;
		const int nParsedImages = mClusterController.documentPointer()->nParsedImages();
		if (nParsedImages > 0) {
			mClusterController.preprocessGlyphs(mpPreprocessingWidget->parameters());
		}
		else {
			QMessageBox::critical(this,"No Input Data", "No parsing results are available!" );
		}

		this->refreshInfoGroupBox();
	 } // end try
	 catch (Exception e) {
		 std::cout << "An exception has occurred in MainWindow::preprocessImages: " << e.what() << std::endl;
		 QMessageBox::critical(this,"An exception has occurred while preprocessing the images", e.what() );
	 }

	return;
 } // end preprocessImages

 void MainWindow::computeFeatures()
 {
	try {
		StopWatch watch;
		mClusterController.createFeatures(	mpPreprocessingWidget->parameters(),
											mpFeatureVectorWidget->dimReductionParameters()	);
		watch.stop();
		this->refreshInfoGroupBox();
	} // end try
	catch (Exception e) {
		 std::cerr << "An exception has occurred in MainWindow::computeFeatures(): " << e.what() << std::endl;
		 QMessageBox::critical(this,"An error has occurred while computing the features", e.what() );
	}
 } // end computeFeatures()

 void MainWindow::startClustering()
 {
	 std::cout << "MainWindow: starting clustering..." << std::endl;
	 std::vector<std::string> inputFileList = mpInputFilesWidget->inputFileList();
	 try {
		// first: set preprocessing parameters in document:
		PreprocessingParameters pars = mpPreprocessingWidget->parameters();
		pars.print();

		this->setBusy(true, "Clustering...");

	    watchClustering.start();

#if 0 // use threaded version
		 boost::thread thread(boost::bind(	&ClusteringController::clusterGlyphs,
				 	 	 	 	 	 	 	&mClusterController,
				 	 	 	 	 	 	 	mpPreprocessingWidget->parameters(),
											mpFeatureVectorWidget->dimReductionParameters(),
											mpClusterMethodWidget->clusterMethodType(),
											mpClusterMethodWidget->parameters()	));
#else // use non-threaded version
			mClusterController.clusterGlyphs(	mpPreprocessingWidget->parameters(),
												mpFeatureVectorWidget->dimReductionParameters(),
												mpClusterMethodWidget->clusterMethodType(),
												mpClusterMethodWidget->parameters()	);
#endif
	 }
	 catch (Exception e) {
		 std::cerr << "An exception has occured in MainWindow::startClustering: " << e.what() << std::endl;
		 QMessageBox::critical(this,"An error has occured while clustering!", e.what() );
		 this->setBusy(false);
	 }

	 return;
 }

 void MainWindow::clusteringFinished(bool success)
 {
	 if (success) {
	 try {
			double timeClustering = watchClustering.stop();
			int minutes = timeClustering / 60;
			double seconds = timeClustering-minutes;
			QString timeStr = QString::number(minutes)+" min., "+QString::number(seconds)+" secs.";

#if 0
			// display result image:
			if ( mClusterController.hasImage() ) {
				mpOutputImageViewer->open( &mClusterController.resultImage() );
			}
#endif

			this->refreshInfoGroupBox();
			this->displayClusterResult();
			this->setBusy(false);

			QMessageBox::information(this, "Success", "Finished clustering successfully!\nElapsed time: "+timeStr);
	 }
	 catch (Exception e) {
		 std::cerr << "An exception has occured in MainWindow::clusteringFinished: " << e.what() << std::endl;
		 QMessageBox::critical(this,"An error has occured while clustering!", e.what() );
		 this->setBusy(false);
	 }
	 } // end if success
	 else {
		 mClusterController.clusteringResultPointer()->deleteClustering();
		 this->setBusy(false);
	 }
	 return;
 }

 void MainWindow::quickClustering()
 {
	 this->parseInputImages();
	 this->preprocessImages();
	 this->startClustering();

	 return;
 }

 void MainWindow::quickFeatureVectorClustering()
 {
	 this->parseInputImages();
	 this->preprocessImages();
	 this->startClustering();

	 return;
 }

 void MainWindow::close()
 {
	 const int index = mpImageViewTabWidget->currentIndex();
	 if (index == mIndexInputCentralWidget)
		 mpInputImageViewer->close();
	 else if (index == mIndexOutputCentralWidget)
		 mpOutputImageViewer->close();

	 return;
 } // end print()

 void MainWindow::print()
 {
	 const int index = mpImageViewTabWidget->currentIndex();
	 if (index == mIndexInputCentralWidget)
		 mpInputImageViewer->print();
	 else if (index == mIndexOutputCentralWidget)
		 mpOutputImageViewer->print();

	 return;
 } // end print()

 void MainWindow::saveAs()
 {
	 const int index = mpImageViewTabWidget->currentIndex();
	 if (index == mIndexInputCentralWidget)
		 mpInputImageViewer->saveAs();
	 else if (index == mIndexOutputCentralWidget)
		 mpOutputImageViewer->saveAs();

	 return;
 } // end print()

 void MainWindow::loadClusterResult()
 {
	 if (hasClusteringResult())
	 if (QMessageBox::question(this, "Clearing clustering result", "Current clustering result will be cleared - proceed?", QMessageBox::Yes | QMessageBox::No)==QMessageBox::No) {
		 return;
	 }

	 try {
		QString xmlFileName =QFileDialog::getOpenFileName(this, tr("Load Cluster Result XML file"),
													QDir::currentPath(),
													tr("Cluster Result XML Files (*.xml)"));
		if (xmlFileName.isEmpty())
			return;

		ClusteringResultOutputHandler *pXMLHandler = new ClusteringResultOutputHandler(mClusterController.documentPointer(), mClusterController.clusteringResultPointer());
		// create simple reader and set the content/error handler
		QXmlSimpleReader reader;
		reader.setContentHandler(pXMLHandler);
		reader.setErrorHandler(pXMLHandler);
		// create QFile object
		QFile file(xmlFileName);
		if (!file.open(QFile::ReadOnly | QFile::Text)) {
			throw Exception("Error openening xml-file while loading clustering result!");
		}
		// create xml input source from file object
		QXmlInputSource xmlInputSource(&file);
		// parse the file using the simple reader
		if (!reader.parse(xmlInputSource)) {
			throw Exception("Parsing error occured while loading cluster result!");
		}

		// Display cluster result:
		this->displayClusterResult();
		// load document into input files widget:
		mpInputFilesWidget->setFileList(mClusterController.documentPointer()->fileNameVec());

		delete pXMLHandler;

		refreshInfoGroupBox();
		QMessageBox::information(this,"Success!", "Successfully loaded cluster result." );
	 } // end try
	 catch (Exception e) {
		 std::cout << "An exception has occured in MainWindow::loadClusterResult(): " << e.what() << std::endl;
		 QMessageBox::critical(this,"Error while loading cluster result", e.what() );
	 } // end catch
	 return;
 } // end loadClusterResult()

 void MainWindow::loadClusterResultImage()
 {
	 if (this->hasClusteringResult()) {
		 try {
			 GrayImage<> outputImage;
			 mClusterController.createClusterImage(outputImage);
			 if (!outputImage.empty()) {
//				 mpOutputImageViewer->open( &outputImage );

				 ImageViewer *pClusteringImageViewer = new ImageViewer();
				 pClusteringImageViewer->resize(800, 1000);
				 pClusteringImageViewer->open(&outputImage);
				 pClusteringImageViewer->show();
			 }
		} // end try
		catch (Exception e) {
			 std::cout << "An exception has occured in MainWindow::saveClusterResult(): " << e.what() << std::endl;
			 QMessageBox::critical(this,"Error saving cluster result!", e.what() );
		}
	 } // end if
	 else {
		 QMessageBox::information(this, "No Cluster Result", "No cluster result available!");
	 }
	 return;
 }

 void MainWindow::createClusterResultFromInputFiles()
 {
	 // TODO: FIXME (out of date??) and: move to model (MVC...)
	 std::cerr << "createClusterResultFromInputFiles() to be implemented for new clusteringresult class!" << std::endl;
#if 0
 try {
	const int nParsedImages = mClusterController.documentPointer()->nParsedImages();
	if (nParsedImages > 0) {
		ClusteringResult *pClusteringResult = mClusterController.clusteringResultPointer();
		pClusteringResult->deleteClustering(); // delete probably old clustering result				std::cout << "hier" << std::endl;
		Document *pDocument = mClusterController.documentPointer();
		std::vector<std::string> clusterPrototypesStr;
		bool clusterFound = false;
		std::vector<ImageChar*>& imageCharsVecRef = pDocument->getImageCharsVecRef();
		for (int i=0; i<nParsedImages; ++i) {
			std::string actCharStr = imageCharsVecRef[i]->text;
			clusterFound = false;
			for (int j=0; j<clusterPrototypesStr.size(); ++j) {
				if (actCharStr.compare(clusterPrototypesStr[j]) == 0) {
					clusterFound = true;
					(*pClusteringResult)[j]->addChar(imageCharsVecRef[i]);
				}
			} // end j
			if (!clusterFound) {// no cluster found --> create new cluster and add character
				clusterPrototypesStr.push_back(actCharStr);
				pClusteringResult->addEmptyCluster(pDocument);
				(*pClusteringResult)[pClusteringResult->nClusters()-1]->addChar(imageCharsVecRef[i]);
			}
		} // end i
		this->displayClusterResult();
	}
	else {
		QMessageBox::critical(this,"No Input Data", "No parsing results are available!" );
	}
#if 0
		 } // end if (!inputFileList.empty())
#endif
	 refreshInfoGroupBox();
	 QMessageBox::information(this,"Success!", "Successfully created cluster result from OCR result file." );
 } // end try
 catch (Exception e) {
	 std::cout << "An exception has occured in MainWindow::createClusterResultFromInputFiles: " << e.what() << std::endl;
	 QMessageBox::critical(this,"An exception has occured in MainWindow::createClusterResultFromInputFiles", e.what() );
 }
#endif
 return;
 } // end createClusterResultFromInputFiles()

 void MainWindow::displayClusterResult()
 {
	// Display cluster result:
	// FIXME (?): workaround, s.t. clustering results are displayed properly...
	int currentIndex = mpImageViewTabWidget->currentIndex();
	mpImageViewTabWidget->removeTab(mIndexClusteringResultTabWidget);
//	printf("%x\n", mClusterController.clusteringResultPointer());
//	mpClusteringResultViewWidget->openClusteringResult(mClusterController.clusteringResultPointer(), mClusterController.documentPointer());
	mpClusteringResultPrototypeViewWidget->openClusteringResult(mClusterController.clusteringResultPointer(), mClusterController.documentPointer());
	// TODO: open clustering result in mpClusteringResultPrototypeViewWidget
//	mIndexClusteringResultTabWidget = mpImageViewTabWidget->addTab(mpClusteringResultViewWidget, "Clustering Result");
//	mIndexClusteringResultTabWidget = mpImageViewTabWidget->insertTab(mIndexClusteringResultTabWidget, mpClusteringResultTabWidget, "Clustering Result");
	mIndexClusteringResultTabWidget = mpImageViewTabWidget->insertTab(mIndexClusteringResultTabWidget, mpClusteringResultPrototypeViewWidget, "Clustering Result");

	mpImageViewTabWidget->setCurrentIndex(currentIndex);
	// ----------------------
	return;
 }

 void MainWindow::saveClusterResult()
 {

	 if (!mClusterController.clusteringResultPointer()->isEmpty()) {
		 try {
			 QString fileName =
						  QFileDialog::getSaveFileName(this, tr("Save Cluster Result XML file"),
													   QDir::currentPath(),
													   tr("Cluster Result XML Files (*.xml)"));
			 if (fileName.isEmpty())
				 return;

			// FIXME: TEST OF ClusteringResultXMLWriter:
			ClusteringResult *pClusteringResult = mClusterController.clusteringResultPointer();
			std::cout << pClusteringResult->nClusters() << std::endl;
			ClusteringResultXMLWriter xmlWriter( mClusterController.documentPointer(), pClusteringResult );
			QFile file(fileName);
			if (!file.open(QFile::WriteOnly | QFile::Text)) {
				std::cout << "cannot write file" << std::endl;
				QMessageBox::warning(this, tr("Cluster Result"),
										   tr("Cannot write file %1:\n%2.")
										   .arg(fileName)
										   .arg(file.errorString()));


				return;
			}
			xmlWriter.writeFile(&file);

			file.close();
			// ---------------------------------------
		}
		catch (Exception e) {
			 std::cout << "An exception has occured in MainWindow::saveClusterResult(): " << e.what() << std::endl;
			 QMessageBox::critical(this,"Error saving cluster result!", e.what() );
		}
	 }
	 else {
		 QMessageBox::information(this, "No Cluster Result", "No cluster result available!");
	 }
	 return;
 } // end saveClusterResult()

 void MainWindow::setBusy(bool isBusy, QString busyText/* = QString("Busy...")*/)
 {
	 if (isBusy) {
		 mpProgressDialog = new QProgressDialog(busyText, "Cancel", 0, -1, this);
		 mpProgressDialog->setWindowModality(Qt::WindowModal);
		 mpProgressDialog->show();

//		 this->setEnabled(false);
//
//		 mpProgressBar->setRange(0,-1);
//		 mpProgressBar->setValue(0);
//		 mpProgressBar->setEnabled(true);
//		 mpProgressBar->setWindowModality(Qt::WindowModal);
	 }
	 else {
		 delete mpProgressDialog;
//		 this->setEnabled(true);

//		 mpProgressBar->setRange(0,1);
//		 mpProgressBar->setValue(0);
//		 mpProgressBar->setEnabled(false);
	 }

	 return;
 }

} // end of namespace impact_tr4
