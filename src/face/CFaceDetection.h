/************************************************************************/
/*
* fuction: 主要用于实现人脸的探测，识别图片中人脸的区域位置
* author: lihaiping1603@aliyun.com
* create date:2018-02-24
*/
/************************************************************************/

#ifndef __CFACEDETECTION_H__
#define __CFACEDETECTION_H__

#include <stdint.h>
#include <vector>
#include "../../Global/global.h"

using namespace std;


namespace PublicTool {
    class CFaceDetection{
    public:
        CFaceDetection();
        ~CFaceDetection();

        //放入探测图片
        int PutImage(const char* szImgPath);
        //获取人脸数量
        int GetFaceSize();
        //获取所有人脸的区域
        int GetFaceRect(vector<Rect_S> &vecFaceRect);
        //获取图片的宽和高
        int GetResolution(int& nWidth,int& nHeight);
    private:
        //存储人脸区域
        vector<Rect_S> m_vecFaceRect;
        int m_nWidth;//图像的宽
        int m_nHeiht;//图像的高
    };
}

#endif
