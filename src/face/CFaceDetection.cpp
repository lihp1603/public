/************************************************************************/
/*
* fuction: 主要用于实现人脸的探测，识别图片中人脸的区域位置
* author: lihaiping1603@aliyun.com
* create date:2018-02-24
*/
/************************************************************************/

#include "CFaceDetection.h"

//#include <cstdint>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../include/SeetaFaceDetection/face_detection.h"
#include "../../Global/global.h"

using namespace std;

namespace PublicTool {

    CFaceDetection::CFaceDetection()
        :m_nWidth(0)
        ,m_nHeiht(0)
    {
        m_vecFaceRect.clear();
    }

    CFaceDetection::~CFaceDetection()
    {

    }

    int CFaceDetection::PutImage( const char* szImgPath )
    {
        const char* img_path = szImgPath;
        //这里需要带入模型
        seeta::FaceDetection detector("F:\\C++\\XML\\demo\\ae_project_parse\\ae_project_parse\\src\\SeetaFaceDetection\\model\\seeta_fd_frontal_v1.0.bin");

        detector.SetMinFaceSize(40);
        detector.SetScoreThresh(2.f);
        detector.SetImagePyramidScaleFactor(0.8f);
        detector.SetWindowStep(4, 4);

        cv::Mat img = cv::imread(img_path, cv::IMREAD_UNCHANGED);
        cv::Mat img_gray;

        if (img.channels() != 1)
            cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
        else
            img_gray = img;

        seeta::ImageData img_data;
        img_data.data = img_gray.data;
        img_data.width = img_gray.cols;
        img_data.height = img_gray.rows;
        img_data.num_channels = 1;

        //long t0 = cv::getTickCount();
        //进行人脸区域探测
        std::vector<seeta::FaceInfo> faces = detector.Detect(img_data);
        //long t1 = cv::getTickCount();
        //double secs = (t1 - t0)/cv::getTickFrequency();

        int num_face = static_cast<int>(faces.size());

        for (int i = 0; i < num_face; i++) {
            Rect_S face_rect;
            face_rect.nX = faces[i].bbox.x;
            face_rect.nY = faces[i].bbox.y;
            face_rect.nWidth = faces[i].bbox.width;
            face_rect.nHeight = faces[i].bbox.height;
            m_vecFaceRect.push_back(face_rect);
        }
        //记录探测图片的宽和高
        m_nWidth=img_data.width;
        m_nHeiht=img_data.height;

        return num_face;
    }

    int CFaceDetection::GetFaceSize()
    {
        return m_vecFaceRect.size();
    }

    int CFaceDetection::GetFaceRect( vector<Rect_S> &vecFaceRect )
    {
        vecFaceRect=m_vecFaceRect;
        return vecFaceRect.size();
    }

    int CFaceDetection::GetResolution( int& nWidth,int& nHeight )
    {
        nWidth=m_nWidth;
        nHeight=m_nHeiht;
        return 0;
    }
}
