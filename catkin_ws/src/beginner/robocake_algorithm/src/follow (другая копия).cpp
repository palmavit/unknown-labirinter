#include "algorithm.h"
#include "math.h"

int howMuchWayNow(int left, int head, int right, int tail) {
int h = 0;
if (left == 0 || left == 1) {
h++;
}
if (right == 0 || right == 1) {
h++;
}
if (right == 0 || right == 1) {
h++;
}
if (tail == 0 || tail == 1) {
h++;
}
return h;
}

int whereICanGo(int left, int head, int right, int tail) {
if (left == 0 || left == 1) {
return 1;
}
if (head == 0 || head == 1) {
return 2;
}
if (right == 0 || right == 1) {
return 4;
}
if (tail == 0 || tail == 1) {
return 8;
}
return 0;
}

int minValueWall(int left, int head, int right, int tail) {
if (left == 0 || head == 0 || right == 0 || tail == 0) {
return 0;
}
if (left == 1 || head == 1 || right == 1 || tail == 1) {
return 1;
}
else {
return 2;
}
}

void changeDirect(int& currDirect, int turn) {
if (turn == 1 || turn == 3) {
currDirect -= turn;
if (currDirect < 1) {
currDirect += 4;
}
}
if (turn == 4) {
currDirect -= 2;
if (currDirect < 1) {
currDirect += 4;
}
}
}

void getLHRT(int direct, int*& left, int*& head, int*& right, int*& teil, int* dir2left, int* dir2head, int* dir2right, int* dir2teil) {
if (direct == 1) {
left = dir2teil;
head = dir2left;
right = dir2head;
teil = dir2right;
}
if (direct == 2) {
left = dir2left;
head = dir2head;
right = dir2right;
teil = dir2teil;
}
if (direct == 3) {
left = dir2head;
head = dir2right;
right = dir2teil;
teil = dir2left;
}
if (direct == 4) {
left = dir2right;
head = dir2teil;
right = dir2left;
teil = dir2head;
}
}

void changeCurrencyPos(int& currencyX, int& currencyY, int direct) {
if (direct == 1) {
currencyY--;
}
if (direct == 2) {
currencyX--;
}
if (direct == 3) {
currencyY++;
}
if (direct == 4) {
currencyX++;
}
}

void changeWallValue(int* teil) {
*teil += 1;
}

void updateKnowledge(int*& rleft, int*& rhead, int*& rright, int*& rteil, int left, int head, int right, int teil) {
if (left != -1) {
*rleft = 0;
}
else {
*rleft = -1;
}
if (head != -1) {
*rhead = 0;
}
else {
*rhead = -1;
}
if (right != -1) {
*rright = 0;
}
else {
*rright = -1;
}
if (teil != -1) {
*rteil = 0;
}
else {
*rteil = -1;
}
}



int search_direction{1};

geometry_msgs::Twist turn_left(int &steps) {
  geometry_msgs::Twist msg;
  if (steps >= 2) {
    steps--;
    msg.linear.x = 0;
    msg.angular.z = 3.95;
  } else if (steps == 1) {
    steps--;
    msg.linear.x = 0;
    msg.angular.z = 3.95;
  }
  return msg;
}

geometry_msgs::Twist turn_right(int &steps) {
  geometry_msgs::Twist msg;
  if (steps >= 2) {
    steps--;
    msg.linear.x = 0;
    msg.angular.z = -3.95;
  } else if (steps == 1) {
    steps--;
    msg.linear.x = 0;
    msg.angular.z = -3.95;
  }
  return msg;
}

geometry_msgs::Twist ride_back(int &steps) {
  geometry_msgs::Twist msg;
  steps--;
  msg.linear.x = -0.3525;
  msg.angular.z = 0.0;
  return msg;
}

geometry_msgs::Twist ride_forward(int &steps) {
  geometry_msgs::Twist msg;
  steps--;
  msg.linear.x = 0.3525;
  msg.angular.z = 0.0;
  return msg;
}



