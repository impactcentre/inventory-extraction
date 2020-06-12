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
#include "ClusterSummaryWidget.hpp"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

namespace impact_tr4
{

ClusterSummaryWidget::ClusterSummaryWidget(CharClustIt clustIt, QWidget *parent /*=0*/)
    : QWidget(parent), mClustIt(clustIt)
{
	_ui.setupUi(this);

	_ui.unicodeLabel->setVisible(false); // FIXME: (?) make unicode label invisible, since this does not work...

	QRegExp rx( "[0-9A-Fa-f]{1,4}" );
	QValidator *validator = new QRegExpValidator( rx, this );
	_ui.unicodeLineEdit->setValidator(validator);

	this->updateClusterInfo();
	this->updatePrototypeImage();

	_ui.clusterNLabel->setText("Cluster");

	_ui.computePrototypeButton->setVisible(false);

	this->setAcceptDrops(true);
}

ClusterSummaryWidget::~ClusterSummaryWidget()
{
//	std::cout << "destructing ClusterSummaryWidget " << mClusterID << std::endl;
}

void ClusterSummaryWidget::on_unicodeLineEdit_textChanged()
{
	QString hexText = _ui.unicodeLineEdit->text();
	bool ok;
	unsigned int code = hexText.toUInt(&ok, 16);
	if (!ok) {
//		throw Exception("Fatal Error: conversion from hex-unicode string to uint not successful!");
		_ui.unicodeLabel->setText("");
		(*mClustIt)->setUnicodeLabel(0);

		return;
	}
	QChar unicodeChar(code);
	_ui.unicodeLabel->setText(unicodeChar);

	(*mClustIt)->setUnicodeLabel(code);

	return;
}

void ClusterSummaryWidget::on_infoTextLineEdit_textChanged()
{
	QString text = _ui.infoTextLineEdit->text();
	(*mClustIt)->setInfoText(text.toStdString());

	return;
}

void ClusterSummaryWidget::reload()
{
	this->updateClusterInfo();
	this->updatePrototypeImage();
}

void ClusterSummaryWidget::updateClusterInfo()
{
	QString info = "Nr of elements = ";
	info += QString::number((*mClustIt)->size());

	_ui.clusterInfoLabel->setText(info);

	QString str;

	_ui.unicodeLineEdit->setText(QString::fromStdString((*mClustIt)->unicodeLabelString()));
	_ui.infoTextLineEdit->setText(QString::fromStdString((*mClustIt)->infoText()));
	return;
}

void ClusterSummaryWidget::updatePrototypeImage()
{
	if (!(*mClustIt)->empty()) {
//		(*mClustIt)->recomputePrototypeImage();
		QImage imageCharQImage;
//		std::cout << "recomputing prototype image in updatePrototypeImage..." << std::endl;
		(*mClustIt)->getPrototypeImagePointer()->computeQImage(imageCharQImage);
		const int newWidth = _ui.prototypeLabel->minimumWidth();
		const int newHeight = _ui.prototypeLabel->minimumHeight();
		imageCharQImage = imageCharQImage.scaled( newWidth, newHeight, Qt::KeepAspectRatio );
//		imageCharQImage.invertPixels();
#if 0
		QImage prototypeImage;
		convertGrayscaleToJet(imageCharQImage, prototypeImage);
#else
		QImage prototypeImage = imageCharQImage;
#endif
		_ui.prototypeLabel->setPixmap(QPixmap::fromImage(prototypeImage));
	}
	else {
		_ui.prototypeLabel->clear();
		_ui.prototypeLabel->setText("EMPTY");
	}

    return;
}

void ClusterSummaryWidget::on_showButton_clicked()
{
	emit showClusterViewWidget(mClustIt);
}

void ClusterSummaryWidget::on_assignUnicodeButton_clicked()
{
	std::cout << "assigning unicode labe by visual keyboard... not implemented yet!" << std::endl;

	return;
}

void ClusterSummaryWidget::on_computePrototypeButton_clicked()
{
	this->updatePrototypeImage();
}

void ClusterSummaryWidget::dragEnterEvent(QDragEnterEvent *event)
{
#if 0
	event->ignore();

#else
	if (event->mimeData()->hasFormat("clustering/ImageChar"))
		event->accept();
	else
		event->ignore();
#endif
}

void ClusterSummaryWidget::dragMoveEvent(QDragMoveEvent *event)
{
#if 0
	event->ignore();
#else
    if (event->mimeData()->hasFormat("clustering/ImageChar")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
//        std::cout << event->pos().x() << ", " << event->pos().y() << std::endl;
    }
    else
        event->ignore();
#endif
}

void ClusterSummaryWidget::dropEvent(QDropEvent *event)
{
	std::cout << "dropping elemnents!!" << std::endl;
#if 0
	event->ignore();
#else
    if (event->mimeData()->hasFormat("clustering/ImageChar")) {
        QByteArray pieceData = event->mimeData()->data("clustering/ImageChar");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);

        CharClustIt senderIt;
        quint64 address;
        dataStream >> address;
        senderIt = *(CharClustIt *)(address);

        int nElements;
        dataStream >> nElements;

        std::vector<ImageCharIt> iterators;
        for (int i=0; i<nElements; ++i) {
        	dataStream >> address;

        	ImageCharIt *pIt = (ImageCharIt *) address;
        	iterators.push_back(*pIt);
        }
        // move elemnts:
//        std::cout << "moving elements from " << senderID << " to " << this->mClusterID << std::endl;
        emit moveToCluster(senderIt, this->mClustIt, iterators);

        // accept event
    	event->setDropAction(Qt::MoveAction);
    	event->accept();
    } // end if accept
    else {
        event->ignore();
    }
#endif
    return;
} // end dropEvent

} // end of namespace impact_tr4
