/*
 * PreprocessingWidget.hpp
 *
 *  Created on: 19.01.2010
 *      
 */

#ifndef PREPROCESSINGWIDGET_HPP_
#define PREPROCESSINGWIDGET_HPP_

#include "ui_PreprocessingWidget.h"

#include "core_headers.h"
#include "image_headers.h"

#include "clustering/DataStructs.hpp"

namespace impact_tr4 {

class MainWindow;

class PreprocessingWidget : public QWidget
{
	Q_OBJECT
public:
	PreprocessingWidget(QWidget *parent = 0, Qt::WFlags f = Qt::Widget);
	~PreprocessingWidget();
	const PreprocessingParameters parameters() const;

public slots:
	void on_preprocessingPreviewButton_clicked();

private:
	Ui::PreprocessingWidget _ui;
	MainWindow *mpMainWindow;

private slots:

signals:
	void preprocessImages();
private:
	QPixmap mPixmap1, mPixmap2;

}; // end of class PreprocessingWidget.hpp

} // end of namespace impact_tr4


#endif /* PREPROCESSINGWIDGET_HPP_ */
