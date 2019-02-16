import math as m

def getAngle(x1,y1,x2,y2):
    dot = x1*x2 + y1*y2
    det = x1*y2 - y1*x2
    angle_r = m.atan2(det, dot)
    a = angle_r * 180 / m.pi;
    return a


x = getAngle(1,0,0,1)
print(x)
