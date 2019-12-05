#include "algorithm.h"

#include <random>

//#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <math.h>

int direction{0};

void ride(geometry_msgs::Twist &msg, double speed) {
  msg.linear.x = speed;
  msg.angular.z = 0;
}

void rotate(geometry_msgs::Twist &msg, double angle) {
  msg.linear.x = 0;
  msg.angular.z = angle;
}
void circle(geometry_msgs::Twist &msg, double speed, double angle){
  msg.linear.x = speed;
  msg.angular.z = angle;
} 

geometry_msgs::Twist control(
                     std::unordered_map<std::string, double> proximity,
                     std::unordered_map<std::string, double> reflectance,
                     double threshold) {
  std::uniform_real_distribution<> d{0, 1};
  std::random_device r;
  std::default_random_engine g(r());
  geometry_msgs::Twist msg;
  static int forward_steps = 0;  // сколько осталось ехать прямо
  static int back_steps = 0;  // сколько осталось ехать назад
  static int left_steps = 0;  // сколько осталось поворачивать влево
  static int right_steps = 0;  // сколько осталось поворачивать вправо
  static int num = 0;
  static double time = 0;
  static double step = 0.01;
  double R = 0.2 * time + 0.033333333;

  double omega = 0.2 / R;
  /*circle(msg, sqrt(0.2*time), 1);*/
  if (proximity["front"] > 0.1 && proximity["left"] > 0.1 &&
      proximity["right"] > 0.1 &&
      reflectance["left"] > threshold && reflectance["right"] > threshold) {

    if (back_steps) {
      back_steps--;
      ride(msg, -0.2);
    } else if (left_steps) {
      left_steps--;
      rotate(msg, 0.25);
    } else {
      circle(msg, 0.2, omega);
      time += step;
    }
    //direction = d(g) < 0.5 ? 1 : (d(g) < 0.5 ? 0 : -1);  // 0 направление движения
    //if (!direction)
    //  msg.linear.x = 0.2;  // 0.2 скорость движения
    //else {
    //  msg.linear.x = 0;
    //  msg.angular.z = direction * M_PI / 2;  // 0 угол поворота
    //}
  } else {  // есть какое-то препятствие
    back_steps = 2;
    left_steps = 1;  // ~ 15 градусов
    ride(msg, -0.2);
    time = 0;
    step = (double) (rand() % 100) / 1000;
    //if (!direction) {
      //if (proximity["right"] < 0.1 || reflectance["right"] < threshold) {
      //  direction = 1;
      //} else if (proximity["left"] < 0.1 || reflectance["left"] < threshold) {
      //  direction = -1;
      //} else {
      //  direction = d(g) < 0.5 ? 1 : -1;
      //}
      //if (d(g) < 0.2) {
      //  direction *= -1;
      //}
    //}
    //msg.linear.x = 0;
    //msg.angular.z = direction * M_PI / 2;
  }
  return msg;
}
