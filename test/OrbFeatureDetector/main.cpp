#include <QCoreApplication>
#include <QDebug>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cuda;
using namespace std;

int main( int argc, char *argv[ ] ) {
    QCoreApplication a( argc, argv );

    auto orb = cuda::ORB::create( 500, 1.2f, 8, 31, 0, 2, 0, 31, 20, true );
    auto matcher = cv::cuda::DescriptorMatcher::createBFMatcher( cv::NORM_HAMMING );

    cv::Mat originL = cv::imread( "E:/Projects/Qt/Builds/DisparityMapExperiments/bin/dataset/7l.png", CV_8U );
    cv::Mat originR = cv::imread( "E:/Projects/Qt/Builds/DisparityMapExperiments/bin/dataset/7r.png", CV_8U );
//    cv::imshow( "origin", originL );
    qDebug( ) << "start";
    GpuMat imgL, imgR;
    imgL.upload( originL );
    imgR.upload( originR );
//    GpuMat imgR( imread( "E:/Projects/Qt/Builds/DisparityMapExperiments/bin/dataset/9r.png" ), CV_8U );

    vector<KeyPoint> keypointsL;
    GpuMat descriptorsL;

    vector<KeyPoint> keypointsR;
    GpuMat descriptorsR;

    orb->detectAndCompute( imgL, noArray( ), keypointsL, descriptorsL );
    orb->detectAndCompute( imgR, noArray( ), keypointsR, descriptorsR );

    std::vector<DMatch> matches;
    vector< vector<DMatch> >  matchesArr;

//    matcher->match( descriptorsL, descriptorsR, matches );
    matcher->radiusMatch( descriptorsL, descriptorsR, matchesArr, 15.f );

    Mat result = Mat( originL.rows, originL.cols * 2, CV_8U );
    //drawMatches( originL, keypointsL, originR, keypointsR, matches, result );
    drawMatches( originL, keypointsL, originR, keypointsR, matchesArr, result );
    if ( !result.empty( ) )
        imshow( "result", result );

    qDebug( ) << "end";

    return a.exec( );
}
