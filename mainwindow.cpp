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


	cv::imwrite("read_temp.png",temp);
	cv::imwrite("read_colorcerversion.png",inputImage);


	QImage image_from_mat = QImage((const unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, inputImage.step, QImage::Format_RGB888);
	image_from_mat.save("read_qimage.png");

	//QPixmap tempimage = QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888).rgbSwapped());
	QPixmap tempimage = QPixmap::fromImage(QImage((const unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, inputImage.step, QImage::Format_RGB888));


	QFile file("read_qpixmap.png");
	file.open(QIODevice::WriteOnly);
	tempimage.save(&file, "PNG");
	file.close();


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

void MainWindow::manualUpdate()
{
	//this->imageviewer->setPixMap(QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888).rgbSwapped()));

	contrastImageFilled = crackDetectionRash(inputImage,contrastImage);

	cv::cvtColor(contrastImageFilled, combinedImage , CV_GRAY2RGB);

	std::cout << inputImage.rows << " " << inputImage.cols << std::endl;
	std::cout << contrastImageFilled.rows << " " << contrastImageFilled.cols << std::endl;

	combinedImage = combinedImage*2 + inputImage;


	imageviewerContrast->setPixMap(QPixmap::fromImage(QImage((unsigned char*) contrastImageFilled.data, contrastImageFilled.cols, contrastImageFilled.rows, QImage::Format_Grayscale8)));
	imageviewerCombined->setPixMap(QPixmap::fromImage(QImage((unsigned char*) combinedImage.data, combinedImage.cols, combinedImage.rows, QImage::Format_RGB888)));

	switchOverlay();
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
