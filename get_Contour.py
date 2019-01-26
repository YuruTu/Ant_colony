##############
#图像中的轮廓提取(这一次我采用了蚁群算法，后面我后带来更多的算法)
#时间：2019/1/19
#作者：cclplus
#仅供学习交流使用
#如若照片本人认为侵犯了肖像权,请联系我的邮箱707101557@qq.com
#如有疑问或者需求，可以联系作者的邮箱
#如果你有什么好的建议或者指导，我将不胜感激


import cv2
from ctypes import *
from matplotlib import pyplot as plt
import numpy as np

dll=WinDLL('Contour_extraction_dll.dll')

class CImg(Structure):
    _fields_=[("data",c_int*1000*1000)]#声明一个足够大的数组先

if __name__=="__main__":
    imgGray = cv2.imread('./test.jpg',0)#以灰度模式加载一幅彩色的图片

    #对图片进行压缩----^_^----顶-----------------------------------------------
    height = int(imgGray.shape[0]/4)    
    width = int(imgGray.shape[1]/4)     
    imgGray = cv2.resize(imgGray, (width, height), interpolation=cv2.INTER_CUBIC)
    

    img = CImg()#实例化
    temp_h = c_int(height)
    temp_w = c_int(width)
    for i in range(height):
        for j in range(width):
            img.data[i][j] =c_int(int(imgGray[i][j]))
    #print(height,width)
    dll.get_data(byref(img),byref(temp_h),byref(temp_w))
    dll.Ana() 
    for i in range(height):
        for j in range(width):
            imgGray[i][j] =int(img.data[i][j])
    
    plt.imshow(imgGray, cmap = 'gray')#因图像格式问题，暂已灰度输出
    plt.show()


