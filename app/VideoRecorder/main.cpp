#include <QCoreApplication>
#include "opencv2/opencv.hpp"

std::string makeFilePath( const std::string &path, const std::string &fileName, const std::string &ext, int index ) {
    QString result;
    result.append( QString::fromUtf8( path.c_str( ) ) );
    result.append( "/" );
    result.append( QString::fromUtf8( fileName.c_str( ) ) );
    result.append( QString::number( index ) );
    result.append( "." );
    result.append( QString::fromUtf8( ext.c_str( ) ) );
    return result.toStdString( );
}

int main( int argc, char *argv[ ] ) {
    QCoreApplication a( argc, argv );

    const cv::String keys =
    "{help h usage ? |                  |print help message}"
    "{hFrame         |480               |Height of the camera frame}"
    "{wFrame         |640               |Width of the camera frame}"
    "{imgsFolder     |./imgs            |Directory of video frames}"
    "{preNameL       |img_l_            |Left video frame name prefix.}"
    "{preNameR       |img_r_            |Right video frame name postfix.}"
    "{imgExt         |jpg               |Video extension. Ext: avi, etc}"
    "{cam1Ind        |0                 |Camera 1 Index}"
    "{cam2Ind        |1                 |Camera 2 Index}";

    cv::CommandLineParser parser( argc, argv, keys );
    parser.about("ImgGrabber v1.0.0");
    if ( parser.has( "help" ) ) {
        parser.printMessage( );
        return 0;
    }

    cv::String imgsFolder = parser.get<cv::String>( "imgsFolder" );
    cv::String imgExt     = parser.get<cv::String>( "imgExt" );
    cv::String preNameL   = parser.get<cv::String>( "preNameL" );
    cv::String preNameR   = parser.get<cv::String>( "preNameR" );
    int wFrame            = parser.get<int>       ( "wFrame" );
    int hFrame            = parser.get<int>       ( "hFrame" );

    Q_UNUSED( wFrame )
    Q_UNUSED( hFrame )

    int cam1Ind           = parser.get<int>       ( "cam1Ind" );
    int cam2Ind           = parser.get<int>       ( "cam2Ind" );

    std::cout << "output imgs dir: " << imgsFolder << std::endl;

    cv::VideoCapture camera1( cam1Ind );
    cv::VideoCapture camera2( cam2Ind );

    if ( !camera1.isOpened( ) || !camera2.isOpened( ) ) {
      std::cout << "error opening cameras: " << cam1Ind << " " << cam2Ind << std::endl;
      return -1;
    }

    std::cout << "press R key for record video" << std::endl;
    const int R_PRESSED = 114; // record key code
    const int DELAY = 30;

    bool record = false;

    cv::Mat img1, img2;
    //cv::Size videoResolution( wFrame, hFrame );
    int counter = 0;
    while ( true ) {
        camera1 >> img1;
        camera2 >> img2;
        if ( img1.empty( ) || img2.empty( ) ) {
            cv::waitKey( DELAY );
            std::cout << "empty frame" << std::endl;
            continue;
        }
        imshow( "left" , img1 );
        imshow( "right", img2 );

        const int KEY_PRESSED = cv::waitKey( DELAY );
        if ( KEY_PRESSED > 0 )
            std::cout << "Key pressed:" << KEY_PRESSED << std::endl;
        if ( KEY_PRESSED == R_PRESSED ) {
            record = !record;
        }
        if ( record ) {
            std::cout << "Record!" << makeFilePath( imgsFolder, preNameL, imgExt, counter ) <<  std::endl;
            cv::imwrite( makeFilePath( imgsFolder, preNameL, imgExt, counter ), img1 );
            cv::imwrite( makeFilePath( imgsFolder, preNameR, imgExt, counter ), img2 );
            ++counter;
        }
        else {
            cv::waitKey( DELAY );
        }
    }
    camera1.release( );
    camera2.release( );
    return a.exec( );
}
