//#ifndef IMAGEVIEWER_H
//#define IMAGEVIEWER_H

//#include <QWidget>

//namespace Ui {
//class ImageViewer;
//}

//class ImageViewer : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit ImageViewer(QWidget *parent = 0);

//    // functions to load the image into the ImageViewer
//    bool loadImage();
//    void setFilePath(const QString& filepath);

//private slots:
//    // buttonActions for zooming
//    void zoomIn();
//    void zoomOut();
//    void fitToWindow();

//protected:
//    // events for resizing window or using the mouswheel
//    void resizeEvent(QResizeEvent *event) override;
//    void wheelEvent(QWheelEvent *event) override;

//private:
//    // functions to scale the image
//    QPixmap scaledImage();
//    void setScaledImage();
//    QSize getScaledSize();

//    // variables for the image
//    UI::ImageViewer *ui;
//    QPixmap t_image;
//    double t_scaleFactor = 1.0;
//    QString t_filePath;

//};

//#endif // IMAGEVIEWER_H
