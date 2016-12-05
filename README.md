# manga-text-detection
### require: OpenCV 3.1.x (latest)

### install on Linux by following steps:

	[compiler] sudo apt-get install build-essential
	[required] sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
	[optional] sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

	git clone https://github.com/opencv/opencv.git

	cd ~/opencv
	mkdir release
	cd release
	cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..

	make
	sudo make install

### get this project via git:

	git clone https://github.com/EyiLee/manga-text-detection.git

### build by following steps:

	mkdir build
	cd build
	cmake ..
	make

### description:
main.cpp
>	detect text regions from input image

img2label.cpp
>	transform images into labeled HOG vectors data

svmtrain.cpp
>	transform labeled HOG vectors data into SVM model

ccx.xml
>	SVM model of cc

ccrx.xml
>	SVM model of ccr

images/
>	original cc and ccr images

train/
>	labeled HOG vectors data

### usage:
main
>	./main <images ...>

>images: original manga pages

img2label
>	./img2label -s <output> -l <label> <images ...>

>e.g.	./img2label -s ../train/connectedcomponents/word/Arisa.xml -l 1 ../images/connectedcomponent/Arisa/word/*.jpg

>output: the file you want to save the labeled HOG vectors data

>label: 1 or -1 to assign the images are words or not

>images: the cc or ccr images you want to label

svmtrain
>	./svmtrain -s <output> <files ...>

>e.g.	./svmtrain -s ccx.xml ../train/connectedcomponents/*/*.xml

>output: the file you want to save the SVM model

>files: the labeled HOG vectors data you want to train
