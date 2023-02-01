//#include "imageviewer.h"

//ImageViewer::ImageViewer(QWidget *parent)
//   : QWidget(parent), ui(UI::ImageViewer)
//{
//    ui->setupUI(this);

//    ui->scrollArea->setBackgroundRole(QPalette::Dark);

//    // set the scrollArea as imageLabel
//    ui->imageLabel->setParent(this);
//    delete ui->scrollArea->takeWidget();
//    ui->scrollAreaWidgetContents = nullptr;
//    ui->scrollArea->setWidget(ui->imageLabel);

//    // actions to the buttons
//    connect(ui->zoomInButton, SIGNAL(pressed()), SLOT(zoomIn()));
//    connect(ui->zoomOutButton, SIGNAL(pressed()), SLOT(zoomOut()));
//    connect(ui->fitToWindowButton, SIGNAL(pressed()), SLOT(fitToWindowButton()));
//}

//ImageViewer::~ImageViewer()
//{
//    delete ui;
//}


//void ImageViewer::setFilePath(const QString& filePath)
//{
//    t_filePath = filePath;
//}

//bool ImageViewer::loadImage()
//{
//    if(t_filePath.isEmpty())
//    {
//        return false;
//    }

//    if(t_image.load(t_filePath))
//    {
//        t_scaleFactor = 1.0;
//        setScaledImage();
//        return true;
//    }
//    return false;
//}

//void ImageViewer::setScaledImage()
//{
//    QPixmap scaledPixmap = scaledImage;
//    if(!scaledPixmap.isNull())
//    {
//        ui->imageLabel->setFixedSize(scaledPixmap.size());
//        ui->imageLabel->setPixmap(scaledPixmap);
//    }
//}

//QPixmap ImageViewer::scaledImage()
//{
//    QSize scaledSize = getScaledSize();
//    QPixmap scaledPixmap;
//    if(!t_image.isNull()) {
//        scaledPixmap = t_image.scaled(scaledSize, QT::KeepAspectRatio, QT::SmoothTransformation);
//    }
//    return scaledPixmap;
//}

//QSize ImageViewer::getScaledSize()
//{
//    QSize scaledSize = size();
//    scaledSize.setHeight(scaledSize.height() - ui->toolbarFrame->height());
//    scaledSize *= t_scaleFactor;
//    scaledSize.setWidth(scakedSize.width() - 5);
//    scaledSize.setHeight(scaledSize.height() - 5);
//    return scaledSize;
//}

//void ImageViewer::resizeEvent(QResizeEvent *event)
//{
//    QWidget::resizeEvent(event);
//    setScaledImage();
//}

//void ImageViewer::wheelEvent(QWheelEvent *event)
//{
//    int delta = event->delta();
//    if(QApplication::keyboardModifiers() & QT::ControlModifier)
//    {
//        if(delta > 0)
//        {
//            zoomIn();
//        }
//        else
//        {
//            zoomOut();
//        }
//    }
//}

//void ImageViewer::zoomIn()
//{
//    t_scaleFactor += 0.3;
//    setScaledImage();
//}

//void ImageViewer::zoomOut()
//{
//    t_scaleFactor -= 0.3;
//    setScaledImage();
//}

//void ImageViewer::fitToWindow()
//{
//    t_scaleFactor = 1.0;
//    setScaledImage();
//}
