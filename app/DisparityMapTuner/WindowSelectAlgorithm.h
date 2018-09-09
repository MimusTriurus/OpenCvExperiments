#ifndef WINDOWSELECTALGORITHM_H
#define WINDOWSELECTALGORITHM_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>

class WindowSelectAlgorithm : public QWidget {
    Q_OBJECT
public:
    explicit WindowSelectAlgorithm( QWidget *parent = nullptr );
private:
    QVBoxLayout _layout;
    QPushButton _btnStereoBM    { "StereoBM" };
    QPushButton _btnStereoSGBM  { "StereoSGBM" };
    QPushButton _btnGpuStereoBM { "GpuStereoBM" };

    QPushButton _btnExit        { "Exit" };
private slots:
    void createStereoBM( );
    void createStereoSGBM( );
    void createGpuStereoBm( );
};

#endif // WINDOWSELECTALGORITHM_H
