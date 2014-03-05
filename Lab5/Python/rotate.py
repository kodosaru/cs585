import cv2
import numpy as np

img = cv2.imread('Data/crocus.jpg',0)
rows,cols = img.shape

M = np.float32([[0.707,-0.707,0.0],[0.707,0.707,0.0]])
dst = cv2.warpAffine(img,M,(cols,rows))

cv2.imshow('img',dst)
cv2.waitKey(0)
cv2.destroyAllWindows()
