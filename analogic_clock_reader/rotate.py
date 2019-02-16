import cv2
import numpy as np

kernel = np.ones((1,10),np.uint8)

img =cv2.imread("clock.png")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
ret,thresh = cv2.threshold(gray,100,255,cv2.THRESH_BINARY)
opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)

contours, hierarchy = cv2.findContours(opening,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
# computes the bounding box for the contour, and draws it on the frame,

for contour, hier in zip(contours, hierarchy):
    (x,y,w,h) = cv2.boundingRect(contour)
    print(x,y,w,h)
    if w < 80 and h < 80:
        cv2.rectangle(opening, (x,y), (x+w,y+h), (255, 0, 0), 2)
        print(x,y,w,h)

cv2.imshow("img",opening)
cv2.waitKey(0)
cv2.destroyAllWindows()
