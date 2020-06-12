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

#include "ImageViewer.hpp"

#include "clustering/data_reader/FRXMLCharDataReader.hpp"
#include "clustering/data_reader/Im2CharRectsCharDataReader.hpp"
#include "clustering/data_reader/DatFileDataReader.hpp"

namespace impact_tr4 {

/*static*/ const QString ImageViewer::IMAGE_LOAD_FILES_FILTER
	= "Image Files (*.bmp *.gif *.jpg *.png *.pbm *.pgm *.ppm *.tiff *.tif *.xbm *.xpm)";
/*static*/ const QString ImageViewer::IMAGE_SAVE_FILES_FILTER
	= "Image Files (*.bmp *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm)";


	QRectF ImageCharGraphicsItem::boundingRect() const
	{
		 int x = mImageChar.bBox.x();
		 int y = mImageChar.bBox.y();
		 int width = mImageChar.bBox.width();
		 int height = mImageChar.bBox.height();
		 QRectF qrect( x-0.5, y+0.5, width-0.5, height+0.5 );

		 return qrect;
	}

	void ImageCharGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			QWidget *widget)
	{
		 int x = mImageChar.bBox.x();
		 int y = mImageChar.bBox.y();
		 int width = mImageChar.bBox.width();
		 int height = mImageChar.bBox.height();
		 QRect qrect( x, y, width, height );

		 QColor boxColor = QColor(Qt::red);
	//					 std::cout << imageChar.suspicious << std::endl;
		 if (mImageChar.suspicious) { // if this is a susp. char, change color to blue!
			 boxColor = QColor(Qt::blue);
		 }
		 QPen pen = QPen(boxColor);
		 pen.setWidth(1);
		 pen.setCosmetic(true);

		 if (this->isSelected()) {
			 pen.setWidth(2);
			 pen.setStyle(Qt::DashLine);
		 }

		 painter->setPen(pen);
		 painter->drawRect(qrect);

		 return;
	} // end paint

 ImageViewer::ImageViewer(QWidget * parent, Qt::WindowFlags f)
 {
	 // init data:
	 mpImage = 0x0;
	 mDrawBoxes = false;
	 mDrawXMLBoxes = false;
	 mFileName = "";

	 // init gui:
	 QHBoxLayout *hboxLayout = new QHBoxLayout;
	 QLabel *zoomLabel = new QLabel("Zoom: ");
	 QFont font("Helvetica", 10, QFont::Bold);
	 zoomLabel->setFont(font); zoomLabel->setFixedSize(zoomLabel->sizeHint());

	 mpDefaultZoomButton = new QPushButton("Default Size");
	 mpDefaultZoomButton->setFont(font); mpDefaultZoomButton->setFixedSize(mpDefaultZoomButton->sizeHint());
	 connect( mpDefaultZoomButton, SIGNAL( clicked() ), this, SLOT( normalSize() ) );

	 mpFitToWindowButton = new QPushButton("Fit To Window");
	 mpFitToWindowButton->setFont(font); mpFitToWindowButton->setFixedSize(mpDefaultZoomButton->sizeHint());
	 connect( mpFitToWindowButton, SIGNAL( clicked() ), this, SLOT( fitToWindow() ) );

	 mpZoomSlider = new QSlider( Qt::Horizontal, this );
	 mpZoomSlider->setRange(1, 400);
	 mpZoomSlider->setValue(100); mpZoomSlider->setFixedHeight(mpZoomSlider->sizeHint().height());
	 connect( mpZoomSlider, SIGNAL( valueChanged(int) ), this, SLOT( changeZoom(int) ) );

	 hboxLayout->addWidget( zoomLabel );
	 hboxLayout->addWidget( mpZoomSlider );
	 hboxLayout->addWidget( mpDefaultZoomButton );
	 hboxLayout->addWidget( mpFitToWindowButton );

     graphicsScene = new QGraphicsScene;
     pixmapItem = new QGraphicsPixmapItem();
     pixmapItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
     connect(graphicsScene, SIGNAL(selectionChanged()), this, SLOT(onGraphicsSceneSelectionChanged()));
     graphicsView = new QGraphicsView(graphicsScene, this);
//     graphicsView->setMinimumSize(800, 800);
//     graphicsView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	 QVBoxLayout *vboxLayout = new QVBoxLayout;

	 vboxLayout->addWidget( graphicsView );
	 vboxLayout->addLayout( hboxLayout );


	 this->setLayout( vboxLayout );

	 emit imageLoaded(true);
//   setWindowTitle(tr("Image Viewer"));
//	 resize(500,400);
 }

 void ImageViewer::open(QString fn /*= QString()*/)
 {
	 std::cout << "Opening image " << fn.toStdString() << std::endl;
	 QString fileName;
	 if (fn.isEmpty()) {
		 fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
				 QDir::currentPath(), IMAGE_LOAD_FILES_FILTER);
	 }
	 else { fileName = fn; }

     if (!fileName.isEmpty()) {
    	 mFileName = fileName;
#if 1
    	 QImageReader imageReader(mFileName);
    	 QImage image = imageReader.read();
    	 if (image.isNull()) {
    		 QMessageBox::critical ( this, "Error opening image", mFileName + ": " + imageReader.errorString() );
    		 return;
    	 }
    	 mPixmap = QPixmap::fromImage(image);
    	 pixmapItem->setPixmap(mPixmap);
//    	 mPixmap.load(fileName);
    	 mPixmapBackup = mPixmap;

    	 graphicsScene->removeItem(pixmapItem);
    	 graphicsScene->clear();
    	 graphicsScene->addItem(pixmapItem);

    	 delete mpImage; mpImage = 0x0;
    	 mpImage = new GrayImage<>(image);
//    	 std::cout << "image:" << std::endl;
//    	 std::cout << *mpImage << std::endl;

//    	 mpImage = new GrayImage(mPixmap.toImage());
//    	 mImage.init(mPixmap.toImage());
#else
    	 mpImage = new GrayImage<>(fileName.toStdString());
    	 // create qimage from existing data:
    	 std::cout << "hier1" << std::endl;
    	 QImage image((uchar*)(mpImage->data()), mpImage->columnsRoi(), mpImage->rowsRoi(), QImage::Format_RGB32);
    	 std::cout << "hier!" << std::endl;
    	 mPixmap = QPixmap::fromImage(image);
    	 std::cout << "h3" << std::endl;
//    	 mPixmap.load(fileName);
    	 mPixmapBackup = mPixmap;
    	 std::cout << "h4" << std::endl;
#endif
         if (mPixmap.isNull()) {
             QMessageBox::information(this, tr("Image Viewer"),
                                      tr("Cannot load %1.").arg(fileName));
             return;
         }

         fitToWindow();
    	 this->update();

         emit imageLoaded(true);
         emit statusBarMessage("Successfully loaded image from file "+mFileName, 5000);
     }
 }

