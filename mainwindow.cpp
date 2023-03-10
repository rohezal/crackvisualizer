#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <crackdetection/parameters.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QStringList argv = QCoreApplication::arguments();
	QString fileName = "";

	if(argv.size() == 2)
	{
		fileName = argv[1];
	}

	else
	{
		fileName = QFileDialog::getOpenFileName(this,tr("Open Image"), "~", tr("Image Files (*.png)"));
	}

	cv::Mat temp = cv::imread(fileName.toStdString().c_str());
	cv::cvtColor(temp, inputImage, cv::COLOR_RGB2BGR);

	//QPixmap tempimage = QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888).rgbSwapped());
	QPixmap tempimage = QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888));

	//image->DataPtr = tempimage.data_ptr();

	//ui->horizontalLayout->addItem(image);


	//connect(ui->mybutton,SIGNAL(pressed()), ui->text1,SLOT(clear()));

	connect(ui->parameterContrastDivisor,SIGNAL(valueChanged(int)),this,SLOT(changeContrastDivisor(int)));

	connect(ui->parameterContrastSubstractor,SIGNAL(valueChanged(int)),this,SLOT(change_contrast_subtractor_divisor(int)));
	connect(ui->parameterHSVSubtractor,SIGNAL(valueChanged(int)),this,SLOT(change_contrast_hsv_substractor_divisor(int)));
	connect(ui->parameterSobelTresholdAdder,SIGNAL(valueChanged(int)),this,SLOT(change_preprocessing_treshold_adder(int)));
	connect(ui->parameterSobelTresholdDivisor,SIGNAL(valueChanged(int)),this,SLOT(change_preprocessing_treshold_divisor(int)));

	//connect slider and extra buttons

	connect(this,SIGNAL(triggerAutomaticUpdate()),this,SLOT(automaticUpdate()));
	connect(ui->buttonUpdate,SIGNAL(pressed()),this,SLOT(manualUpdate()));
	connect(ui->slider_constrast_divisor,SIGNAL(sliderMoved(int)),ui->parameterContrastDivisor,SLOT(setValue(int)));
	connect(ui->slider_constrast_divisor,SIGNAL(sliderReleased()),this,SLOT(sliderConstrastDivisorReleased()));
	connect(this,SIGNAL(triggerManualUpdate()),this,SLOT(manualUpdate()));
	connect(ui->overlayCheckbox,SIGNAL(clicked(bool)),this,SLOT(switchOverlay()));
	connect(ui->exportButton,SIGNAL(pressed()),this,SLOT(exportParameters()));

	//connect save and load
	connect(ui->buttonSave,SIGNAL(pressed()),this,SLOT(save()));
	connect(ui->buttonLoad,SIGNAL(pressed()),this,SLOT(load()));

	this->imageviewer = new ImageViewer(tempimage, nullptr);
	this->imageviewerContrast = new ImageViewer(tempimage, nullptr);
	this->imageviewerCombined = new ImageViewer(tempimage, nullptr);

	ui->imageLayout->addWidget(this->imageviewer);
	ui->imageLayout->addWidget(this->imageviewerContrast);
	ui->imageLayout->addWidget(this->imageviewerCombined);

	ui->labelContrastDivisor->setToolTip("High values lowers the amount of constract increase, contrast_improved_value = pixel_value * sqrt(pixel_value) / contrast_divisor.");

	manualUpdate();
	imageviewerContrast->enableRightClick();
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

void MainWindow::change_preprocessing_treshold_divisor(int value)
{
	preprocessing_treshold_divisor = ((float)value) / 10;
	emit triggerAutomaticUpdate();
}

void MainWindow::change_contrast_subtractor_divisor(int value)
{
	contrast_subtractor_divisor = ((float)value) / 10;
	emit triggerAutomaticUpdate();
}

void MainWindow::change_preprocessing_treshold_adder(int value)
{
	preprocessing_treshold_adder = ((float)value) / 10;
	emit triggerAutomaticUpdate();
}

void MainWindow::change_contrast_hsv_substractor_divisor(int value)
{
	contrast_hsv_substractor_divisor = ((float)value) / 10;
	emit triggerAutomaticUpdate();
}

//preprocessing_treshold_divisor
//contrast_subtractor_divisor
//preprocessing_treshold_adder
//contrast_hsv_substractor_divisor


void MainWindow::automaticUpdate()
{
	if(ui->liveUpdatecheckBox->isChecked())
	{
		manualUpdate();
	}
}

