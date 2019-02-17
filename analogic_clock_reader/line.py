import cv2
import numpy as np

img = cv2.imread('clock.jpg')

gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

edges = cv2.Canny(gray,50,150,apertureSize = 3)

ret,thresh = cv2.threshold(edges,127,255,cv2.THRESH_BINARY)


lines = cv2.HoughLines(thresh,1,np.pi/180,70)

kernel = np.ones((20,20),np.uint8)

xx1 = 0
xx2 = 0
yy1 = 0
yy2 = 0

img2 = np.ones((255,255),dtype=np.uint8) * 255

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


        img3 = np.ones(gray.shape,dtype=np.uint8) * 255
        if ((abs(x2) >  abs(xx2) + 10)):
            cv2.line(img3,(x1,y1),(x2,y2),(0,0,255),2)
            img3 = cv2.morphologyEx(img3, cv2.MORPH_OPEN, kernel)
            """contours, hierarchy = cv2.findContours(img3,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
            for contour in contours:
                (x,y,w,h) = cv2.boundingRect(contour)
                cv2.drawContours(img, contours, -1, (0,255,0), 3)
                ret,thresh = cv2.threshold(img3,0,1,cv2.THRESH_BINARY)
                #cv2.rectangle(img, (x,y), (x+w,y+h), (0, 255, 0), 2)
                print(thresh)"""

            gray = gray * img3
            #cv2.imshow('Img', img3)
            cv2.imshow('Img', gray)
            cv2.waitKey(0)
            cv2.destroyAllWindows()
            print(x2,x1,y2,y1)
        xx1 = x1
        xx2 = x2
        yy1 = y1
        yy2 = y2


# computes the bounding box for the contour, and draws it on the frame,

"""
cv2.imshow("img",img)
cv2.waitKey(0)
cv2.destroyAllWindows()"""
