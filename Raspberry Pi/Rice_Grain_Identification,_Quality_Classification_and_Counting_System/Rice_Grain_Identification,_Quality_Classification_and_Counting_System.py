import cv2
import numpy as np
import time



cap = cv2.VideoCapture(0)
fps = cap.get(cv2.CAP_PROP_FPS)



def get_classificaton(ratio):
            ratio =round(ratio,1)
            toret=""
            if(ratio>=3):
                    toret="Slender"
            elif(ratio>=2.1 and ratio<3):
                    toret="Medium"
            elif(ratio>=1.1 and ratio<2.1):
                    toret="Bold"
            elif(ratio<=1):
                    toret="Round"
            toret="("+toret+")"
            return toret

def process(contours):
            
            #image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
            #mage_gray = cv2.GaussianBlur(image_gray, (5, 5), 0)
            #cv2.imshow('image_res', image_gray)

            #convert into binary
            #ret,binary = cv2.threshold(image_gray,160,255,cv2.THRESH_BINARY)# 160 - threshold, 255 - value to assign, THRESH_BINARY_INV - Inverse binary
            #cv2.imshow('binary img', binary)
    
            #averaging filter
            #kernel = np.ones((5,5),np.float32)/9
            #dst = cv2.filter2D(image,-1,kernel)# -1 : depth of the destination image


            #kernel2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))

            #erosion
            #erosion = cv2.erode(dst,kernel2,iterations = 1)
            #cv2.imshow('erosion img', erosion)
    
            #dilation 
            #dilation = cv2.dilate(erosion,kernel2,iterations = 1)

            #edge detection
            #edges = cv2.Canny(dilation,100,200)
            #cv2.imshow('edged img', edges)
 
            ### Size detection
            #_,contours,hierarchy = cv2.findContours(erosion, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            #print("No. of " + color +" rice grains = ",len(contours))
            
         
            total_ar=0
            for cnt in contours:
                    x,y,w,h = cv2.boundingRect(cnt)
                    aspect_ratio = float(w)/h
                    if(aspect_ratio<1):
                            aspect_ratio=1/aspect_ratio
                    print(round(aspect_ratio,2),get_classificaton(aspect_ratio))
                    total_ar+=aspect_ratio
            #avg_ar=total_ar/len(contours)
            #print("Average Aspect Ratio=",round(avg_ar,2),get_classificaton(avg_ar))
            return len(contours)

while True:
    start = time.time()
    # load the image
    _,frame = cap.read()
    cv2.imshow('image', frame)
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    cv2.imshow('hsv', hsv)

    # copy of original image
    #image_to_process = hsv.copy()
    
    # white
    lower_limit_white = np.array([0, 2, 138])
    upper_limit_white = np.array([180, 70, 255])

    # find the colors within the specified boundaries    
    w_image_mask = cv2.inRange(hsv, lower_limit_white, upper_limit_white)
    #cv2.imshow('white image_mask ', image_mask )
    # apply the mask
    w_image_res = cv2.bitwise_and(frame, frame, mask= w_image_mask)
    cv2.imshow('white', w_image_res)
    kernel = np.ones((5,5),np.float32)/9
    w_dst = cv2.filter2D(w_image_mask,-1,kernel)# -1 : depth of the destination image


    kernel2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))

    #erosion
    w_erosion = cv2.erode(w_dst,kernel2,iterations = 1)
    _,w_contours,hierarchy = cv2.findContours(w_erosion, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    print("No. of white rice grains = ",len(w_contours)-1)
    w_cnt = process(w_contours)

    #red
    lower_limit_red = np.array([0,  80,  20])
    upper_limit_red = np.array([16, 250,  135])
    
    # find the colors within the specified boundaries    
    r_image_mask = cv2.inRange(hsv, lower_limit_red, upper_limit_red)
    #cv2.imshow('image_mask ', image_mask )
    # apply the mask
    r_image_res = cv2.bitwise_and(frame, frame, mask= r_image_mask)
    cv2.imshow('red', r_image_res)
    kernel = np.ones((5,5),np.float32)/9
    r_dst = cv2.filter2D(r_image_mask,-1,kernel)# -1 : depth of the destination image


    kernel2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))

    #erosion
    r_erosion = cv2.erode(r_dst,kernel2,iterations = 1)
    _,r_contours,hierarchy = cv2.findContours(r_erosion, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    print("No. of red rice grains = ",len(r_contours))
    r_cnt = process(r_contours)

    totalGrain = w_cnt + r_cnt -1
    print("Total Grain = " + str(totalGrain))
    
##    kernel = np.ones((5,5),np.float32)/9
##    dst = cv2.filter2D(image_mask2,-1,kernel)# -1 : depth of the destination image
##
##
##    kernel2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))
##
##    #erosion
##    erosion = cv2.erode(dst,kernel2,iterations = 1)
##    cv2.imshow('red erosion img', erosion)
##    _,contours,hierarchy = cv2.findContours(erosion, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
##    print("No. of red rice grains = ",len(contours))
##            
##         
##    total_ar=0
##    for cnt in contours:
##            x,y,w,h = cv2.boundingRect(cnt)
##            aspect_ratio = float(w)/h
##            if(aspect_ratio<1):
##                    aspect_ratio=1/aspect_ratio
##            print(round(aspect_ratio,2),get_classificaton(aspect_ratio))
##            total_ar+=aspect_ratio
    end = time.time()
    tTotal = end-start
    print("Processing Time = " + str(tTotal))
    

    key = cv2.waitKey(1)
    if key == 27:
        break



print("Frame Per Second = " + str(fps))   
 #plt.imshow(frame)
cap.release()
cv2.destroyAllWindows()
