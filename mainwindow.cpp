#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	cv::Mat cvimage;

	cvimage = cv::imread("jessica.png");
	cv::Mat converedImage = cvimage;

	QPixmap tempimage = QPixmap::fromImage(QImage((unsigned char*) cvimage.data, cvimage.cols, cvimage.rows, QImage::Format_RGB888).rgbSwapped());

	//image->DataPtr = tempimage.data_ptr();

	//ui->horizontalLayout->addItem(image);
	ui->imagelabel->setPixmap(tempimage);


	//connect(ui->mybutton,SIGNAL(pressed()), ui->text1,SLOT(clear()));

	connect(ui->parameterContrastDivisor,SIGNAL(valueChanged(int)),this,SLOT(changeContrastDivisor(int)));
	connect(this,SIGNAL(triggerAutomaticUpdate()),this,SLOT(automaticUpdate()));
	connect(ui->buttonUpdate,SIGNAL(pressed()),this,SLOT(manualUpdate()));

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
	contrastDivisor = ((float)value) / 10;
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
}