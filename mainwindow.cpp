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
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::changeText()
{
	ui->text1->setText("Button was clicked");
}
