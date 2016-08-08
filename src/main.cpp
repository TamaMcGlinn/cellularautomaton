// Copyleft: You are free to use this any way you like, as long as the file remains attributed to me, Tama McGlinn. Written July 20st 2013

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::to_string;
using std::flush;

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

#include "CellularAutomaton.hpp"

string window_name_begin = "Cellular Automaton: ";
int cellSize; unsigned long config;

//input
void interactive_threshold(Mat & data);
void interactive_getSeed(CellularAutomaton & demo);
void getBitString(unsigned long & config, const char * userInput);

//output
void displayImage(Mat & data, const char * cam_cap_title);
void saveImage(Mat & frame);
void printControls();

/** @function main */
int main ( int argc, char** argv )
{
	if(argc != 2){
		cout << "Defaulting to Life. To do a custom Cellular Automaton, run: " << endl
				 << argv[0] << " [18-bit binary string]" << endl
				 << "The first 9 bits specify when a live bit survives, the second 9 bits specify "
				 << "when a dead bit is born. From left to right, the first of these nine means there "
				 << "are 8 live neighbours, while the last means there are none. For example, "
				 << "000001100000001000 is Life." << endl << endl;
		config = 6152;
	} else {
		getBitString(config, argv[1]);
		int length = strlen(argv[1]);
		if( length != 18 ){
			cout << "That's " << length << " bits. You must specify an 18 bit string." << endl;
			exit(0);
		}
	}
	string window_name = window_name_begin + to_string(config);
	CellularAutomaton demo(config);
	//demo.setRand(100);//setup 10% live cells
	
	interactive_getSeed(demo);
	
	printControls();
	
	bool paused = true;
	bool displayTicks = true;
	int delay = 250;
	Mat img(demo.getMatrix().rows*cellSize, demo.getMatrix().cols*cellSize, CV_8U); //display is resized version of data
	while( true ){
		int keyPressed = waitKey(30 + delay);
		bool step = false;

		if( keyPressed != -1 ){
			if( keyPressed == 27 ){ //esc
				break;
			} else if(keyPressed == 104){ // 'h'
				printControls();
			} else if(keyPressed == 32){ //spacebar
				paused = !paused;
			} else if(keyPressed == 97){ // 'a'
				cout << "Enter new bitstring: ";
				string newCAstring;
				cin >> newCAstring;
				getBitString(config, newCAstring.c_str());
				demo.setAutomaton(config);
				cvDestroyWindow(window_name.c_str());
				window_name = window_name_begin + to_string(config);
				paused = true;
			} else if(keyPressed == 102){ // 'f'
				cout << "Ticks to run simulation: ";
				int ticks;
				cin >> ticks;
				for(int i = 0; i < ticks; i++){
					demo.timestep();
					if(displayTicks){
						cout << '>' << flush;
					}
				}
			} else if(keyPressed == 119){ // 'w'
				while(demo.timestep_checkChange());
			} else if(keyPressed == 118){ // 'v'
				displayTicks = !displayTicks;
			} else if(keyPressed == 115){ // 's'
				saveImage(img);
			} else if(keyPressed == 44){ // '<'
				delay+=50;
				cout << "Delay: " << delay << " milliseconds" << endl;
			} else if(keyPressed == 46){ // '>'
				delay = std::max(delay-50, 0);
				cout << "Delay: " << delay << " milliseconds" << endl;
			} else if(keyPressed == 65596){ // shift + '<'
				delay+=10;
				cout << "Delay: " << delay << " milliseconds" << endl;
			} else if(keyPressed == 65598){ // shift + '>'
				delay = std::max(delay-10, 0);
				cout << "Delay: " << delay << " milliseconds" << endl;
			} else if(keyPressed == 65363){ // right arrow
				step = true;
			} else {
				cout << "No key binding for: " << keyPressed << endl;
			}
		}
		
		if(!paused || step){
			demo.timestep();
			if(displayTicks){
				cout << '>' << flush;
			}
			step = false;
		}
		
		//Size(0,0) indicates to handle input size automatically
		resize( demo.getMatrix(), img, Size(0,0), static_cast<double>(cellSize), static_cast<double>(cellSize), INTER_NEAREST );
		imshow( window_name.c_str(), img );
	}
	cout << endl;
	return 0;
}