#if 1
 void ImageViewer::open( const GrayImage<> *pImage )
 {
	 mFileName = "";
	 delete mpImage; mpImage = 0x0;
	 mpImage = new GrayImage<>(*pImage);
	 QImage image;
	 mpImage->computeQImage(image);
	 mPixmap = QPixmap::fromImage(image);
	 pixmapItem->setPixmap(mPixmap);
	 graphicsScene->removeItem(pixmapItem);
	 graphicsScene->clear();
	 graphicsScene->addItem(pixmapItem);
	 mPixmapBackup = mPixmap;

	 fitToWindow();

     emit imageLoaded(true);
     emit statusBarMessage("Successfully loaded image from data", 5000);

     return;
 } // end open( GrayImage<> *pImage )
#endif

 void ImageViewer::close()
 {
	 delete mpImage; mpImage = 0x0;
	 graphicsScene->removeItem(pixmapItem);
	 graphicsScene->clear();

	 emit imageLoaded(false);
 } // end close()

 void ImageViewer::saveAs(QString fn /*= QString()*/)
 {
	 QString fileName;
	 if (fn.isEmpty()) {
		 fileName = QFileDialog::getSaveFileName(this, tr("Save File As"),
				 QDir::currentPath(), IMAGE_SAVE_FILES_FILTER);
	 }
	 else { fileName = fn; }

	 if (!fileName.isEmpty()) {
		 QImageWriter imageWriter( fileName );
		 if ( !imageWriter.write ( mPixmap.toImage() ) ) {
			 QMessageBox::critical ( this, "Error saving image", imageWriter.errorString() );
		 }
	 } // end if file was selected
	 return;
 } // end saveAs

 void ImageViewer::showSegmentation(ParsingParameters parsingParameters, bool show)
 {
	 if (show) {
		try {
//			std::cout << "parsing method: " << parsingParameters.parsingMethod << std::endl;
			CharDataReaderI *pCharDataReader = 0x0;
			std::string fnSeg="";
			switch (parsingParameters.parsingMethod) {
				case FINEREADER_XML:
					pCharDataReader = new FRXMLCharDataReader();
					fnSeg = createSegmentationResultFileName(mFileName.toStdString(), "xml", parsingParameters.filenamePrefix, parsingParameters.filenameSuffix);
					break;
				case IM2CHARRECTS_XML:
					pCharDataReader = new Im2CharRectsCharDataReader();
					fnSeg = createSegmentationResultFileName(mFileName.toStdString(), "xml", parsingParameters.filenamePrefix, parsingParameters.filenameSuffix);
					break;
				case DAT_FILES:
					pCharDataReader = new DatFileDataReader();
					fnSeg = createSegmentationResultFileName(mFileName.toStdString(), "dat", parsingParameters.filenamePrefix, parsingParameters.filenameSuffix);
					break;
				default:
					throw Exception("Error parsing input files - method unknown!");
					break;
			} // end switch

			// check, if result file exists:
		    QFile file( QString::fromStdString(fnSeg) );
		    if (!file.open(QFile::ReadOnly | QFile::Text)) {
		     	throw FileNotFoundException("Error openening parsing result file "+fnSeg);
		    }

			if (pCharDataReader != 0x0) {
				std::vector<ImageChar> imageCharVec;
				pCharDataReader->parseSingleImageFile(mFileName.toStdString(), fnSeg, mpImage, imageCharVec);
				 for (int i=0; i<imageCharVec.size(); ++i) {
					 ImageCharGraphicsItem *pImageCharItem = new ImageCharGraphicsItem(imageCharVec[i]);
					 graphicsScene->addItem(pImageCharItem);
					 pImageCharItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
				 } // end for all bounding boxes
				 this->update();
			}
		} // end try
		catch (Exception e) {
			std::cout << "An exception has occured in ImageViewer::showSegmentation " << e.what() << std::endl;
			QMessageBox::critical(this,"An exception has occured in ImageViewer::showSegmentation", e.what() );
		}
	 } // end if show == true
	 else {
		 graphicsScene->removeItem(pixmapItem); // remove pixmap from scene so it won't be destroyed
		 graphicsScene->clear(); // clear and destroy all items (i.e. rectangles) from scene
		 graphicsScene->addItem(pixmapItem); // re-add pixmap item to scene
	 }
	 return;
 }

 void ImageViewer::paintEvent(QPaintEvent *event)
 {

 } // end of paintEvent(...)

 void ImageViewer::print()
 {
     QPrintDialog dialog(&printer, this);
     if (dialog.exec()) {
         QPainter painter(&printer);
         QRect rect = painter.viewport();
         QSize size = mPixmap.size();
         size.scale(rect.size(), Qt::KeepAspectRatio);
         painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
         painter.setWindow(mPixmap.rect());
         painter.drawPixmap(0, 0, mPixmap);
     }
 }

 void ImageViewer::zoomIn()
 {
     scale(1.25);
 }

 void ImageViewer::zoomOut()
 {
     scale(0.8);
 }

 void ImageViewer::changeZoom( int zoomFactor )
 {
     scale( double(zoomFactor/100.0f) );
 }

 void ImageViewer::normalSize()
 {
	 mpZoomSlider->setValue(100);
//     scale(1.0f);
 }

 void ImageViewer::fitToWindow()
 {
#if 1
	 graphicsView->fitInView(pixmapItem, Qt::KeepAspectRatio);
	 mpZoomSlider->setValue(graphicsView->transform().m11()*100); // set slider value to scale
#endif
 }

 void ImageViewer::scale(double factor)
 {
     graphicsView->resetTransform();
     graphicsView->scale(factor, factor);

     return;
 }

 void ImageViewer::setZoomEnabled(bool flag)
 {
     mpDefaultZoomButton->setEnabled( flag );
     mpZoomSlider->setEnabled( flag );
 }

 void ImageViewer::onGraphicsSceneSelectionChanged()
 {
//	 std::cout << "maintaining selections!" << std::endl;
	 QList<QGraphicsItem  *> items = graphicsScene->selectedItems();
	 if (items.size() == 0) return;

	 disconnect(graphicsScene, SIGNAL(selectionChanged()), this, SLOT(onGraphicsSceneSelectionChanged()));
	 QGraphicsItem* lastItem = items.last();
	 graphicsScene->clearSelection();
	 lastItem->setSelected(true);
	 connect(graphicsScene, SIGNAL(selectionChanged()), this, SLOT(onGraphicsSceneSelectionChanged()));

#if 1
	 int x = ((ImageCharGraphicsItem*)lastItem)->mImageChar.bBox.x();
	 int y = ((ImageCharGraphicsItem*)lastItem)->mImageChar.bBox.y();
	 int width = ((ImageCharGraphicsItem*)lastItem)->mImageChar.bBox.width();
	 int height = ((ImageCharGraphicsItem*)lastItem)->mImageChar.bBox.height();
	 QString bBoxStr("BBox (x,y,width,height) = [");
	 bBoxStr += QString::number(x) + ", ";
	 bBoxStr += QString::number(y) + ", ";
	 bBoxStr += QString::number(width) + ", ";
	 bBoxStr += QString::number(height) + "]";
	 QString asciiStr("Text = >");
	 asciiStr += QString::fromStdString(((ImageCharGraphicsItem*)lastItem)->mImageChar.text) + "<";

	 emit statusBarMessage(bBoxStr + "; " + asciiStr, 0);
#endif
	 return;
 }

} // end of namespace impact_tr4
