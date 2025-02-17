# Automated tracing algorithm for fibers of uniform thickness
This programm was specifically developed to trace the actin fibers visible in SEM images of mammalian cells after triton extraction. It can likely also be applied to any other fiber system where the fibers have a sufficiently uniform thickness. The programm works with grayscale images and any color images used as input will be converted to greyscale internally for the tracing.

# Installation
It is recommendet to use one of the manjaro linux distributions (https://manjaro.org/) which is based on Arch, so other linux distributions based on Arch should also work.
The software requires the OpenCV library (https://opencv.org/) which can be conviniently installed on Arch based distributions by typing the following command into the command line and pressing enter:
```
sudo pacman -Syu opencv
```
Next download the Source code of the software by for example executing the following command:
```
git clone https://github.com/SRaent/FiNTA.git
```
Note that if git is not installed, this will promt an error such as "git: command not found". You can install git using "sudo pacman -Syu git".
Next the code needs to be compiled against the OpenCV library. On Manjaro if the "opencv" package is installed as described above simply execute the "compile.sh" script using the following commands:
```
cd ./Actin
./compile.sh
```
This will generate the executable. This executalbe has to be called as described in the next section.

# Usage
To apply the tracing algorithm to a given image, the path to the image, and the path to a configuration file have to be handed over to the programm. To archieve this, run the programm in the command line with the paths as arguments. The path to the image has to be passed as a argument after the "-f" flag and the path to the settings file after the "-s" flag. To successfully execute the programm, type something like
```
./finta -s /path/to/settings_file.set -f /path/to/image.tif
```
into the linux terminal.
Note that for this to work, the finta executabel generated in the previous section has to be present in the current working directory of the terminal. You can check the current working directory by using the "pwd" command. Alternatively you can check the files in the current working directory for the finta executable by using the "ls" command.
A typical usage case would be, that you add the image, the finta executable and the settings file to a directory. Next you open a terminal in that directory (right click on the file explorer in may cases has this option) and you type
```
./finta -f ./imagename.png -s ./settingsfilename.set
```
where you exchange the "imagename.png" by the actual imagename you want to trace and the "settingsfilename.set" by the name of the settingsfile. The supplied settingsfile is called "example.set".

# The settings file
This file defines the tracing parameters as well as the output of the programm. Comments in this file are defined as everything behind a "#".
## Tracing parameters
Adjustments to the tracing parameters have a big impact on the tracing quality. Therefore they have to be chosen with great care. Guidelines on how to choose them are provided in the choosing\_parameters.pdf that is provided with the programm. The parameters that can be chosen are: "sigma\_smooth", "sigma\_conf", "r\_min", "r\_max", "steps", "thresh", "r\_step", and "min\_loop\_length". From these only "sigma\_smooth", "sigma\_conf","r\_max" and "r\_step" have to be chosen for the programm to run, but it is recommended to manually choose all tracing parameters. The default values for the parametes that do not have to be chosen are: r\_min = 0, steps = 360, thresh = 0 and min\_loop\_length = 6. Parameters are set by writing their name followed by a whitespace and then the value they are assigned. These parameters can only be assigned once.
An example for setting the "sigma\_smooth" parameter to 0.5 would be:
```
sigma_smooth 0.5
```

## Using default parameter sets
We found a parameterset which worked on almost all of our traced images. This set can be used with the keyword "default\_param\_set" by the fiber thickness.

## Cropping the image
Since many imaging programs add a scalebar or other information at various places to the aquired image, it is possible to only analyze a specified part of the opened image. If the whole image was to be analyzed, any scalebars or text added to it would be considered to be features of the image and traced accordingly, skewing the outcome. In order to crop the image before tracing, the keyword "crop" has to be used, followed by 4 arguments seperated by whitespaces. The four arguments contain two edges of the cropping rectangle in the order, x-coordienate of the first corner of the corpping rectangle, y coordinate of this corner, x coordinate of the other corner and y coordinate of that corner. All operations will be performed on the cropped image. A example for how to cropp the image would be:
```
crop 0 0 1024 884
```
Cropping can also be used to quickly trace a smaller section of the image in order to more quickly optimize the tracing parameters.

## Inverting the image
Use the keyword "invert" without arguments to invert the image to be traced. This will affect all backgrounds of the draw function exept for "original". This is useful, because FiNTA is designed to trace bright fibers on a dark background. If your imaging technique produces dark fibers on a bright background it has to be inverted.

## Tracing only one color channel
In many flourecence microscopy images, the different colorchannels of an image correspond to the signals of different wavelengths. Often it is therefor of interest to only consider one specific color channel of the image for tracing. This can be achieved using the "use\_channel" keyword followed by a number specifying the channel. The channels start at 1 and due to the color handling of OpenCV, channel 1 usually corresponds to blue, channel 2 corresponds to green and channel 3 corresponds to red. This will affect all backgrounds of the draw function exept for "original".

## Image rescaling
By default, the image is rescaled before the tracing, such that the darkest pixel has a brightness of 0 and the brightest one a brightness of 255. This rescaling can be turned off using the "no\_rescaling" command.

## Tracing start points
In order for the algorithm to function, at least one starting point has to be set on the image. Starting points can be set manually by the keyword "startpoint" followed by the x and y koordinate of the startpoint. A arbitrary number of startpoints can be defined this way by simply using the "startpoint" keyword in several lines each followed by the corresponding koordinates.
A function to automatically place startpoints is also provided. It is activated by the keyword "auto\_startpoint" and can take between 0 and 3 arguments. The first argument is the maximal number of start points to be created and its default value is 100. The second argument is the minimal distance between the start points. Here already manually created start points are NOT taken into account. Its default vaulue is 20 * r\_step. The third argument is the minimum of the negative smaller eigenvalue of the hessian at the startpoint position. It can not be chosen below 0 and its default value is 0. For more information on how this function operates see the choosing\_parameters.pdf that is provided with the programm. A example for defining a startpoint manually would be:
```
startpoint 100 100
```
and for defining the settings for automated startpoint generation:
```
auto_startpoint 100 60 10
```
where a maximal number of 100 startpoints will be generated at least 60 pixel apart and with a minimum negative second order derivative of 10.

## The <imagename\> keyword
The "<imagename\>" keyword can be used in any path that is specified in the settings file. It will be replaced by the name of the image that is beeing analyzed in the respective run of the programm.

## Using the Modified hessian
The "use\_modified\_hessian" keyword causes the program to use a modified hessian matrix which can improve the tracing result. The parameters might have to be adjusted slightly when compared to using the unmodified hessain on the same image.

## Handling small loops
During the tracing process, everytime 2 already existing nodes want to connect with each other, it is checked weather this creates a loop that is smaller than "min\_loop\_length". By default if this is the case, the connection is simply not made. This behaviour can lead to suboptimal connections branching. Therefore it is possible to choose a diffenert behaviour using the keyword "delete\_worst\_sections". Here the connection is made and whe following procedure is repeated while a loop with less than "min\_loop\_length" nodes persists. The smallest loop with the just formed connection is identified and cut into sections. These sections always end and begin at nodes with more than 2 junctions. If only one section is found the function falls back to the default behaviour and deletes the newly made connection. If more than 1 section is found all the sections are rated and the section with the worst rating is deleted. The rating is normalized to the length of the sections and evaluates how well they travel along the fibers according to the hessian matrix. The width of the pixels around the fibers taken into account for this can be adjusted by adding a number behind the keyword "delete\_worst\_sections" that describes this width in pixels. This value should be chosen aroud the fiber thickness and the default value is twice "sigma\_conv".


## Deleting nodes according to image brightness
The commands "delete\_nodes\_above\_threshold", "delete\_nodes\_below\_threshold", "delete\_lines\_above\_threshold" and "delete\_lines\_below\_threshold" can be used to delete all nodes below or above a brightness threshold. The commands "delete\_lines\_above\_threshold" and "delete\_lines\_below\_threshold" cause all nodes along the fiber of the node below the threshold to be deleted until a junction is met. The threshold is specified as a positive integer number behind the command. Here the brighness is measured on the grayscale converted original image WITHOUT the brightnes rescaling from 0 to 255 that is performed to aid the tracing. Therefore the brightness values obtained from other image analysis software can be used directly. These four commands are not mutually exclusive.

## Drawing commands
Drawing commands are initiated by the "draw" keyword. Next a number of properties of the image to be drawn can be specified in any order behind the "draw" keyword seperated by whitespaces. These properties have to be specified in the same line as the "draw" keyword.
A arbitrary number of drawing commands can be specified in the settings file, that will all be interpreted and result in a distinct image.
The path, the image will be saved to has to be specified directly behind the "name" keyword only seperated by a whitespace. Only certain file endings are allowed here. Supporded endings are: .bmp, .bmp, jpeg, .jpg, .jpe, .jp2, .png, .pbm, .pgm, .ppm, .sr, .ras, .tiff and .tif. If no ending is specified, the eding of the input image is used.
The background to be drawn on can be specified by exatly one of the following keywords per drawing command: "original", "cropped", "tubeness", "hessian", "visualized\_hessian", "white" and "black". All of these backgrounds will have the size of the cropped image exept the "original" backgroud. The backgrounds "original", "cropped", "white" and "black" are assumed to be self explanatory. The background "tubeness" maps each pixel of the cropped image to the smaller eigenvalue of the hessian with flipped sign. Values obtained this way are then rescaled to be between 0 and 255. The background "hessian" uses the three color channels blue, green and red to encode the values of the computed hessian a, b and c respectively. These values are also rescaled to be between 0 and 255 while all three channels are scaled uniformely. for more information on what the hessian matrix is see the choosing\_parameters.pdf that is provided with the programm. The background "visualized\_hessian" also containes all information about the hessian matrix in each pixel, but packaged in a more visible way. This background is generated in the HSV colorspace. The Hue (H) represents the direction of the smaller eigenvector of the hessian. The saturation (S) represents the relationship between the absolute values of the smaller and bigger eigenvalue of the hessian and the value (V) or brightness represents the size of the smaller eigenvalue of the hessian with flipped sign.
If only the background is specified, nothing will be drawn on it and it will be saved to the specified path directly. It is possible to draw the traced network in two different stages on the network before saving the image. The keywords for these two networks are "network" for the entire network and "only\_loops" for only the nodes that are part of closed loops. Also both of these networks can be drawn on the same background. In this case the entire network will always be drawn first and then be partially covered by the "only\_loops" protion of it. After each of the two keywords "network" and "only\_loops", 0 to 4 numerical values can be specified that determine the thickness of the lines and their color. If 0 numbers are provided, the default values are used. For "network" the default color is red (255 0 0) and the default thickness is 1 pixel. For "only\_loops" the default color is blue (0 0 255) and the default thickness is also 1 pixel. If one numerical value is provided, it is interpreted as the grayscale brightness to be drawn and the line thickness remaines 1 pixel. If two numerical values are provided after any of the two keywords "network" and "only\_loops" the first one is interpreted as the grayscale value of the color and the second one as the line thickness in pixels. If three numerical values are provided, they are interpreted as a color, where the first number is used for the red channel, the second one for the green channel and the thrid one for the blue channel. All values relating to the color or grayscale value have to be between 0 and 255. If 4 numerical values are provided, the first three are again interpreted as a color and the fourth one is used as the line thickness in pixel. It is also possible to encode the fiber direction in the color. For this the "only\_loops" or "network" keyword has to be followed by the "angle\_hue" keyword. Here the HSV colorspace is used and the saturation and value are both set to 1. The Hue is determined by the fiber angle.
A example for a drawing command would be:
```
draw original network 255 0 0 2 name ./results/test.png only_loops 0 0 255
```
Where the whole network will be drawn on the whole original image in red with a line thickness of 2 pixels and the closed loops will be drawn on top of the already drawn network in blue with a line thickness of 1 pixel. The image will be saved in the subfolder "results" with the name "test.png".
A different use if the drawing command could be:
```
draw visualized_hessian name ./results/vis_hes.tif
```
Where the visualized hesian of the cropped image will be saved without anything drawn on it.

## Setting the image scale
Since pixel can be a cumbersome unit to use, it is possible to define a arbitrary unit into which all output data will be converted. Also certain parameters can be defined using this unit. The keyword used to define such a unit is "set\_scale". It has to be followed by a number of pixels and a number of units the amount of pixels corresponds to. These two values have to be seperated by a "=" and the order in which they are stated is irrellevant. The number of pixels has to be followed by "px" or "pixel" after a whitespace. The number of units has to be followed by the word that defines the unit also after a whitespace. Examples to use "set\_scale" would be:
```
set_scale 300 px = 1000 nm
```
or
```
set_scale 1 um = 300 pixel
```
where "um" or "nm" are from now on a keyword for the conversion of the now defined unit to pixels. The "set\_scale" keyword can only be used once and only one unit can be defined this way. 
All positions or lengths of the output data will be converted to this unit and all areas will be converted to the unit squared. This will not be noted or indicated in the files where the data is saved.
Now it is also possible to define certain numerical arguments for other keywords using this unit. For example:
```
draw original network 255 0 0 8 nm name ./results/test.png
```
where the "8 nm" will be converted to 2.4 pixel automatically. This conversion does not work on a few specific parameters, that are scale invariant. Specifically: "sigma\_smooth", "steps", "thresh" and "min\_loop\_length", because defining them in terms of length does not make any sense. Note that the defined unit can only be used AFTER "set\_scale" has been called.

## Saving extracted data
Currently the programm only supports a small number of parameters that are extracted form the traced network. The files saved this way only contain numbers and no text. If the "set\_scale" command is used in the settings file, all numerical values contained in these files will have the appropriate unit derived from the one in the "set\_scale" command. Specifically it will be the unit itself or the unit squared. There will be no remark or other indication in the saved files that this scaling has taken place.

### Saving loop areas
In order to save the loop areas, the keyword "save\_loop\_areas" is used. The path where the loop areas will be saved can be specified after a whitespace. In addition a fiber thickness can be specified by a numerical value. In that case from each loop area, the circumference of the loop times half the specified fiber thickness will be subtracted from each loop area. This takes the fiber thicknes into account, but since the loop is not a streight line, the line segments that now have a thickness associated with them will slightly overlap at inward kinks and leave empty space at outward kinks. In order to compensate for this inaccuracy as best as possible, pi times the fiber thickness squared devided by 4 is added to each of the areas obtained this way. If the resulting area is smaller than 0, it is discarded.
A arbitrary amount of files containing the loop areas for different considered fiber diameters can be saved to different paths. The default value for fiber thickness is 0, so only the area of the polygon describing the loop is considered. A example for how to save loop areas would be:
```
save_loop_areas  ./results/loop_areas_thickness.dat 3
save_loop_areas  ./results/loop_areas.dat
```
Where two seperate files would be saved. One with a assumed fiber thickness of 3 pixels and one with a assumed fiber thickness of 0 pixels. The order in which the path and the fiber thickness is stated is irrelevant. If the "set\_scale" command is called in the settings file, the output will have the dimension of the defined unit squared without any remarks in the saved file.

### Save loop circumferences
In order to save the loop circumferences of all closed loops, the keyword "save\_loop\_circumferences" is used. It can only be called once and the path, to which the data will be saved can be specified after a white space. If the "set\_scale" keyword is used in the settings file, the output will be in the unit defined there and scaled accordingly without any indication in the created file. If "save\_loop\_areas" is also used with the default considered fiber thickness of 0, the two files will have the same number of values in them and the values will correspond to one another. This means that the 5th entry in the loop area file will correspond to the same loop as the 5th entry in the loop circumference file and so on.

### Save loop circleness
The loop circleness is defined as 4 times Pi times the loop area devided by the loop circumference squared. It can be saved to a file by using the "save\_loop\_circleness" keyword optionally followed by the path the data should be saved to. Since the output is dimensionless no rescaling will take place. This command can only be called once.

### Save loop diameters
The loop diameter is defined as the longest possible distance between two nodes that are part of the loop. It can be saved using the "save\_loop\_diameters" keyword optionally followed by the path the data should be saved to.

### Save junction distances
Here two commands can be used: "save\_all\_junction\_distances" and "save\_loop\_junction\_distances". They both perform the same tasks but on different parts of the traced network. They compute all distances between junctions in the network along the network. Junctions are defined as nodes that have more than two neighbours. They do not compute the distance along a streight path between the junctions but how far one has to walk along the network to get from one junction to the next. The total length of the traced network can be computed by adding up all values in the files generated by these functions. It should be noted that since most connections between nodes are "r\_step" long, most values in these files are multiples of this value. The difference between the two commands is that "save\_all\_junction\_distances" takes all nodes into account and "save\_loop\_junction\_distances" only takes nodes into account that are part of closed loops.

### Save junction connectivity
The commands "save\_all\_junction\_connectivities" and "save\_loop\_junction\_connectivities" can be used to save the number of connections of each junction in either the complete network or only considering the nodes that are part of closed loops. Both commands can optionally be followed by a path where the data should be saved.

### Unifying junctions
These commands can also be followed by a positive integer number, which is then interpreted as the unification distance of the junctions. This way all junctions that have fewer steps inbetween them than this number are unified to one junction. It is possible for two junctions to be part of one unified junction even if they are further apart than the specified number of steps. This happens if there is a third junction that is within the unification distance of both of them. In this case at least all 3 of these junctions are then unified to one junction in the analysis of the junction connectivity. The number of steps between the junction is counted along the traced network connection and therefor influenced by the "r\_step" parameter. It is recoomended to not choose the unification distance greater than 2 or 3 and it is strongly recommendet to not choose it greater than the "min\_loop\_length" parameter. It is possible to specify several unification distances in one settings file by simply using the appropriate command (such as "save\_all\_junction\_connectivities") several times, once for each unification parameter. In this case it is important to specify different paths for each command to prevent overwriting since each command will generate its own output file. Also lines that end with a node with only one connection (lines that are not part of closed loops) which are shorter than the unification distance are not considered in the junction unification. This also exteds to arbitrarily complex node trees without any closed loops in them. Only if there exists a streight line in the tree longer than the unification distance is it considered part of the newtork for the unified junctions. This consideration is only relevant for commands that analyse the whole network and not only the closed loops.

### Save line lengths
Atafut also has the ability to extract fibers from the nework and output their length. The fibers are defined as all nodes that have a sufficiently small angle between them. The commands "save\_loop\_line\_lengths" and "save\_all\_line\_lengths" can be used, nessesarily followed by the cutoff angle inbetween the nodes IN RADIAN, not in degrees. The cutoff angle is measured from the straight continuation of the last considered connection. So the total angle in which the next connection may lie to continue the fiber is twice the specified value. If multiple connections are in the acceptable angle range the straightest one is chosen. Lines can not contain less than 3 nodes. The command can also optionally be followed by a path to which the data should be saved. Aditionally it is also possible to draw the found lines in several images. To archive this add the keyword "draw\_lines" behind either of the commands. This keyword can be followed by 2 optional arguments. The number of images the lines shall be distributed over and the path to where they sould be saved. The lines will be distributed over several images to allow their visual evaluation. The lines will also be orderd by there length and one image will contain all lines in one length intervall. The length intervalls are chosen such that the total length of lines drawn on one image are as constanst as possible. It might happen that not the exact specified number of images are generated by this command. Also the line lengths in one image are color coded by HUE.
For every command that saves line lengths, a entry in the auxiliary data file is generated which states the persistence length of the lines that were found. To compute this value outside of FiNTA, the angle-distance correlation data can be saved. For this add the keyword "persistence\_data", optionally followed by the path the data file should be written to, to the line of the line lenghs command.
Example:
```
save_all_line_lengths 0.35 <imagename>_line_lengths.dat draw_lines 10 ./line_images/img persistence_data <imagename>_persistence_data.dat
```
This will use 0.35 radians as break off angle, save the line lengths to the first file name in the string, draw all recognised lines distributed over 10 images in the folder "line\_images" with the names img1.png to img10.png and it will save the data neccessary to compute the persistence length in the last filename.
The data generated by "persistence\_data" has 2 columns seperated by a whitespace. To compute the persistence lengt, one plots the distances between line segments on the x-axis and the normalised scalar product of the line segment directions on the y-axis. The x-axis data is in the first column and the y-axis data is in the second column.

### Save auxiliary data
The following data is considered auxiliary: total number of nodes, total number of junctions, total network lenght, total number of nodes contributing to closed loops, total number of junctions in the network of closed loops, total number of closed loops, total area of all closed loops, total network length. This data can be saved to a file using the command "save\_auxiliary\_data" optionally followed by the path where the file should be saved. All data in this file is scaled acordingly.

## Optimising the computation time
One part of the algorithm that is the most computationally expensive can be multithreaded by using the "threads" command followed by the number of threads that will be used after a whitespace. For best performance use the number of physical threads of your processor. Also the parameter "steps" is very important for the computation time. In order to decrease the computation time, decrease the "steps" parameter. At some point this might cause a worse tracing result, but a value of 100 for "steps" is still very save and will drastically decrease the computation time compared to the default value of 100.

## Saving the raw tracing data
If one wishes to evaluate the generated tracing data on their own, it is recommended to modifie the c++ source code, since there the raw tracing data is accessible in a convinient datastructure. Nevertheless a keyword to save a representation of the raw tracing date is available. It is "save\_tracing\_data" and can only be called once, followed by the path to the file, where the data should be saved. The datastructure is the following: The first part of the file consists of three columns, that contain the number, x and y position of each node in the traced network in that order. If "set\_scale" was used in the settings file, the x and y coordinates position will be scaled accordingly. After the three column part of the file, it transitions to a two column part, in which all the connections between the nodes defined in the first part are listet. For a connection between nodes A and B, the index of node A is given in the first column and the index of the second node B is given in the second column. Here for every connection between two nodes A and B, there exists a second connection between B and A, with the same inices but with the order of them flipped. Therefore each connection is sort of listed twice.

## Saving the loop poligons
If one wishes to do more extensive analysis on the closed loops in the traced network, it is possible to save those to a file with the keyword "save\_loop\_data", which can only be used once. Behind this keyword after a whitespace, the path, where the loop data should be saved can be entered. This data is represented in the file in the following way: The file containes three columns seperated by whitespaces. The first column containes the index of the loop. Each time the first column increases by 1, a new loop is beeing described. The second and third column contain the x and y koordinates of the nodes the loop consists of. If the "set\_scale" keyword is used in the settings file, the output will be in the unit defined there and scaled accordingly without any indication in the created file.

## Saving the absolute angle distribution.
Use the keywords "save\_all\_absolute\_angles" or "save\_loop\_absolute\_angles" to save the angles of node connections in the complete network or the network consisting only of closed loops respectively. The keywords can be followed by the path the data should be saved to. The values will be in degrees and lie between 0° and 180° since the connections dont have a direction and therefore rotating them by 180° gives the same angle. The angle is measured counterclockwise to the vertical axis.

## Generating a animation of the tracing
To generate a animation of the tracing, the keyword "animate\_tracing" can be used. It takes arguments very similar to the drawing command. To specify where the ".avi" file should be saved, write the path, without a file ending, behind the "name" keyword in the same line. Additionally, the color and line thickness to be drawn can be specified by between 0 (for default) and 4 numbers, where the first 3 specify the color in RGB and the last one the linethickness in pixel.


# Using finta in scripts
It is also possible for scripting use to pass all the lines of a settings file in the command line. To archieve this, simply specify the image as usual behind the "-f" command line argument and then after that pass the command line argument "-settings" followed by all settings lines you wish to pass. The lines have to be sepperated by a semicolon ";".

# Worsening the image for testing purposes
## Adding noise
To evaluate the software it is possible to add noise to the imported image. This will always decrease the tracing quality, but by using a image with a known fiber structure, the effects of noise on the tracing quality can be evaluated this way. The keyword is "add\_noise" followed by the noise level. The noise level is the standard deviation of the gaussian noise which is added to each pixel before the image is rescaled. The noise will be visible in images using the "cropped" background and the effects of the noise will be visible on all image backgrounds related to the tracing, but not when useing the "original" background.

## Pointilising the image
In order to simulate STORM images, the keyword "pointilise" can be used, followed by a number between 0 and 1 representing a probability. Now every pixel can be turned compleatly black with the specified probability. Generally STORM images only posess compleatly white or black pixels. To simulate this, the input image must already be compleatly black and white.
