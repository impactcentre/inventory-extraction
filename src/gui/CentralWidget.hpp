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
#ifndef CENTRALWIDGET_HPP_
#define CENTRALWIDGET_HPP_

#include <QWidget>
#include <QSlider>
#include <QPushButton>

namespace impact_tr4
{

class ImageViewer;
class CentralWidget;

class CentralWidget : public QWidget
{
	Q_OBJECT
public:
	CentralWidget(ImageViewer *pImageViewer, QWidget *parent = 0, Qt::WFlags f = Qt::Widget);
	virtual ~CentralWidget();

	void setZoomEnabled(bool flag);

public slots:
	void scaleImageNormalSize();

signals:

private:
	void connectStuff();

	ImageViewer *mpImageViewer;
    QSlider *mpZoomSlider;
    QPushButton *mpDefaultZoomButton;

};

} // end of namespace impact_tr4

#endif /*CENTRALWIDGET_HPP_*/
