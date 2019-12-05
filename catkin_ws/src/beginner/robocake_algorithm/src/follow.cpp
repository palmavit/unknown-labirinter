#include "algorithm.h"
#include "math.h"
#include <string>
#include "exception"

static int wait_time = 5;

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
if (*rleft == -2) {
if (left != -1) {
*rleft = 0;
}
else {
*rleft = -1;
}
}
if (*rhead == -2) {
if (head != -1) {
*rhead = 0;
}
else {
*rhead = -1;
}
}
if (*rright == -2) {
if (right != -1) {
*rright = 0;
}
else {
*rright = -1;
}
}
if (*rteil == -2) {
if (teil != -1) {
*rteil = 0;
}
else {
*rteil = -1;
}
}
}

int search_direction{ 1 };

geometry_msgs::Twist turn_left(int& steps) {
geometry_msgs::Twist msg;
if (steps >= 2) {
steps--;
msg.linear.x = 0;
msg.angular.z = 3.95;
}
else if (steps == 1) {
steps--;
msg.linear.x = 0;
msg.angular.z = 3.95;
}
return msg;
}

geometry_msgs::Twist turn_right(int& steps) {
geometry_msgs::Twist msg;
if (steps >= 2) {
steps--;
msg.linear.x = 0;
msg.angular.z = -3.95;
}
else if (steps == 1) {
steps--;
msg.linear.x = 0;
msg.angular.z = -3.95;
}
return msg;
}

geometry_msgs::Twist ride_back(int& steps) {
geometry_msgs::Twist msg;
steps--;
msg.linear.x = -0.35;
msg.angular.z = 0.0;
return msg;
}

