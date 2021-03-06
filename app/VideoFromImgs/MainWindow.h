#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QLayout>
#include <QLabel>
#include <QFileInfo>
#include <QFileInfoList>

#include "opencv2/opencv.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow( );
private:
    Ui::MainWindow *ui;

    QGridLayout _layout;

    QLabel _lblLeftImgsDir;
    QLabel _lblRightImgsDir;
    QLabel _lblCalibFiles;
    QLabel _lblOutputDir;

    QPushButton _btnLeftImgsDir;
    QPushButton _btnRightImgsDir;
    QPushButton _btnOpenCalibFiles;
    QPushButton _btnOutputDir;
    QPushButton _btnStart;

    QString _stereoVideoDirPath;

    QFileInfoList _leftImgsList;
    QFileInfoList _rightImgsList;

    QFileInfoList getImgsFileInfo( const QString &dirPath );

    cv::Mat _leftIntr, _rightIntr;
    cv::Mat _leftDist, _rightDist;
};

#endif // MAINWINDOW_H
