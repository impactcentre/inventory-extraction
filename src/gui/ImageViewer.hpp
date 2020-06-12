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
#ifndef IMAGEVIEWER_HPP_
#define IMAGEVIEWER_HPP_

#include <QMainWindow>
#include <QPrinter>
#include <QScrollBar>
#include <QScrollArea>
#include <QLabel>
#include <QAction>
#include <QImageReader>
#include <QImageWriter>
#include <QSlider>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QVBoxLayout>

#include "core/image/GrayImage.hpp"
// #include "core/FineReaderData.hpp"
#include "xml/xml.h"
#include "clustering/data_reader/CharDataReaderI.hpp"

#include "MainWindow.hpp"
// #include "FineReaderXMLDataParserWidget.hpp"

namespace impact_tr4 {

 class MainWindow;
 class ImageChar;


 class ImageCharGraphicsItem : public QGraphicsItem
 {
 public:
	 ImageCharGraphicsItem(ImageChar imageChar)
		 : mImageChar(imageChar) {}

	 QRectF boundingRect() const;
	 void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);
// private:
	 ImageChar mImageChar;
 };


 class ImageViewer : public QWidget
 {
     Q_OBJECT

 public:
	ImageViewer(QWidget * parent = 0, Qt::WindowFlags f = 0);

	~ImageViewer()
	{
		delete mpImage;
	}

	const QString& fileName() const { return mFileName; }
	GrayImage<>* image() { return mpImage; }

    // public static const members:
    static const QString IMAGE_LOAD_FILES_FILTER;
    static const QString IMAGE_SAVE_FILES_FILTER;

 signals:
 	void imageLoaded(bool loaded);
 	void statusBarMessage(QString msg, int timeoutMs);

 public slots:
 	 // if fn is not empty, it is used for opening, elsewise dialog pops up
     void open( QString fn = QString() );
     /// Opens existing GrayImage object. Note that the whole image is copied into the viewer to remain consistent.
     void open( const GrayImage<> *pImage );
     void close( );
     // if fn is not empty, it is used for saving, elsewise dialog pops up
     void saveAs( QString fn = QString() );
     void print();
     void zoomIn();
     void zoomOut();
     void changeZoom( int zoomFactor );
     void normalSize();
     void fitToWindow();
     void setZoomEnabled(bool flag);

     void showSegmentation(ParsingParameters parsingParameters, bool show);

 protected:
     void paintEvent(QPaintEvent *event);

 private slots:
     void onGraphicsSceneSelectionChanged();

 private:
//	 void createZoomSlider();
	 void connectSignals();
     void createMenus();
     void scaleImage(double factor);
     void scale(double factor);

     QSlider *mpZoomSlider;
     QPushButton *mpDefaultZoomButton, *mpFitToWindowButton;
     QGraphicsScene *graphicsScene;
     QGraphicsView *graphicsView;
     QGraphicsPixmapItem *pixmapItem;

     QString mFileName;
     QPrinter printer;
     QPixmap mPixmap, mPixmapBackup;
     GrayImage<> *mpImage;

     bool mDrawXMLBoxes;
     bool mDrawBoxes;
 };
} // end of namespace impact_tr4

#endif /*IMAGEVIEWER_HPP_*/
