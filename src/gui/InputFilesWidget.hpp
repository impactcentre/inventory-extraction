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
#ifndef INPUTFILESWIDGET_HPP_
#define INPUTFILESWIDGET_HPP_

#include "ui_InputFilesWidget.h"

#include <iostream>

#include "clustering/DataStructs.hpp"
#include "clustering/data_reader/FRXMLCharDataReader.hpp"
#include "clustering/data_reader/Im2CharRectsCharDataReader.hpp"

namespace impact_tr4
{


class ImageViewer;
class InputFilesWidget;

class InputFilesWidget : public QWidget
{
Q_OBJECT
public:
	InputFilesWidget(QWidget *parent = 0, Qt::WFlags f = Qt::Widget);
	virtual ~InputFilesWidget();

	std::vector<std::string> inputFileList() const;
	ParsingMethodType parsingMethodType() const;
	ParsingParameters parsingParameters() const;

public slots:
	void on_addImageFilesButton_clicked();
	void on_removeSelectedButton_clicked();
	void on_removeAllButton_clicked();
	void on_showSegmentationCheckBox_stateChanged(int state);
	void on_createFineReaderOutputButton_clicked();
	void on_createClusterResultButton_clicked();

	void on_imageFileListWidget_itemDoubleClicked(QListWidgetItem * item);
	void clearFiles() {_ui.imageFileListWidget->clear(); }
	void setFileList(const std::vector<std::string>& filenames);

signals:
	void openImage( QString fn );
//    void sendCharDataReaderPointer(XMLCharDataReaderI* pDataReader);
    void showSegmentation(ParsingParameters parsingParameters, bool show);
    void createClusterResult();

private:
	// PRIVATE MEMBERS:
	Ui::InputFilesWidget _ui;
	QString mCurrentOpenImageFn; // stores the filename of the currently opened image
	CharDataReaderI* mpCharDataReader;

};

} // end of namespace impact_tr4

#endif /*INPUTFILESWIDGET_HPP_*/
