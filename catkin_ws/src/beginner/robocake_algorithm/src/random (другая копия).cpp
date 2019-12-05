#include "algorithm.h"

#include <random>

//#include <unistd.h>
#include <ctime>
#include <cstdlib>

int direction{0};

void ride(geometry_msgs::Twist &msg, double speed) {
  msg.linear.x = speed;
  msg.angular.z = 0;
}

void rotate(geometry_msgs::Twist &msg, double angle) {
  msg.linear.x = 0;
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

  if (proximity["front"] > 0.1 && proximity["left"] > 0.1 &&
      proximity["right"] > 0.1 &&
      reflectance["left"] > threshold && reflectance["right"] > threshold) {

    double angle = M_PI;  // 1 оборот ~ 5*M_PI либо при left_steps = 25;
    if (back_steps) {
      back_steps--;
      ride(msg, -0.2);
    } else if (left_steps) {
      left_steps--;
      rotate(msg, angle);
    } else if (right_steps) {
      right_steps--;
      rotate(msg, -angle);
    } else if (forward_steps) {
      forward_steps--;
      ride(msg, 0.2);
    } else {
      srand(time(0));
      if (d(g) < 0.5)
        left_steps = rand() % 5 + 1;
      else
        right_steps = rand() % 5 + 1;
      forward_steps = rand() % 10 + 5;
    }
    //direction = d(g) < 0.5 ? 1 : (d(g) < 0.5 ? 0 : -1);  // 0 направление движения
    //if (!direction)
    //  msg.linear.x = 0.2;  // 0.2 скорость движения
    //else {
    //  msg.linear.x = 0;
    //  msg.angular.z = direction * M_PI / 2;  // 0 угол поворота
    //}
  } else {  // есть какое-то препятствие
    back_steps = 0;
    left_steps = 1;  // ~ 15 градусов
    forward_steps = rand() % 10 + 5;
    ride(msg, -0.2);
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
