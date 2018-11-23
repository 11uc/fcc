## Introduction

This is a program used to control fear conditioning experiments. 
Intended to be used on Raspberry pi. Firstly implemented on MacOS. 
Qt and Opencv are used.

Basic functions include:
    * Video recording animal behavior through camera.
    * Control auditory cue 
    * Control electric shock using ttl signal
    * Customize experiment protocol

## Compilation

### MacOS

Compile using qmake and gnu make.
    1. Create project file `qmake -project -o fcrig.pro`.
    2. Add to fcrig.pro file the following lines to include Qt libaries
    ```
    Qt += widgets
    Qt += core
    CONFIG += qt debug
    ```
    3. Create MakeFile using `qmake -makefile fcrig.pro`.
    4. In MakeFile, add opencv address to INPATH and add
    ```
    -L/usr/local/Cellar/opencv/3.4.2/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_imgproc
    ```
    to LIBS.
    5. Build using `make`.
