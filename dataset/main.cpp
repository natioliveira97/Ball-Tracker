#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <math.h>

class mouseData{

public:
    int mouseX;
    int mouseY;
    int mouseClickX;
    int mouseClickY;
    cv::Mat image;
    std::string windowName;  
};

void drawSquare(int x, int y, int mouseClickX, int mouseClickY, cv::Mat image, std::string windowName){
    int l1 = x-mouseClickX;
    int l2 = y-mouseClickY;
    int l = std::max(abs(l1),abs(l2));
    cv::Mat imageClone = image.clone();

    if(l1!=0 && l2!=0){
        cv::Point p1 = cv::Point(mouseClickX, mouseClickY);
        cv::Point p2 = cv::Point(mouseClickX + (int)(l*l1/abs(l1)), mouseClickY + (int)(l*l2/abs(l2)));;

       // std::cout << "(" << x << "," << y << ") (" << mouseClickX << "," << mouseClickY << ")" << std::endl;
       // std::cout << l1 << " " << l2 << std::endl;
        cv::rectangle(imageClone, p1, p2, cv::Scalar(0,0,255),1,8,0);
    }
    cv::imshow(windowName, imageClone);
}

void onMouse(int event, int x, int y, int flags, void* userdata){

    mouseData *mouse = (mouseData*)userdata;

    if(event == cv::EVENT_LBUTTONDOWN){
        //std::cout << "1" << std::endl;
        if(mouse->mouseClickX == -1 && mouse->mouseClickY == -1){
            mouse->mouseClickX = x;
            mouse->mouseClickY = y;
        }
        else if(mouse->mouseClickX != -1 && mouse->mouseClickY != -1){
            //Escrever no arquivo
            mouse->mouseClickX = -1;
            mouse->mouseClickY = -1;
        }
    }

    if(event == cv::EVENT_MOUSEMOVE){
        //std::cout << "2" << std::endl;
        //std::cout << mouse->mouseClickY << std::endl;

        if(mouse->mouseClickX != -1 && mouse->mouseClickY != -1){
            //std::cout << "3" << std::endl;
            drawSquare(x, y, mouse->mouseClickX, mouse->mouseClickY, mouse->image, mouse->windowName);
        }
    }
}

void createDataset(){

    cv::VideoCapture capture;
    cv::Mat frame;
    std::string videoName = "LARC.avi";
    std::string windowName = videoName;
    std::string datasetName;

    mouseData mouse;
    mouse.mouseClickX = -1;
    mouse.mouseClickY = -1;
    mouse.windowName = windowName;

    capture.open(videoName);
    if(!capture.isOpened()){
        std::cout << "Não foi possível abrir o video!" << std::endl;
        while (std::cin.get() != '\n');
        return;
    }

    while(1){
        capture >> frame;
        if(!frame.data){
            break;
        }

        cv::namedWindow(windowName);
        cv::imshow(windowName, frame);

        mouse.image = frame;
        cv::setMouseCallback(windowName, onMouse, &mouse);


        cv::waitKey(0);
    }

}

int main(){
    createDataset();
}