#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>
#include <opencv2/imgproc.hpp>

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



private:
	Ui::MainWindow *ui;

	float contrastDivisor;

signals:
	void triggerAutomaticUpdate();
	void triggerManualUpdate();

};

#endif // MAINWINDOW_H
