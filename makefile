

all:
	g++ Principal.cpp -I/OpenInstall/opencv/opencvi/include/opencv4/ -L/OpenInstall/opencv/opencvi/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio  -lopencv_objdetect -o pedro.bin -std=c++17 -lstdc++fs
	
run:
	./pedro.bin