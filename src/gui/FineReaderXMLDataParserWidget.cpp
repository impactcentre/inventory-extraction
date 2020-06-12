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
#include "FineReaderXMLDataParserWidget.hpp"

#include <QFileDialog>
//#include <boost/thread/thread.hpp>

namespace impact_tr4 {

FineReaderXMLDataParserWidget::
FineReaderXMLDataParserWidget(ImageViewer *pImageViewer, QWidget *parent /*= 0*/, Qt::WFlags f /*= Qt::Widget*/)
: QWidget(parent, f), mpImageViewer(pImageViewer)
{
	// setup ui of corresponging ui-file-variable
	// NOTE: setupUi also connects all signals of all child objects of the widget with the slots named by the on_<object-name>_<signal-name>(...) pattern!
	_ui.setupUi(this);


} // end of FineReaderXMLDataParserWidget default constructor

FineReaderXMLDataParserWidget::
~FineReaderXMLDataParserWidget()
{
	delete mpHandler; mpHandler = 0x0;
}

void FineReaderXMLDataParserWidget::closeEvent ( QCloseEvent * event )
{
//	std::cout << "CLOSE EVENT" << std::endl;
	// send 0x0 pointer for finereader data, so that bboxes are erased:
	emit sendFineReaderXMLData(0x0);
	emit sendFRData(0x0);
}

void FineReaderXMLDataParserWidget::on_parseXMLButton_clicked()
{
	GrayImage<> *pBaseImage = mpImageViewer->image();

	std::cout << "Dimensions of base image: " << std::endl;
    std::cout << pBaseImage->rows() << " " << pBaseImage->columns() << std::endl;

	// create customized handler
    mpHandler = new FineReaderXMLOutputHandler();
	// create simple reader and set content/error handler
    QXmlSimpleReader reader;
    reader.setContentHandler(mpHandler);
    reader.setErrorHandler(mpHandler);

    std::cout << _ui.xmlFileNameLabel->text().toStdString() << std::endl;
    QFile file(_ui.xmlFileNameLabel->text());
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    // creat xml input source from file
    QXmlInputSource xmlInputSource(&file);
    // parse the file using the simple reader
    reader.parse(xmlInputSource);
    std::cout << "Successfully parsed data!" << std::endl;

    SimpleFineReaderXMLData* pData = mpHandler->getFineReaderData();

    if (_ui.showBBoxesCheckBox->isChecked()) {
    	emit sendFineReaderXMLData(pData);
    }
    if (_ui.saveBBoxesCheckBox->isChecked()) {
    	std::cout << "Saving bounding boxes... ";
    	QDir dir;
    	QString path = _ui.bboxSaveDirLabel->text();
    	QString fn, base;
    	GrayImage<> *pBboxImage = 0x0;
    	BoundingBox bbox;

    	if (!dir.exists(path)) dir.mkpath(path);
    	if (!path.endsWith("/")) path.append("/");
    	for (int i=0; i<pData->parsedBBoxes.size(); ++i) {
    		bbox = pData->parsedBBoxes[i];
    		base.setNum(i);
    		fn = path + base.rightJustified(4, '0') + ".jpg";
    		// set bbox to image and save roi:
    		bbox.saveImage(*pBaseImage, fn.toStdString());
#if 0
    		pBaseImage->setRoi(bbox);
    		pBaseImage->save(fn.toStdString());
    		pBaseImage->releaseRoi();
#endif
    	} // end for all bboxes i
    	std::cout << "finished!" << std::endl;
    }
}

void FineReaderXMLDataParserWidget::on_parseFolderButton_clicked()
{
QString dirName = QFileDialog::getExistingDirectory( this, "Open Directory",
												QDir::currentPath() );
std::cout << dirName.toStdString() << std::endl;
if (!dirName.isEmpty()) {
	std::cout << dirName.toStdString() << std::endl;
	QDir dir(dirName);
	QStringList fileList =
		dir.entryList ( QDir::Files, QDir::Name );
	FineReaderOperations::loadEngine();
	for (int i=0; i<fileList.size(); ++i) {
		emit sendFRData(0x0);
		QString fn = dirName + "/" + fileList.at(i);
		std::cout << fn.toStdString() << std::endl;
		mpImageViewer->open(fn);
		FineReaderOperations::parseImage( fn.toStdString(), mParsedData );
		mParsedData.exportImages<>( *mpImageViewer->image(), (fn+"_frout").toStdString() );
		emit sendFRData(&mParsedData);
		mpImageViewer->repaint();
		mpImageViewer->saveAs(fn+"_frout/segmentation_image.tiff");
	}
	FineReaderOperations::unloadEngine();
}
return;
}

void FineReaderXMLDataParserWidget::on_parseImageButton_clicked()
{
//FRData data;
FineReaderOperations::loadEngine();
FineReaderOperations::parseImage(mpImageViewer->fileName().toStdString(), mParsedData);
FineReaderOperations::unloadEngine();
//mParsedData.exportImages(*mpImageViewer->image(), (mpImageViewer->fileName()+"_out").toStdString());
emit sendFRData(&mParsedData);

/*
FRCharIterator it = data.begin();

std::cout << "begin: " << data.begin() << std::endl;
std::cout << "emd: " << data.end() << std::endl;

while (it != data.end()) {
	std::cout << it->boundingBox();
//	std::cout << it;

//	FRCharacter ch = *it;
//	ch.print();
//	std::cout << ch.boundingBox();

	++it;
}
*/
return;
}

void FineReaderXMLDataParserWidget::on_chooseBBoxSaveDirButton_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
	                                                 QDir::currentPath(),
	                                                 QFileDialog::ShowDirsOnly
	                                                 | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		_ui.bboxSaveDirLabel->setText(dir);
	}
	return;
}

void FineReaderXMLDataParserWidget::on_chooseXMLFileButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName( this, "Choose XML file", ".", "XML Files (*.xml)");
	if (!fileName.isEmpty()) {
		_ui.xmlFileNameLabel->setText(fileName);
	}
}

//void FineReaderXMLDataParserWidget::on_chooseImageFileButton_clicked()
//{
//	QString filter = "Image Files (*.bmp *.gif *.jpg *.png *.pbm *.pgm *.ppm *.tiff *.tif *.xbm *.xpm)";
//	QString fileName = QFileDialog::getOpenFileName( this, "Choose Image file", ".", filter);
//	if (!fileName.isEmpty()) {
//		_ui.imageFileNameLabel->setText(fileName);
//	}
//}

} // end of namespace impact_tr4
