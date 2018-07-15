#ifndef DATAFUN_H
#define DATAFUN_H DATAFUN_H

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string.h>

#include "tiffio.h"


//importsemtif takes the string of a path to the image that should be imported, and pointers to the image matrix, with and higth variables of the image.
//importsemtif will automatically allocate memory for the double array that is the image matrix, but the pointers to with and hight have to be already allocated.
//this function is fine tuned to import tif images generated by the SEM software in the cellar of the INM, it might not work for any other images.
//the arguments  are :"/path/to/image.tif" ; double uint8 pointer in which the image will be written, pointer in which the with of the image will be written ; pointer in wich the hight of the image will be written.
//returns 0 if sucsessful, 1 if not.
bool importsemtif(const char*,uint8**&,uint32*, uint32*);

#endif
