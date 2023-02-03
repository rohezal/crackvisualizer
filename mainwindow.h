#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "imageviewer.h"
#include <crackdetection/functions.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void changeText();
	void changeContrastDivisor(int value);
	void automaticUpdate();
	void manualUpdate();
	void sliderConstrastDivisorReleased();
	void exportParameters();



private:
	Ui::MainWindow *ui;

	ImageViewer* imageviewer;
	ImageViewer* imageviewerContrast;
	ImageViewer* imageviewerCombined;
	float contrastDivisor;
	cv::Mat inputImage;
	cv::Mat contrastImage;
	cv::Mat contrastImageFilled;
	cv::Mat combinedImage;

signals:
	void triggerAutomaticUpdate();
	void triggerManualUpdate();

};

#endif // MAINWINDOW_H
