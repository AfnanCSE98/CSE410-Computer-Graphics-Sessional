import cv2
import numpy as np
import glob
import shutil
# for filename in glob.glob('*.bmp'):
#     for i in range(5):
#         shutil.copy2(filename, filename + "copy_" + str(i) + '.bmp')

img_array = []
for filename in glob.glob('*.bmp'):
    img = cv2.imread(filename)
    height, width, layers = img.shape
    size = (width,height)
    img_array.append(img)


out = cv2.VideoWriter('ray_tracing.avi',cv2.VideoWriter_fourcc(*'DIVX'), 15, size)
 
for i in range(len(img_array)):
    out.write(img_array[i])
out.release()