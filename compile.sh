#!/bin/sh
g++ main.cpp -ltiff -lfftw3 -I /usr/include/opencv -L /usr/lib -lopencv_core -lopencv_imgproc	-lopencv_highgui -lopencv_imgcodecs -O0 -g
#-O0 -g
