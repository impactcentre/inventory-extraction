/*
 * PreprocessingWidget.cpp
 *
 *  Created on: 19.01.2010
 *      
 */


#include "PreprocessingWidget.hpp"
#include "clustering/ClusteringController.hpp"
#include "clustering/Document.hpp"
#include "gui/MainWindow.hpp"

namespace impact_tr4 {

PreprocessingWidget::PreprocessingWidget(QWidget *parent /*= 0*/, Qt::WFlags f /*= Qt::Widget*/)
:QWidget(parent, f)
{
	// setup ui of corresponging ui-file-variable
	// NOTE: setupUi also connects all signals of all child objects of the widget with the slots named by the on_<object-name>_<signal-name>(...) pattern!
	_ui.setupUi(this);

	_ui.otherParametersGroupBox->setVisible(false);

	mpMainWindow = (MainWindow *)(parent);
}

PreprocessingWidget::~PreprocessingWidget()
{

}

const PreprocessingParameters PreprocessingWidget::parameters() const
{
	PreprocessingParameters pars;
	pars.binarize = _ui.binarizeCheckBox->isChecked();
	pars.invert = _ui.invertCheckBox->isChecked();
	pars.preMedian = _ui.preMedianCheckBox->isChecked();
	pars.preMedianMaskSize = _ui.preMedianSpinBox->value();
	pars.sizeNormWidth = _ui.newSizeWidthSpinBox->value();
	pars.sizeNormHeight = _ui.newSizeHeightSpinBox->value();
	pars.postMedian = _ui.postMedianCheckBox->isChecked();
	pars.postMedianMaskSize = _ui.postMedianSpinBox->value();

	return pars;
}

void PreprocessingWidget::on_preprocessingPreviewButton_clicked()
{
	int index = _ui.preprocessingPreviewSpinBox->value() - 1;
	ClusteringController *pClustContr = mpMainWindow->getClusterControllerPointer();
	Document *pDocument = pClustContr->documentPointer();
//	std::cout << "index = " << index << std::endl;

//	const int nProcessedImages = pDocument->nProcessedImages();
	const int nParsedImages = pDocument->nParsedImages();
//	std::cout << "n parsed images = " << nParsedImages << std::endl;
//	std::cout << "n processed images = " << nProcessedImages << std::endl;

//	if (index < nParsedImages && index < nProcessedImages) {
	if (index < nParsedImages) {
		// convert original image to pixmap:
		std::vector<ImageChar*>* pImageCharsVecPointer = pDocument->getImageCharsVecPointer();
		GrayImage<> *pImage = (*pImageCharsVecPointer)[index]->pImage;
		BoundingBox bbox = (*pImageCharsVecPointer)[index]->bBox;
		pImage->setRoi(bbox);
		GrayImage<> glyphImage;
		pImage->computeRoiImage(glyphImage);
		pImage->releaseRoi();

		// preprocess and show preprocessed image:
		GrayImage<> preprocessedImage = glyphImage;
#if 1
		PreprocessingParameters pars = this->parameters();
		PreprocessingResults::preprocessGlyph(pars, glyphImage, preprocessedImage);
//		preprocessedImage.show("ppimage");
		QImage ppImage;
		preprocessedImage.computeQImage(ppImage);
#else
		// convert preprocessimage to pixmap:
		QImage ppImage;
		(*pDocument->getProcessedImageVecPointer())[index].computeQImage(ppImage);
#endif

		// show original image:
		QImage origImage;
		glyphImage.computeQImage(origImage);
		mPixmap1 = QPixmap::fromImage(origImage);

		mPixmap2 = QPixmap::fromImage(ppImage);
//		mPixmap2 = QPixmap::fromImage(origImage);
		// set pixmaps in gui:
		_ui.originalImageLabel->setPixmap(mPixmap1);
		_ui.preprocessedImageLabel->setPixmap(mPixmap2);
	}
	return;
} // end on_preprocessingPreviewButton_clicked()


} // end of namespace impact_tr4
