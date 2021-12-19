#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#ifdef DEBUG_ARMOR_DETECT
// ------ debug armor ------
//#define SHOW_BINARY_IMAGE
//#define SHOW_LIGHT_PUT_TEXT
#define SHOW_ARMOR_PUT_TEXT
#define SHOW_LIGHT_CONTOURS
#define SHOW_ROI_RECTANGLE
#define SHOW_DRAW_SPOT
#define SHOW_DRAW_RECT
// ------ debug armor ------
#endif

#define FAST_DISTANCE
/**
 * @brief 装甲板灯条相关数据信息
 */
class LED_Stick{
public:

    LED_Stick():matched(false){}

    LED_Stick(const RotatedRect& R){
        rect.angle = R.angle;
        rect.center = R.center;
        rect.size = R.size;
        matched = false;
    }

    RotatedRect rect;   // 装甲板灯条相关数据信息
    bool matched;       // 匹配状态， 用于灯条匹配
    size_t match_index; // 匹配对应的灯条序号， 用于灯条匹配
    float match_factor; // 匹配强度， 用于灯条匹配
};

/**
 * @brief 装甲板相关数据信息
 */
class armor{
public:
    armor();
    armor(const LED_Stick& L1, const LED_Stick& L2);

    void draw_rect( Mat& img, Point2f roi_offset_poin) const;    // 画出装甲板
    void draw_spot(Mat &img, Point2f roi_offset_point) const;
    int get_average_intensity(const Mat& img) ; // 计算装甲板roi平均色彩强度，用于筛选装甲板中心有灯条
    void max_match(vector<LED_Stick>& LED, size_t i, size_t j); // 灯条匹配算法
    bool is_suitable_size(void) const;          // 判断可能的装甲板是否符合尺寸

    LED_Stick Led_stick[2];  // 装甲板的两个灯条
    float error_angle;       // 两个灯条的误差的角度
    Point2i center;          // 装甲板中心点
    Rect2i rect;             // 装甲板roi矩形
    int average_intensity;   // 装甲板roi的平均色彩强度
};


/**
 * @brief 装甲板检测器
 */
class ArmorDetector
{
public:
    /**
     * @brief GetRoi 获取图像ROI区域
     * @param img
     * @return 返回感兴趣区域的矩形
     */
    Rect GetRoi(const Mat &img);
    /**
     * @brief DetectArmor 装甲板识别函数
     * @param img
     * @param roi_rect
     * @return
     */
    bool DetectArmor(Mat &img, Rect roi_rect);
private:
    bool makeRectSafe(cv::Rect & rect, cv::Size size){
        if (rect.x < 0)
            rect.x = 0;
        if (rect.x + rect.width > size.width)
            rect.width = size.width - rect.x;
        if (rect.y < 0)
            rect.y = 0;
        if (rect.y + rect.height > size.height)
            rect.height = size.height - rect.y;
        if (rect.width <= 0 || rect.height <= 0)
            return false;
        return true;
    }

    /**
     * @brief 装甲板类型确定
     * 通过检测历史数据判断装甲板类型，大装甲板和小装甲板
     */
    bool getTypeResult(bool is_small);
   

private:
    // 外部参数
    int color_= 0;
    int cap_mode_;

public:
    // 调试参数
    int short_offset_x_ = 100;
    int short_offset_y_ = 100;
    int long_offset_x_ = 100;
    int long_offset_y_ = 100;
    int color_th_ = 16;
    int gray_th_ = 60;


private:
    // ｒｏｉ参数
    Rect last_target_;
    int lost_cnt_ = 0;
    int detect_cnt_ = 0;

};