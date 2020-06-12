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
#include "InputFilesWidget.hpp"

#include <stdlib.h>

#include <QFileDialog>
#include "ImageViewer.hpp"

#include "clustering/data_reader/FRXMLCharDataReader.hpp"
#include "clustering/data_reader/Im2CharRectsCharDataReader.hpp"

namespace impact_tr4
{

InputFilesWidget::InputFilesWidget(QWidget *parent /*= 0*/, Qt::WFlags f /*= Qt::Widget*/)
: QWidget(parent, f)
{

	// setup ui of corresponging ui-file-variable
	// NOTE: setupUi also connects all signals of all child objects of the widget with the slots named by the on_<object-name>_<signal-name>(...) pattern!
	_ui.setupUi(this);
	mCurrentOpenImageFn = "";

	// ...:
	_ui.createClusterResultButton->setVisible(false);
	_ui.createFineReaderOutputButton->setVisible(false);
}

InputFilesWidget::~InputFilesWidget()
{
}

void InputFilesWidget::on_addImageFilesButton_clicked()
{
	QStringList fileNames = QFileDialog::getOpenFileNames( this, "Select image files", QDir::currentPath(), ImageViewer::IMAGE_LOAD_FILES_FILTER );

    for (int i = 0; i < fileNames.size(); ++i) {
         _ui.imageFileListWidget->addItem( fileNames.at(i) );
    }

	return;
} // end on_addImageFilesButton_clicked()

void InputFilesWidget::on_removeSelectedButton_clicked()
{
	QList<QListWidgetItem *> selectedItemsList = _ui.imageFileListWidget->selectedItems();
	std::cout << selectedItemsList.size() << std::endl;
	for (int i=0; i<selectedItemsList.size(); ++i){
		int rowI = _ui.imageFileListWidget->row ( selectedItemsList.at(i) );
		delete _ui.imageFileListWidget->takeItem ( rowI );
	}

	return;
}

void InputFilesWidget::on_removeAllButton_clicked()
{
	_ui.imageFileListWidget->selectAll();
	this->on_removeSelectedButton_clicked();
	return;
}

void InputFilesWidget::on_createClusterResultButton_clicked()
{
	emit createClusterResult();
}

void InputFilesWidget::setFileList(const std::vector<std::string>& filenames)
{
	this->clearFiles();
	for (int i=0; i<filenames.size(); ++i) {
		_ui.imageFileListWidget->addItem( QString::fromStdString(filenames[i]) );
	}
	return;
}

void InputFilesWidget::on_showSegmentationCheckBox_stateChanged(int state)
{
	if (mCurrentOpenImageFn.isEmpty()) {
		QMessageBox::critical(this,"No image opened yet!", "No image opened yet!" );
		return;
	}
	emit showSegmentation(this->parsingParameters(), state>0 );

	return;
}

void InputFilesWidget::on_createFineReaderOutputButton_clicked()
{
	std::cout << "creating FineReader XML output!" << std::endl;
	std::vector<std::string> inputFileListStrs = this->inputFileList();
	bool flag=true;
	for (int i=0; i<inputFileListStrs.size(); ++i) {
		std::string inputFn = inputFileListStrs[i];
		std::string tmp = baseFileNameWithPath(inputFn);
		std::string outputFn = tmp + ".xml";
		bool doExecute = true;
		// check if file exists already and jump out of loop if no overwrite is wanted:
		if (QFile(QString::fromStdString(outputFn)).exists()) {
			QString txt = "The output file "+QString::fromStdString(outputFn)+" exists. Overwrite?";
			QMessageBox::StandardButton returnButton = QMessageBox::question(this, "File exists", txt,QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
			if (returnButton == QMessageBox::Cancel) { // jump out of whole loop
				std::cout << "cancel!" << std::endl;
				return;
			}
			else if (returnButton == QMessageBox::No) {
				std::cout << "do not overwrite!" << std::endl;
				doExecute = false;
			}
		}
		// execute finereader command:
		if (doExecute) {
//			std::string commandStr = "cli.exe -if " + inputFn + " -f XML -xca -xnc -of " + outputFn;
			// use '-prn  Gothic' for gothic
			std::string commandStr = "cli.exe -if " + inputFn + " -prn Gothic -f XML -xca -xnc -of " + outputFn;
			if (system(commandStr.c_str())) {
				flag = false;
			}
		}
	} // end for all files i
	if (flag) {
		QMessageBox::information(this,"Success!", "FineReader output created successfully!");
	}
	else {
		QMessageBox::critical(this,"Error!", "An error occurred calling FineReader command line application!");
	}
	return;
} // end on_createFineReaderOutputButton_clicked()

void InputFilesWidget::on_imageFileListWidget_itemDoubleClicked(QListWidgetItem * item)
{
//	std::cout << item->text().toStdString() << std::endl;
	emit openImage( item->text() );
	_ui.showSegmentationCheckBox->setChecked(false);

	mCurrentOpenImageFn = item->text();
}

std::vector<std::string> InputFilesWidget::inputFileList() const
{
	std::vector<std::string> fileList;
	for (int i=0; i<_ui.imageFileListWidget->count(); ++i) {
		fileList.push_back(_ui.imageFileListWidget->item(i)->text().toStdString());
	}
	return fileList;
}

ParsingMethodType InputFilesWidget::parsingMethodType() const
{
	QString methodStr = _ui.parsingMethodComboBox->currentText();
	if (!methodStr.compare("XML (FineReader)")) {
		return FINEREADER_XML;
	}
	else if (!methodStr.compare("XML (Im2CharRects)")) {
		return IM2CHARRECTS_XML;
	}
	else if (!methodStr.compare("DAT Files")) {
		return DAT_FILES;
	}
	return UNKNOWN_PARSING_METHOD;
}

ParsingParameters InputFilesWidget::parsingParameters() const
{
	ParsingParameters parsingPars;
	parsingPars.parsingMethod = parsingMethodType();
	parsingPars.filenamePrefix = _ui.prefixLineEdit->text().toStdString();
	parsingPars.filenameSuffix = _ui.suffixLineEdit->text().toStdString();
	parsingPars.subsetPerc = _ui.parseSubsetSpinBox->value();

	// set subset generation type according to selection:
	parsingPars.generationType = UNKNOWN_GENERATION_TYPE;
	if (_ui.randomRadioButton->isChecked()) {
		parsingPars.generationType = PARSE_RANDOMLY;
	}
	else if (_ui.firstRadioButton->isChecked()) {
		parsingPars.generationType = PARSE_FIRST;
	}
	else if (_ui.lastRadioButton->isChecked()) {
		parsingPars.generationType = PARSE_LAST;
	}

	// set parsing type according to selection:
	parsingPars.parsingType = UNKNOWN_PARSESUBSET_TYPE;
	if (_ui.parseAllRadioButton->isChecked()) {
		parsingPars.parsingType = PARSE_ALL;
	}
	else if (_ui.parseNonSuspRadioButton->isChecked()) {
		parsingPars.parsingType = PARSE_ONLY_NON_SUSP;
	}
	else if (_ui.parseSuspRadioButton->isChecked()) {
		parsingPars.parsingType = PARSE_ONLY_SUSP;
	}

	return parsingPars;
}

} // end of namespace impact_tr4