//input functions:

//turn it into black and white only, based on cin'ed threshold value
void interactive_threshold(Mat & img){
	int threshold_value;
	cout << "Enter the threshold value to use (0-255): ";
	cin >> threshold_value;
	threshold(img, img, threshold_value, 255, THRESH_BINARY);
}

void interactive_getSeed(CellularAutomaton & demo){
	Mat seed;
	cout << "Type a letter to choose how to seed the matrix: " << endl
			<< "c: Use a camera capture." << endl
			<< "i: Seed from image." << endl
			<< "r: Random seed." << endl;
			
	char choice;
	cin >> choice;
	
	cout << "Enter the display size. 1 is the lowest, meaning one pixel per cell." << endl;
	cin >> cellSize;
	
	while(true){
		if(choice == 'r'){ //set the matrix directly
			int im_width, im_height;
			cout << "Enter image width: ";
			cin >> im_width;
			cout << "Enter image height: ";
			cin >> im_height;
			
			seed.create(Size(im_width, im_height), 0);
			
			int live_percentage;
			cout << "Enter the millentage of cells that start alive (0-1000): ";
			cin >> live_percentage;
			demo.setRand(im_width, im_height, live_percentage);
			break;
		} else { //choices where the image is viewed, and cellsize and threshold determines the seed matrix
			if(choice == 'c'){
				//capture camera image
				VideoCapture capture(0);
				if(!capture.isOpened()){  // check if we succeeded
					cerr << "Could not capture from camera." << endl;
					exit(1);
				}
				capture >> seed;
			} else if(choice == 'i'){
				string image_path;
				cout << "Enter the path to the image (relative or absolute): ";
				cin >> image_path;
				
				seed = imread( image_path.c_str(), 0 );
				if( !seed.data ){
					cerr << "Invalid image supplied." << endl;
					exit(1);
				}
			}
			
			resize( seed, seed, Size(0,0), 1.0/static_cast<double>(cellSize), 1.0/static_cast<double>(cellSize), INTER_NEAREST );
			Mat img;
			resize( seed, img, Size(0,0), static_cast<double>(cellSize), static_cast<double>(cellSize), INTER_NEAREST );
			
			displayImage(img, "Camera capture");
			interactive_threshold(seed);
			cvDestroyWindow("Camera capture");
			
			demo.setMat(seed);
			
			break;
		}
	}
}

void getBitString(unsigned long & config, const char * userInput){
	unsigned long setbit = 1 << 17;
	config = 0;
	for(int i = 0; i < 18; i++){
		if(userInput[i] == '\0'){
			cout << "Bitstring too short. You must specify an 18 bit string." << endl;
			exit(1);
		}
		if( userInput[i] == '1' ){
			config |= setbit;
		}
		setbit = setbit >> 1;
	}
}

//output functions:

void displayImage(Mat & data, const char * cam_cap_title){
	namedWindow(cam_cap_title, CV_WINDOW_AUTOSIZE);
	imshow(cam_cap_title, data);
	//wait X milliseconds, discard result. This gives highgui time to process the image show call above
	waitKey(100);
}

void saveImage(Mat & frame){
	cout << "Enter a name to save the image as. This can be a relative or absolute path, and must NOT include the extension: "; 
	string imageName;
	std::cin >> imageName;
	imageName = imageName + string(".jpg");
	imwrite( imageName, frame );
	cout << "Image saved." << endl;
}

void printControls(){
	cout << "Cellular Automaton controls: " << endl
			 << "esc - exit" << endl
			 << "h - display this message" << endl
			 << "spacebar - pause/unpause" << endl
			 << "a - switch to different Cellular Automaton" << endl
			 << "f - run a specified number of ticks" << endl
			 << "w - run until stable" << endl
			 << "v - toggle verbose (displays ticks)" << endl
			 << "s - save image to disk" << endl
			 << "> - Speed up simulation (50 ms)" << endl
			 << "< - Slow down simulation (50 ms)" << endl
			 << "shift + > - Speed up simulation (10 ms)" << endl
			 << "shift + < - Slow down simulation (10 ms)" << endl
			 << "right arrow - step" << endl;
}
