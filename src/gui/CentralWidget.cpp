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
#include "CentralWidget.hpp"

#include "ImageViewer.hpp"

namespace impact_tr4
{

CentralWidget::CentralWidget(ImageViewer *pImageViewer, QWidget *parent /*= 0*/, Qt::WFlags f /*= Qt::Widget*/)
: QWidget(parent, f), mpImageViewer(pImageViewer)
{
	 QHBoxLayout *hboxLayout = new QHBoxLayout;
	 QLabel *zoomLabel = new QLabel("Zoom: ");
	 QFont font("Helvetica", 10, QFont::Bold);
	 zoomLabel->setFont(font);

	 mpDefaultZoomButton = new QPushButton("Normal Size");
	 mpDefaultZoomButton->setFont(font);
	 connect( mpDefaultZoomButton, SIGNAL( clicked() ), this, SLOT( scaleImageNormalSize() ) );
	 mpZoomSlider = new QSlider( Qt::Horizontal, this );
	 mpZoomSlider->setRange(1, 350);
	 mpZoomSlider->setValue(100);

	 hboxLayout->addWidget( zoomLabel );
	 hboxLayout->addWidget( mpZoomSlider );
	 hboxLayout->addWidget( mpDefaultZoomButton );

	 QVBoxLayout *vboxLayout = new QVBoxLayout;

	 QScrollArea *scrollArea = new QScrollArea;
     //scrollArea->setBackgroundRole(QPalette::Dark);
     //scrollArea->setWidget(mpImageViewer);
     //vboxLayout->addWidget( scrollArea );
	 vboxLayout->addWidget( mpImageViewer );
	 vboxLayout->addLayout( hboxLayout );

	 this->setLayout( vboxLayout );

	 connectStuff();
}

void CentralWidget::connectStuff()
{
	connect( mpZoomSlider, SIGNAL( valueChanged(int) ), mpImageViewer, SLOT( changeZoom(int) ) );

	return;
}

CentralWidget::~CentralWidget()
{
}

void CentralWidget::setZoomEnabled(bool flag)
{
    mpDefaultZoomButton->setEnabled( flag );
    mpZoomSlider->setEnabled( flag );
}

void CentralWidget::scaleImageNormalSize()
{
	mpZoomSlider->setValue(100);
}

} // end of namespace impact_tr4
