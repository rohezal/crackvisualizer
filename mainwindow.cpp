#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <crackdetection/parameters.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"), "~", tr("Image Files (*.png)"));

	cv::Mat temp = cv::imread(fileName.toStdString().c_str());
	cv::cvtColor(temp, inputImage, cv::COLOR_RGB2BGR);

	//QPixmap tempimage = QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888).rgbSwapped());
	QPixmap tempimage = QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888));

	//image->DataPtr = tempimage.data_ptr();

	//ui->horizontalLayout->addItem(image);
	ui->imagelabel->setPixmap(tempimage);


	//connect(ui->mybutton,SIGNAL(pressed()), ui->text1,SLOT(clear()));

	connect(ui->parameterContrastDivisor,SIGNAL(valueChanged(int)),this,SLOT(changeContrastDivisor(int)));
	connect(this,SIGNAL(triggerAutomaticUpdate()),this,SLOT(automaticUpdate()));
	connect(ui->buttonUpdate,SIGNAL(pressed()),this,SLOT(manualUpdate()));
	connect(ui->slider_constrast_divisor,SIGNAL(sliderMoved(int)),ui->parameterContrastDivisor,SLOT(setValue(int)));
	connect(ui->slider_constrast_divisor,SIGNAL(sliderReleased()),this,SLOT(sliderConstrastDivisorReleased()));
	connect(this,SIGNAL(triggerManualUpdate()),this,SLOT(manualUpdate()));

	this->imageviewer = new ImageViewer(tempimage, nullptr);
	this->imageviewerCombined = new ImageViewer(tempimage, nullptr);

	ui->imageLayout->addWidget(this->imageviewer);
	ui->imageLayout->addWidget(this->imageviewerCombined);

	ui->labelContrastDivisor->setToolTip("High values lowers the amount of constract increase, contrast_improved_value = pixel_value * sqrt(pixel_value) / contrast_divisor.");

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::changeText()
{
	ui->text1->setText("Button was clicked");
}

void MainWindow::changeContrastDivisor(int value)
{
	contrast_divisor = ((float)value) / 10;
	emit triggerAutomaticUpdate();
}

void MainWindow::automaticUpdate()
{
	if(ui->liveUpdatecheckBox->isChecked())
	{
		manualUpdate();
	}
}

void MainWindow::manualUpdate()
{
	ui->debuglabel->setText(QString::number(contrastDivisor));
	//this->imageviewer->setPixMap(QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888).rgbSwapped()));

	contrastImageFilled = crackDetectionRash(inputImage,contrastImage);

	cv::cvtColor(contrastImageFilled, combinedImage , CV_GRAY2RGB);

	std::cout << inputImage.rows << " " << inputImage.cols << std::endl;
	std::cout << contrastImageFilled.rows << " " << contrastImageFilled.cols << std::endl;

	combinedImage = combinedImage*2 + inputImage;


	imageviewer->setPixMap(QPixmap::fromImage(QImage((unsigned char*) contrastImageFilled.data, contrastImageFilled.cols, contrastImageFilled.rows, QImage::Format_Grayscale8)));
	imageviewerCombined->setPixMap(QPixmap::fromImage(QImage((unsigned char*) combinedImage.data, combinedImage.cols, combinedImage.rows, QImage::Format_RGB888)));

}

void MainWindow::sliderConstrastDivisorReleased()
{
	//ui->parameterContrastDivisor->setValue(ui->slider_constrast_divisor->value());
	//contrastDivisor = ((float)ui->slider_constrast_divisor->value()) / 10;
	emit triggerManualUpdate();

}

void MainWindow::exportParameters()
{
	return;
}
