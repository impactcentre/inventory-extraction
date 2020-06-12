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
#ifndef FINEREADERXMLDATAPARSERWIDGET_HPP_
#define FINEREADERXMLDATAPARSERWIDGET_HPP_

#include "ui_FineReaderXMLDataParserWidget.h"

#include "xml/xml.h"
#include "xml/FineReaderXMLOutputHandler.hpp"

#include "core/core.h"
#include "core/image/GrayImage.hpp"
#include "core/FineReaderData.hpp"
#include "core/FineReaderOperations.hpp"

#include "gui/ImageViewer.hpp"

namespace impact_tr4 {

// Forward references:
class ImageViewer;

// Class Definition:
class FineReaderXMLDataParserWidget : public QWidget
{
Q_OBJECT
// VARIABLES:
private:

public:
	// CONSTRUCTORS:
	FineReaderXMLDataParserWidget(ImageViewer *pImageViewer, QWidget *parent = 0, Qt::WFlags f = Qt::Widget);
	~FineReaderXMLDataParserWidget();

public:
	void closeEvent ( QCloseEvent * event );

public slots:
	void on_parseXMLButton_clicked();
	void on_parseImageButton_clicked();
	void on_parseFolderButton_clicked();
	void on_chooseXMLFileButton_clicked();
//	void on_chooseImageFileButton_clicked();

	void on_chooseBBoxSaveDirButton_clicked();

signals:
	void sendFineReaderXMLData(SimpleFineReaderXMLData* pData);
	void sendFRData(FRData* pData);
private:

	// PRIVATE MEMBERS:
	Ui::FineReaderXMLDataParserWidget _ui;
	FineReaderXMLOutputHandler* mpHandler;
	ImageViewer* mpImageViewer;
	FRData mParsedData;

}; // end class


} // end of namespace impact_tr4


#endif /*FINEREADERXMLDATAPARSERWIDGET_HPP_*/