geometry_msgs::Twist control(
                     std::unordered_map<std::string, double> proximity,
                     std::unordered_map<std::string, double> reflectance,
                     double threshold) {
  geometry_msgs::Twist msg;
  static int time = 0;
static bool isTurned = true;

    static int left_steps = 5;
    static int right_steps = 0;
    static int back_steps = 0;
    static int forward_steps = 0;
    static int stop_steps = 0;
    static int stop_for_steps = 0;
    	static int currDirect = 2;
    static bool isTurnedAfter = false;
    static bool isRide = false;

while(stop_for_steps > 0){
     stop_for_steps--;
     return msg;
    }
    while (left_steps > 0) {
isTurnedAfter = true;
isRide = true;
      stop_steps=2;
      return turn_left(left_steps);
    }
    while (right_steps > 0) {
isTurnedAfter = true;
isRide = true;
      stop_steps=2;
      return turn_right(right_steps);
    }
        while(stop_steps > 0){
     stop_steps--;
     return msg;
    }
    while (back_steps > 0) {
isTurnedAfter = true;
isRide = true;
      return ride_back(back_steps);
    }
    while (forward_steps > 0) {
isTurnedAfter = true;
isRide = true;
      return ride_forward(forward_steps);
    }

if(isTurnedAfter){
isTurnedAfter = false;
double direction =(proximity["right_center"]-proximity["left_center"]);
if(direction != 0 && proximity["right_center"] <= 0.1 && proximity["left_center"] <= 0.1){
direction = direction>0?1:-1;
double alfa = (M_PI/2 - atan(2*0.052/(proximity["right_center"]-proximity["left_center"])*direction))*3.95*20/M_PI*2;
msg.angular.z = alfa*direction;
} /*else {
direction = (proximity["right_left"]-proximity["left_left"]);
if(direction != 0 && proximity["right_left"] <= 0.1 && proximity["left_left"] <= 0.1){
direction = direction>0?1:-1;
double alfa = (M_PI/2 - atan(2*0.037/(proximity["right_left"]-proximity["left_left"])*direction))*3.95*200/M_PI*2;
msg.angular.z = alfa*direction;
}
}*/
return msg;
}
if(isRide){
isRide = false;
double direction = proximity["right_center"];
if(direction != 0 && proximity["right_center"] <= 0.1 && proximity["left_center"] <= 0.1){
msg.linear.x = (direction-0.04)*0.3525/0.036;
}
return msg;
}

	static int n1 = 11, n2 = 10, m1 = 10, m2 = 11;
	static int** matrixHorizontalWalls = new int* [n1];
	static int** matrixVerticalWalls = new int* [n2];
	static int** robotKnowMatrixHorizontalWalls = new int* [n1];
	static int** robotKnowMatrixVerticalWalls = new int* [n2];
	static int currencyX = 5, currencyY = 5;
        time++;
	if(time>=100 && (time % 10 == 0)){
	if (time == 100) {
		left_steps = 5;
		matrixHorizontalWalls[0] = new int[10]{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
		matrixHorizontalWalls[1] = new int[10]{ 0, -1, 0, 0, 0, -1, -1, 0, -1, 0 };
		matrixHorizontalWalls[2] = new int[10]{ 0, -1, -1, 0, 0, -1, 0, 0, 0, 0 };
		matrixHorizontalWalls[3] = new int[10]{ 0, 0, 0, 0, 0, 0, 0, -1, -1, 0 };
		matrixHorizontalWalls[4] = new int[10]{ 0, 0, -1, -1, 0, 0, 0, 0, 0, 0 };
		matrixHorizontalWalls[5] = new int[10]{ 0, 0, 0, 0, -1, 0, 0, 0, -1, 0 };
		matrixHorizontalWalls[6] = new int[10]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		matrixHorizontalWalls[7] = new int[10]{ 0, 0, 0, 0, -1, -1, -1, 0, 0, 0 };
		matrixHorizontalWalls[8] = new int[10]{ 0, -1, 0, -1, 0, 0, 0, -1, -1, 0 };
		matrixHorizontalWalls[9] = new int[10]{ 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 };
		matrixHorizontalWalls[10] = new int[10]{ -1, -1, -1, -1, -1, -1, -1, -1, 8, -1 };

		matrixVerticalWalls[0] = new int[11]{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
		matrixVerticalWalls[1] = new int[11]{ -1, 0, -1, 0, -1, 0, 0, -1, -1, -1, -1 };
		matrixVerticalWalls[2] = new int[11]{ -1, -1, 0, -1, -1, 0, -1, -1, 0, -1, -1 };
		matrixVerticalWalls[3] = new int[11]{ -1, 0, 0, 0, -1, -1, -1, -1, 0, 0, -1 };
		matrixVerticalWalls[4] = new int[11]{ -1, 0, -1, 0, 0, -1, -1, -1, -1, -1, -1 };
		matrixVerticalWalls[5] = new int[11]{ -1, -1, 0, -1, -1, 0, -1, -1, -1, 0, -1 };
		matrixVerticalWalls[6] = new int[11]{ -1, 0, 0, -1, 0, -1, 0, -1, 0, -1, -1 };
		matrixVerticalWalls[7] = new int[11]{ -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1 };
		matrixVerticalWalls[8] = new int[11]{ -1, -1, 0, -1, -1, -1, -1, 0, 0, -1, -1 };
		matrixVerticalWalls[9] = new int[11]{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
		time++;

		for (int i = 0; i < n1; i++) {
			robotKnowMatrixHorizontalWalls[i] = new int[m1];
			for (int j = 0; j < m1; j++) {
				robotKnowMatrixHorizontalWalls[i][j] = -2;
			}
		}
		for (int i = 0; i < n2; i++) {
			robotKnowMatrixVerticalWalls[i] = new int[m2];
			for (int j = 0; j < m2; j++) {
				robotKnowMatrixVerticalWalls[i][j] = -2;
			}
		}
		return msg;
	}

	// Старт


	static int* left = matrixVerticalWalls[currencyX] + currencyY;
	static int* head = matrixHorizontalWalls[currencyX] + currencyY;
	static int* right = matrixVerticalWalls[currencyX] + currencyY + 1;
	static int* teil = matrixHorizontalWalls[currencyX + 1] + currencyY;
	static int* rleft = robotKnowMatrixVerticalWalls[currencyX] + currencyY;
	static int* rhead = robotKnowMatrixHorizontalWalls[currencyX] + currencyY;
	static int* rright = robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1;
	static int* rteil = robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY;
	updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
	while (*left != 8 && *head != 8 && *right != 8 && *teil != 8) {
		int minValue = minValueWall(*left, *head, *right, *teil);
		if (*left == minValue) {
			changeDirect(currDirect, 1);
			changeCurrencyPos(currencyX, currencyY, currDirect);
			left_steps=5; forward_steps=5;
			getLHRT(currDirect, left, head, right, teil, matrixVerticalWalls[currencyX] + currencyY, matrixHorizontalWalls[currencyX] + currencyY, matrixVerticalWalls[currencyX] + currencyY + 1, matrixHorizontalWalls[currencyX + 1] + currencyY);
getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(teil);
updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
}
else {
if (*head == minValue) {
changeDirect(currDirect, 2);
changeCurrencyPos(currencyX, currencyY, currDirect);
			forward_steps=5;
		
getLHRT(currDirect, left, head, right, teil, matrixVerticalWalls[currencyX] + currencyY, matrixHorizontalWalls[currencyX] + currencyY, matrixVerticalWalls[currencyX] + currencyY + 1, matrixHorizontalWalls[currencyX + 1] + currencyY);
getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(teil);
updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
}
else {
if (*right == minValue) {
changeDirect(currDirect, 3);
changeCurrencyPos(currencyX, currencyY, currDirect);
			right_steps=5;forward_steps=5;
getLHRT(currDirect, left, head, right, teil, matrixVerticalWalls[currencyX] + currencyY, matrixHorizontalWalls[currencyX] + currencyY, matrixVerticalWalls[currencyX] + currencyY + 1, matrixHorizontalWalls[currencyX + 1] + currencyY);
getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(teil);
updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
}
else {
if (*teil == minValue) {
changeDirect(currDirect, 2);
changeCurrencyPos(currencyX, currencyY, currDirect);
			back_steps=5;
getLHRT(currDirect, left, head, right, teil, matrixVerticalWalls[currencyX] + currencyY,
matrixHorizontalWalls[currencyX] + currencyY, matrixVerticalWalls[currencyX] + currencyY + 1, matrixHorizontalWalls[currencyX + 1] + currencyY);
getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(teil);
updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
}
}
}
}
return msg;
}
}
  return msg;
}

/*
    if (steps > 10) {
      msg.linear.x = 0;
      msg.angular.z = ;
      steps--;
    } else if (steps > 0) {
      msg.linear.x = 1.0;  // max = 2 ~ 1,5 m/s
      msg.angular.z = 0.0; //15.6 = 360* // 1.3s for 360*
      steps--;
    } else {
      msg.linear.x = 0;
      msg.angular.z = 0;
      steps = 11;
    }
*/
