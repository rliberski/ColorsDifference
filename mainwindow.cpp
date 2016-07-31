#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*opening file*/
QString MainWindow::open(QGraphicsView *view)
{
    //opening file dialog. you can load jpg,bmp or png image
    QString fileDirectory=QFileDialog::getOpenFileName(this,
            tr("Open..."),"/home/",tr("JPG Image (*.jpg);;PNG Image (*.png);;Bitmaps (*.bmp)"));
    if(fileDirectory.isEmpty())
        return fileDirectory;

    //create pixel map from choosen file
    QPixmap pixelMap(fileDirectory);
    if(pixelMap.isNull())
        return fileDirectory;

    //send pixel map to graphics view
    QGraphicsScene *scene = new QGraphicsScene(view);
    scene->addPixmap(pixelMap);
    view->setScene(scene);
    view->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    view->show();

    return fileDirectory;
}

/*saving file*/
void MainWindow::save(QString fileName)
{
    if(fileName.isEmpty())
    {
        //if you save file first time or choose "save as..", file dialog will open
        fileName = QFileDialog::getSaveFileName(this,
                tr("Save file as..."),tr("/home/"),tr("JPG Images (*.jpg);;PNG Images (*.png);;Bitmaps (*.bmp)"));
        if(fileName.isEmpty())
            return;
    }

    if(resultImage.isNull())
        return;
    resultImage.save(fileName);
    resultImageDirectory = fileName;
}

/*run*/
void MainWindow::run()
{
    //checking files
    if(file1Directory.isEmpty() || file2Directory.isEmpty())
    {
        QMessageBox error;
        error.setText("Load image first!");
        error.exec();
        return;
    }

    //calculation result image size
    QImage Image1(file1Directory);
    QImage Image2(file2Directory);
    int resultWidth;
    int resultHeight;

    if(Image1.width()>Image2.width())
        resultWidth=Image2.width();
    else
        resultWidth=Image1.width();

    if(Image1.height()>Image2.height())
        resultHeight=Image2.height();
    else
        resultHeight=Image1.height();

    resultImage = QImage(resultWidth,resultHeight,Image1.format());

    //loading bar settings
    ui->Loading->setMinimum(0);
    ui->Loading->setMaximum(resultImage.height()+1);
    int progress=0;

    //needed variables for calculations
    QColor pixel_1;          //single pixel from image 1
    QColor pixel_2;          //single pixel from image 2
    int redDifference;       //(red1 - red2) ^ 2
    int greenDifference;
    int blueDifference;
    double totalDifference;  //sqrt(red+green+blue)

    //calculations
    for(int i=0;i<resultImage.height();i++)
    {
        for(int j=0;j<resultImage.width();j++)
        {
            pixel_1=Image1.pixel(j,i);
            pixel_2=Image2.pixel(j,i);

            redDifference=pow(pixel_1.red()-pixel_2.red(),2);
            greenDifference=pow(pixel_1.green()-pixel_2.green(),2);
            blueDifference=pow(pixel_1.blue()-pixel_2.blue(),2);
            totalDifference=qSqrt(redDifference+greenDifference+blueDifference);

            QRgb color=qRgb(totalDifference/1.73,totalDifference/1.73,totalDifference/1.73);
            resultImage.setPixel(j,i,color);
                                                                //1.73 because:
                                                                //255^2 = 65025; 65025*3 = 195075; sqrt(195075)= about 442
                                                                //442/255 = about 1.73
        }
        progress++;
        ui->Loading->setValue(progress);
    }


    //send result image to graphics view
    resultImage.save("temp.bmp");   //temp file

    QGraphicsScene *scene = new QGraphicsScene(ui->ResultImage);
    QPixmap pixelMap("temp.bmp");
    scene->addPixmap(pixelMap);
    ui->ResultImage->setScene(scene);
    ui->ResultImage->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    ui->ResultImage->show();

    QFile::remove("temp.bmp");

    progress++;
    ui->Loading->setValue(progress);
}

/*Button - Load image*/
void MainWindow::on_Open1_clicked()
{
    file1Directory=open(ui->Image1);
}

/*Button - Load image (2) */
void MainWindow::on_Open2_clicked()
{
    file2Directory=open(ui->Image2);
}

/*Menu - Save */
void MainWindow::on_Save_triggered()
{
    save(resultImageDirectory);
}

/*Menu - Save as */
void MainWindow::on_SaveAs_triggered()
{
    save("");
}

/*Menu - Close*/
void MainWindow::on_Close_triggered()
{
    qApp->exit();
}

/*Button - Run */
void MainWindow::on_Run_clicked()
{
    run();
}