geometry_msgs::Twist ride_forward(int& steps) {
geometry_msgs::Twist msg;
steps--;
msg.linear.x = 0.35;
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

static int left_steps = 0;
static int right_steps = 0;
static int back_steps = 0;
static int forward_steps = 0;
static int stop_steps = 0;
static int stop_for_steps = 0;
static int currDirect = 2;
static bool isTurnedAfter = false;
static bool isRide = false;
time++;



while (left_steps > 0) {
isTurnedAfter = true;
isRide = true;
stop_steps = 1;
return turn_left(left_steps);
}
while
(right_steps > 0) {
isTurnedAfter = true;
isRide = true;
stop_steps = 1;
return turn_right(right_steps);
}
while (stop_steps > 0) {
stop_steps--;
return msg;
}
while (back_steps > 0) {
isTurnedAfter = true;
isRide = true;
if (back_steps == 1) stop_steps = 1;
return ride_back(back_steps);
}
while (forward_steps > 0) {
isTurnedAfter = true;
isRide = true;
if (forward_steps == 1) stop_steps = 1;
return ride_forward(forward_steps);
}

if (isTurnedAfter) {
isTurnedAfter = false;
double direction = (proximity["right_center"] - proximity["left_center"]);
if (direction != 0 && proximity["right_center"] <= 0.12 && proximity["left_center"] <= 0.12) {
if (std::min(proximity["right_center"], proximity["left_center"]) <= 0.01 && std::min(proximity["right_center"], proximity["left_center"]) * 4 < std::max(proximity["right_center"], proximity["left_center"])) {
msg.angular.z = 0.01; isTurnedAfter = true; return msg;
}
direction = direction > 0 ? 1 : -1;
double alfa = (M_PI / 2 - atan(2 * 0.052 / (proximity["right_center"] - proximity["left_center"]) * direction)) * 3.95 * 20 / M_PI * 2;
msg.angular.z = alfa * direction;
stop_steps = 1;
return msg;
}
else {
direction = (proximity["right_left"] - proximity["left_left"]);
if (direction != 0 && proximity["right_left"] <= 0.12 && proximity["left_left"] <= 0.12) {
if (std::min(proximity["right_left"], proximity["left_left"]) <= 0.01 && std::min(proximity["right_left"], proximity["left_left"]) * 4 < std::max(proximity["right_left"], proximity["left_left"])) {
msg.angular.z = 0.01; isTurnedAfter = true; return msg;
}
direction = direction > 0 ? 1 : -1;
double alfa = (M_PI / 2 - atan(0.048 / (proximity["right_left"] - proximity["left_left"]) * direction)) * 3.95 * 5 / M_PI * 2;
msg.angular.z = alfa * direction;
//throw std::logic_error(std::to_string(proximity["right_left"])+" "+std::to_string(proximity["left_left"])+" "+std::to_string(alfa));
stop_steps = 1;
return msg;
} else {
direction = (proximity["right_right"] - proximity["left_right"]);
if (direction != 0 && proximity["right_right"] <= 0.12 && proximity["left_right"] <= 0.12) {
if (std::min(proximity["right_right"], proximity["left_right"]) <= 0.01 && std::min(proximity["right_right"], proximity["left_right"]) * 4 < std::max(proximity["right_right"], proximity["left_right"])) {
msg.angular.z = 0.01; isTurnedAfter = true; return msg;
}
direction = direction > 0 ? 1 : -1;
double alfa = (M_PI / 2 - atan(0.048 / (proximity["right_right"] - proximity["left_right"]) * direction)) * 3.95 * 5 / M_PI * 2;
msg.angular.z = -alfa * direction;
//throw std::logic_error(std::to_string(proximity["right_left"])+" "+std::to_string(proximity["left_left"])+" "+std::to_string(alfa));
stop_steps = 1;
return msg;
} else {
double direction = (proximity["right_back"] - proximity["left_back"]);
if (direction != 0 && proximity["right_back"] <= 0.12 && proximity["left_back"] <= 0.12) {
if (std::min(proximity["right_back"], proximity["left_back"]) <= 0.01 && std::min(proximity["right_back"], proximity["left_back"]) * 4 < std::max(proximity["right_back"], proximity["left_back"])) {
msg.angular.z = 0.01; isTurnedAfter = true; return msg;
}
direction = direction > 0 ? 1 : -1;
double alfa = (M_PI / 2 - atan(2 * 0.052 / (proximity["right_back"] - proximity["left_back"]) * direction)) * 3.95 * 20 / M_PI * 2;
msg.angular.z = -alfa * direction;
stop_steps = 1;
return msg;
}
}
}
}
return msg;
}
if (isRide) {
isRide = false;
double direction = proximity["right_center"];
if (direction != 0 && proximity["right_center"] <= 0.12 && proximity["left_center"] <= 0.12) {
msg.linear.x = (direction - 0.045) * 0.3525 / 0.036;
stop_steps = 1;
}
return msg;
}


if (time > 60 && time < 75) {
double direction = (proximity["right_center"] - proximity["left_center"]);
if (direction != 0 && proximity["right_center"] <= 0.12 && proximity["left_center"] <= 0.12) {
if (std::min(proximity["right_center"], proximity["left_center"]) <= 0.01 && std::min(proximity["right_center"], proximity["left_center"]) * 4 < std::max(proximity["right_center"], proximity["left_center"])) {
msg.angular.z = 0.01;
return msg;
}
direction = direction > 0 ? 1 : -1;
double alfa = (M_PI / 2 - atan(2 * 0.052 / (proximity["right_center"] - proximity["left_center"]) * direction)) * 3.95 * 20 / M_PI * 2;
msg.angular.z = alfa * direction;
stop_steps = 1;
return msg;
}
else {
direction = (proximity["right_left"] - proximity["left_left"]);
if (direction != 0 && proximity["right_left"] <= 0.12 && proximity["left_left"] <= 0.12) {
if (std::min(proximity["right_left"], proximity["left_left"]) <= 0.01 && std::min(proximity["right_left"], proximity["left_left"]) * 4 < std::max(proximity["right_left"], proximity["left_left"])) {
msg.angular.z = 0.01;
return msg;
}
direction = direction > 0 ? 1 : -1;
double alfa = (M_PI / 2 - atan(0.048 / (proximity["right_left"] - proximity["left_left"]) * direction)) * 3.95 * 5 / M_PI * 2;
msg.angular.z = alfa * direction;
stop_steps = 1;
return msg;
}
}

}


if (time > 75 && time < 90) {
double direction = proximity["right_center"];
if (direction != 0 && proximity["right_center"] <= 0.12 && proximity["left_center"] <= 0.12) {
msg.linear.x = (direction - 0.045) * 0.3525 / 0.036;
return msg;
}
}


static int n1 = 11, n2 = 10, m1
= 10, m2 = 11;
static int** robotKnowMatrixHorizontalWalls = new int* [2*std::max(n1, m1)];
static int** robotKnowMatrixVerticalWalls = new int* [2*std::max(n2, m2)];
static int currencyX = std::max(n1, m1), currencyY = std::max(n2, m2);
static int goalX = 9, goalY = 8;
static int left = 0;
static int head = 0;
static int right = 0;
static int teil = 0;

if (time >= 100 && (time % 10 == 0)) {
if (time == 100) {
teil = proximity["right"] < 0.12 ? -1 : 0;
left_steps = 5;
time++;

for (int i = 0; i < 2*std::max(n1, m1); i++) {
robotKnowMatrixHorizontalWalls[i] = new int[2*std::max(n1, m1)];
for (int j = 0; j < 2*std::max(n1, m1); j++) {
robotKnowMatrixHorizontalWalls[i][j] = -2;
}
}
for (int i = 0; i < 2*std::max(n2, m2); i++) {
robotKnowMatrixVerticalWalls[i] = new int[2*std::max(n2, m2)];
for (int j = 0; j < 2*std::max(n2, m2); j++) {
robotKnowMatrixVerticalWalls[i][j] = -2;
}
}
return msg;
}

// Старт

////////////////////////////
if(std::max(proximity["left_left"], proximity["right_left"]) <= 0.01 || std::max(proximity["left_center"], proximity["right_center"]) <= 0.01 ||
std::max(proximity["left_right"], proximity["right_right"]) <= 0.01 || std::max(proximity["left_back"], proximity["right_back"]) <= 0.01){
msg.angular.z = 0.01;
return msg;
}
int tleft  = std::max(proximity["left_left"], proximity["right_left"]) < 0.12 ? -1 : 0;
int thead  = std::max(proximity["left_center"], proximity["right_center"]) < 0.12 ? -1 : 0;
int tright = std::max(proximity["left_right"], proximity["right_right"]) < 0.12 ? -1 : 0;
int tteil = std::max(proximity["left_back"], proximity["right_back"]) < 0.12 ? -1 : 0;


static int* rleft = robotKnowMatrixVerticalWalls[currencyX] + currencyY;
static int* rhead = robotKnowMatrixHorizontalWalls[currencyX] + currencyY;
static int* rright = robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1;
static int* rteil = robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY;
static bool isClear = false;


if(!isClear) {getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
updateKnowledge(rleft, rhead, rright, rteil, tleft, thead, tright, tteil);
}
else {if(reflectance["center"] > 0.8) {stop_steps = 2; isClear = false;} return msg;}
if (reflectance["center"] < 0.8 && !isClear){
currencyX = std::max(n1, m1); currencyY = std::max(n2, m2);
for (int i = 0; i < 2*std::max(n1, m1); i++) {
for (int j = 0; j < 2*std::max(n1, m1); j++) {
robotKnowMatrixHorizontalWalls[i][j] = -2;
}
}
for (int i = 0; i < 2*std::max(n2, m2); i++) {
for (int j = 0; j < 2*std::max(n2, m2); j++) {
robotKnowMatrixVerticalWalls[i][j] = -2;
}
}
isClear = true;
}
while (reflectance["center"] > 0.8) {
int maxValue = std::max(*rleft, std::max(*rhead, std::max(*rright, *rteil)));
if(maxValue==-1) return msg;
int minValue = maxValue;
if(*rleft!=-1) minValue=std::min(minValue, *rleft);
if(*rhead!=-1) minValue=std::min(minValue, *rhead);
if(*rright!=-1) minValue=std::min(minValue, *rright);
if(*rteil!=-1) minValue=std::min(minValue, *rteil);
if (*rleft == minValue) {
changeDirect(currDirect, 1);
changeCurrencyPos(currencyX, currencyY, currDirect);
left_steps = 5; forward_steps = 5;

//getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(rteil);
//updateKnowledge(rleft, rhead, rright, rteil, left, head, right, teil);
}
else {
if (*rhead == minValue) {
changeDirect(currDirect, 2);
changeCurrencyPos(currencyX, currencyY, currDirect);
forward_steps = 5;

//getLHRT(currDirect, left, head, right, teil, matrixVerticalWalls[currencyX] + currencyY, matrixHorizontalWalls[currencyX] + currencyY, matrixVerticalWalls[currencyX] + currencyY + 1, matrixHorizontalWalls[currencyX + 1] + currencyY);
//getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(rteil);
//updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
}
else {
if (*rright == minValue) {
changeDirect(currDirect, 3);
changeCurrencyPos(currencyX, currencyY, currDirect);
right_steps = 5; forward_steps = 5;
//getLHRT(currDirect, left, head, right, teil, matrixVerticalWalls[currencyX] + currencyY, matrixHorizontalWalls[currencyX] + currencyY, matrixVerticalWalls[currencyX] + currencyY + 1, matrixHorizontalWalls[currencyX + 1] + currencyY);
//getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(rteil);
//updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
}
else {
if (*rteil == minValue) {
changeDirect(currDirect, 4);
changeCurrencyPos(currencyX, currencyY, currDirect);
left_steps = 10; forward_steps = 5;
//getLHRT(currDirect, left, head, right, teil, matrixVerticalWalls[currencyX] + currencyY,
//matrixHorizontalWalls[currencyX] +
//currencyY, matrixVerticalWalls[currencyX] + currencyY + 1, matrixHorizontalWalls[currencyX + 1] + currencyY);
//getLHRT(currDirect, rleft, rhead, rright, rteil, robotKnowMatrixVerticalWalls[currencyX] + currencyY, robotKnowMatrixHorizontalWalls[currencyX] + currencyY, robotKnowMatrixVerticalWalls[currencyX] + currencyY + 1, robotKnowMatrixHorizontalWalls[currencyX + 1] + currencyY);
changeWallValue(rteil);
//updateKnowledge(rleft, rhead, rright, rteil, *left, *head, *right, *teil);
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
msg.linear.x = 1.0; // max = 2 ~ 1,5 m/s
msg.angular.z = 0.0; //15.6 = 360* // 1.3s for 360*
steps--;
} else {
msg.linear.x = 0;
msg.angular.z = 0;
steps = 11;
}
*/
