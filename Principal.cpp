
#include <iostream>
#include <cstdlib>

#include <cmath> // Esta librería contiene las funciones para realizar operaciones matemáticas (sin, cos, tan, log, exp, etc.)

// Cuando se carga la cabecer opencv.hpp automáticamente se cargan las demás cabeceras
//#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp> // Contiene los elementos básicos como el objeto Mat (matriz que representa la imagen)
#include <opencv2/highgui/highgui.hpp> // Contiene los elementos para crear una interfaz gráfica básica
// OpenCV no está pensado para crear interfaces gráficas potentes. Se centra en la visión artificial y PDI. Si se desea crear una interfaz gráfica completa, se debe usar QT

#include <opencv2/imgcodecs/imgcodecs.hpp> // Contiene las funcionalidad para acceder a los códecs que permiten leer diferentes formatos de imagen (JPEG, JPEG-2000, PNG, TIFF, GIF, etc.)

// Librerías para acceder al video y para poder escribir vídeos en disco
#include <opencv2/video/video.hpp> 
#include <opencv2/videoio/videoio.hpp>

#include <opencv2/imgproc/imgproc.hpp> // Librería para realizar operaciones de PDI 

#include <opencv2/objdetect/objdetect.hpp> // Librería contiene funciones para detección de objetos

#include <ctime>

using namespace std;
using namespace cv;

//Mat framePeatones = imread("framePeatones.jpeg");
        
Mat decimal; // Imagen en formato decimal para representar los gradientes
Mat gx, gy; // Gradientes en "x" y en "y"
Mat angulo, magnitud; // Ángulo y magnitud del gradiente
Mat framePeatones;    
int pix1=8;
int pix2=128;
int num=0;
int numframes=0;
int numPeatones=0;
        


void detectPeatones(string ruta){
    VideoCapture video(ruta);
    if (video.isOpened()) {
        while (true) {
            video >> framePeatones;
            
            //namedWindow("Imagen Original", WINDOW_AUTOSIZE);
            //namedWindow("Gx", WINDOW_AUTOSIZE);
            //namedWindow("Gy", WINDOW_AUTOSIZE);
            //namedWindow("Magnitud", WINDOW_AUTOSIZE);
            namedWindow("Detecciones", WINDOW_AUTOSIZE); 
            
            createTrackbar("pixel1","Detecciones", &pix1, 255, nullptr, nullptr);
            createTrackbar("pixel2","Detecciones", &pix2, 255, nullptr, nullptr);
            createTrackbar("Save detects","Detecciones", &num, 1, nullptr, nullptr);

            if (framePeatones.rows == 0 || framePeatones.cols == 0)
                break;
            //crearTrackbars("video");            
            //imshow("video", frame);            
            resize(framePeatones, framePeatones, Size(), 0.5,0.5);    
            cvtColor(framePeatones, framePeatones, COLOR_BGR2GRAY);
            framePeatones.convertTo(decimal, CV_32F, 1.0/255.0); // Creamos una imagen decimal con escala entre 0 y 1 (255 = 1.0)
            // Calculamos los gradientes a través del operador de Sobel
            Sobel(decimal, gx, CV_32F, 1, 0, 1); // Gradiente en "x"
            Sobel(decimal, gy, CV_32F, 0, 1, 1); // Gradiente en "y"
            
            // Calculamos la magnitud y el ángulo del gradiente
            cartToPolar(gx, gy, magnitud, angulo, true); // Pasamos a coordenas polares
            
            // Cálculo del HOG
            //HOGDescriptor hog (Size(64,128),Size(16,16),Size(8,8),Size(8,8),9,1);
            HOGDescriptor hog (Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9);
            //hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
            hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
            vector<Rect> detecciones;
            
            //hog.detectMultiScale(framePeatones, detecciones, 0, Size(pix1,pix1), Size(pix2,pix2), 1.05, 2, false);
            hog.detectMultiScale(framePeatones, detecciones, 0, Size(pix1,pix1), Size(pix2,pix2), 1.05, 2, true);
            Mat clon = framePeatones.clone();            
            cout << "Numero Peatones: " << detecciones.size() << endl;
            Rect r;
            for (int i=0;i<detecciones.size();i++){
                r = detecciones[i];
                r.x += cvRound(r.width*0.1);
                r.width= cvRound(r.width*0.8);
                r.y += cvRound(r.height*0.07);
                r.height = cvRound(r.height*0.8);
                
                rectangle(clon,r.tl(),r.br(), Scalar(10,10,233),2);       
                if(num==1){
                    Mat extract;
                    clon(Rect(r.x,r.y,r.width,r.height)).copyTo(extract);
                    string name="img"+to_string(i)+".png";
                    imwrite(name,extract);
                }                
                numPeatones+=1;
                   
            }       
            numframes+=1;            
            //imshow("Gx", gx);
            //imshow("Gy", gy);
            //imshow("Magnitud", magnitud);
            imshow("Detecciones", clon);
            

              

            if (waitKey(23) == 27)//Al pulsar la tecla de escape
                break;            
        }
        cout<<"Prom de peatones por frame: "<< numPeatones/numframes << endl;

        destroyAllWindows();
    }

}

int main(int argc, char *argv[]){
    
    detectPeatones("/home/trigun/Descargas/peatones1.mp4");        
    return 0;
}


