#include "WindowSelectAlgorithm.h"
#include "WindowStereoBM.h"
#include "WindowStereoSGBM.h"
#include "WindowGpuStereoBM.h"
#include <QMessageBox>

WindowSelectAlgorithm::WindowSelectAlgorithm( QWidget *parent ) : QWidget( parent ) {
    this->setLayout( &_layout );
    this->setWindowFlags(Qt::WindowTitleHint);
    _layout.addWidget( &_btnStereoBM );
    _layout.addWidget( &_btnStereoSGBM );
    _layout.addWidget( &_btnGpuStereoBM );

    _layout.addWidget( &_btnExit );

    connect( &_btnStereoBM, SIGNAL( clicked( bool ) ), SLOT( createStereoBM( ) ) );
    connect( &_btnStereoSGBM, SIGNAL( clicked( bool ) ), SLOT( createStereoSGBM( ) ) );
    connect( &_btnGpuStereoBM, SIGNAL( clicked( bool ) ), SLOT( createGpuStereoBm( ) ) );

    connect( &_btnExit, &QPushButton::clicked, [ ](){
        QMessageBox message;
        message.setText( "Are you sure?" );
        message.setStandardButtons( QMessageBox::Ok | QMessageBox::Cancel );
        if ( message.exec( ) == 1024 ) {
            qApp->exit( );
        }
    } );
}

void WindowSelectAlgorithm::createStereoBM( ) {
//    this->close( );
    auto w = new WindowStereoBM( );
    w->show( );
}

void WindowSelectAlgorithm::createStereoSGBM( ) {
//    this->close( );
    auto w = new WindowStereoSGBM( );
    w->show( );
}

void WindowSelectAlgorithm::createGpuStereoBm( ) {
//    this->close( );
    auto w = new WindowGpuStereoBM( );
    w->show( );
}
