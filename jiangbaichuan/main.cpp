#include "detect.h"
#include "detect.cpp"

int main(){
    VideoCapture cap;
    cap.open("/home/jiangbaichuan/test/armor/1.mp4");
    if(!cap.isOpened()){
        cout<<"摄像头打不开\n"<<endl;
        return -1;
    }
    Mat frame;
    Mat adjust;
    ArmorDetector detect;
    while (1)
    {
        cap>>frame;
        if(frame.empty()){
            break;
        }
        imshow("first",frame);
        brightAdjust(frame,frame,1,-175);
        Rect roi = detect.GetRoi(frame);
        detect.DetectArmor(frame,roi);
        // imshow("adjust",adjust);
        waitKey(30);


    }
    return 0;
    
    
}