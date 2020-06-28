import cv2

cam = cv2.VideoCapture(0)
while True:
    ass, frame = cam.read()
    cv2.imshow('window', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    
cam.release()
cv2.destroyAllWindows()
