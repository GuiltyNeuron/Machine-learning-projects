import cv2
import numpy as np

img = cv2.imread('clock.jpg')

gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

edges = cv2.Canny(gray,50,150,apertureSize = 3)

ret,thresh = cv2.threshold(edges,127,255,cv2.THRESH_BINARY)


lines = cv2.HoughLines(thresh,1,np.pi/180,70)

xx1 = 0
xx2 = 0
yy1 = 0
yy2 = 0


for i in range(len(lines)):
    for rho,theta in lines[i]:
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a*rho
        y0 = b*rho
        x1 = int(x0 + 1000*(-b))
        y1 = int(y0 + 1000*(a))
        x2 = int(x0 - 1000*(-b))
        y2 = int(y0 - 1000*(a))


        if ((abs(x2) >  abs(xx2) + 10)):
            cv2.line(img,(x1,y1),(x2,y2),(0,0,255),2)
            print(x1,y1)
            """im = cv2.getRectSubPix(thresh, (y1, y2), (x1, x2))
            cv.imshow('Img', im)
            cv.waitKey(0)"""
        xx1 = x1
        xx2 = x2
        yy1 = y1
        yy2 = y2


cv2.imshow("img",img)
cv2.waitKey(0)
cv2.destroyAllWindows()