Mat MainWindow::colorizeIslandImage(Mat &labelImage, int nLabels)
{
	std::vector<Vec3b> colors(nLabels);
	colors[0] = Vec3b(0, 0, 0);//background
	for(int label = 1; label < nLabels; ++label){
		colors[label] = Vec3b( label%3==0? 50+label*10%256 : (label%3==1 ? label*5%256 : label) , label%3==1? 50+label*10%256 : (label%3==2 ? label*5%256 : label), 50+label%3==2 ? label*10%256 : (label%3==0 ? label*5%256 : label));
	}
	Mat dst(inputImage.size(), CV_8UC3);
	for(int r = 0; r < dst.rows; ++r){
		for(int c = 0; c < dst.cols; ++c){
			int label = labelImage.at<int>(r, c);
			Vec3b &pixel = dst.at<Vec3b>(r, c);
			pixel = colors[label];
		 }
	 }

	cv::Mat temp;
	cv::cvtColor(dst, temp, cv::COLOR_RGB2BGR);


	return temp;
}


void MainWindow::manualUpdate()
{
	//this->imageviewer->setPixMap(QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888).rgbSwapped()));

	//contrastImageFilled = crackDetectionRash(inputImage,contrastImage);

	IslandData data;

	cv::cvtColor(inputImage, contrastImage, CV_BGR2GRAY);

	calculateIslandImage(contrastImage,data);

	std::cout << "Labels: " << data.number_of_labels << std::endl;

	contrastImageFilled = colorizeIslandImage(data.islandImage,data.number_of_labels);
	combinedImage = contrastImageFilled.clone();

	/*
	String windowName = "The Guitar"; //Name of the window
	namedWindow(windowName); // Create a window
	imshow(windowName, contrastImageFilled); // Show our image inside the created window.
	waitKey(0); // Wait for any keystroke in the window
	*/

	combinedImage = combinedImage*2 + inputImage/10;


	imageviewerContrast->setPixMap(QPixmap::fromImage(QImage((unsigned char*) contrastImageFilled.data, contrastImageFilled.cols, contrastImageFilled.rows, QImage::Format_RGB888)));
	imageviewerCombined->setPixMap(QPixmap::fromImage(QImage((unsigned char*) combinedImage.data, combinedImage.cols, combinedImage.rows, QImage::Format_RGB888)));

	switchOverlay();

	//calculateIslands
}

void MainWindow::sliderConstrastDivisorReleased()
{
	//ui->parameterContrastDivisor->setValue(ui->slider_constrast_divisor->value());
	//contrastDivisor = ((float)ui->slider_constrast_divisor->value()) / 10;
	emit triggerManualUpdate();

}

void MainWindow::exportParameters()
{
	QString message;
	message =
	"--contrast_divisor " + QString::number(contrast_divisor) + " " +
	"--preprocessing_treshold_divisor " +QString::number(preprocessing_treshold_divisor) + " " +
	"--contrast_subtractor_divisor " +QString::number(contrast_subtractor_divisor) + " " +
	"--preprocessing_treshold_adder " +QString::number(preprocessing_treshold_adder) + " " +
	"--contrast_hsv_substractor_divisor " +QString::number(contrast_hsv_substractor_divisor) + " ";

	QGuiApplication::clipboard()->setText(message);

	QMessageBox msgBox;
	msgBox.setText(message);
	msgBox.exec();


	//contrast_divisor
	//preprocessing_treshold_divisor
	//contrast_subtractor_divisor
	//preprocessing_treshold_adder
	//contrast_hsv_substractor_divisor

}

void MainWindow::switchOverlay()
{
	if(ui->overlayCheckbox->isChecked())
	{
		imageviewer->setPixMap(QPixmap::fromImage(QImage((unsigned char*) combinedImage.data, combinedImage.cols, combinedImage.rows, QImage::Format_RGB888)));
	}
	else
	{
		imageviewer->setPixMap(QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888)));
	}
}

void MainWindow::save()
{
	saved_contrast_divisor = contrast_divisor;
	saved_contrast_subtractor_divisor=contrast_subtractor_divisor;
	saved_contrast_hsv_substractor_divisor=contrast_hsv_substractor_divisor;
	saved_preprocessing_treshold_divisor=preprocessing_treshold_divisor;
	saved_preprocessing_treshold_adder=preprocessing_treshold_adder;
}

void MainWindow::load()
{
	ui->parameterContrastDivisor->setValue(saved_contrast_divisor*10);
	ui->parameterContrastSubstractor->setValue(saved_contrast_subtractor_divisor*10);
	ui->parameterHSVSubtractor->setValue(saved_contrast_hsv_substractor_divisor*10);
	ui->parameterSobelTresholdAdder->setValue(saved_preprocessing_treshold_adder*10);
	ui->parameterSobelTresholdDivisor->setValue(saved_preprocessing_treshold_divisor*10);
	manualUpdate();

}
