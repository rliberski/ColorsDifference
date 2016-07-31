#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QImage>
#include <QColor>
#include <QProgressBar>
#include <QtMath>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString open(QGraphicsView *Podglad);
    void save(QString fileName);
    void run();

private slots:
    void on_Run_clicked();
    void on_Open1_clicked();
    void on_Open2_clicked();
    void on_Close_triggered();
    void on_Save_triggered();
    void on_SaveAs_triggered();

private:
    Ui::MainWindow *ui;

    QString file1Directory;
    QString file2Directory;
    QString resultImageDirectory;
    QImage resultImage;


};

#endif // MAINWINDOW_H

