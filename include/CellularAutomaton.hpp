// Copyleft: You are free to use this any way you like, as long as the file remains attributed to me, Tama McGlinn. Written July 23nd 2013

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include <bitset>
using std::bitset;

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

/* The rule is written as a bitset in the following format:
 * 8 7 6 5 4 3 2 1 0 | 8 7 6 5 4 3 2 1 0 - sum of neighbours
 * bit is alive (S)  | bit is dead (B)   - sum of centre ( multiply by 9 to get index (0,17) with which to find applicable rule )
 * 1 1 1 1 1 0 0 0 0 | 1 1 1 1 0 0 0 0 0 - Example: make islands
 * 0 0 0 0 0 1 1 0 0 | 0 0 0 0 0 1 0 0 0 - Example: life
 * 0 0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 1 0 0 - Example: seeds
 * You can read the numbers backwards and see the usual CA format: B3/S23 for life
 * If you read it forwards as a binary number, you get the rule with which to initialise the CellularAutomaton
 */

class CellularAutomaton {
public:
	CellularAutomaton(bitset<18> rule);
	void setAutomaton(bitset<18> new_rule);
	void setMat(Mat & seed);
	void setRand(int width, int height, int millentage, int seed);

	void timestep();
	bool timestep_checkChange();
	Mat getMatrix();
	
private:
	Mat m_data;
	bitset<18> m_rule;
	
	//0-8 stores the neighbourhood sum if the cell is dead, 9-17 if it is alive; 
	//e.g. a sum of 12 means the centre is alive and 3 neighbours
	const Mat kernel;
	const Point anchor;//always indicates center
	const double delta;//value to be added during filter2D
	const int ddepth;//indicating the second param of filter2D has the same depth as the first param
};
