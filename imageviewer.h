//#ifndef IMAGEVIEWER_H
//#define IMAGEVIEWER_H

//#include <QObject>
//#include <QMainWindow>

//namespace Ui {
//class ImageViewer;
//}

//class ImageViewer : public QMainWindow
//{
//    Q_OBJECT

//public:
//    ImageViewer(QWidget *parent = nullptr);
//    bool loadFile(const QString &);

//private slots:
//    void open();
//    void zoomIn();
//    void zoomOut();
//    void normalSize();
//    void fitToWindow();

//private:
//    void createActions();
//    void createMenus();
//    void updateActions();
//    void setImage(const QImage &newImage);
//    void scaleImage(double factor);
//    void adjustScrollBar(QScrollBar *scrollBar, double factor);

//    QImage image;
//    QLabel *imageLabel;
//    QScrollArea *scrollArea;
//    double scaleFactor = 1;

//    QAction *zoomInAct;
//    QAction *zoomOutAct;
//    QAction *normalSizeAct;
//    QAction *fitToWindowAct;
//};

//#endif // IMAGEVIEWER_H
