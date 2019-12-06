Установить можно двумя способами:

Способ 1. ТОЛЬКО ДЛЯ MELODIC
1. Клонировать репозиторий в домашнюю директорию;
2. Перейти в директорию с папкой catkin_ws;
3. Прописать команды:

catkin_make

source ~/unknown-labirinter/catkin_ws/devel/setup.bash

roslaunch robocake_gazebo demo.launch


Способ 2. ДЛЯ ЛЮБОЙ ВЕРСИИ (ЕСЛИ УЖЕ СТОЯЛА ПАПКА CATKIN_WS)
1. Скачать папку beginner из текущего репозитория;
2. Заменить существующую папку beginner из catkin_ws/src/ на новую.
3. Прописать команды:

catkin_make

source ~/unknown-labirinter/catkin_ws/devel/setup.bash

roslaunch robocake_gazebo demo.launch


Способ 3. ДЛЯ ЛЮБОЙ ВЕРСИИ (ЕСЛИ НЕ БЫЛО ВООБЩЕ НИЧЕГО)
1. Сначала установить роботиков отсюда: https://github.com/robocake/beginner;
2. Перейти к способу 2.


В Gazebo открывается мир с лабиринтом, в котором (по умолчанию в центре) стоит робот.
Робот не знает карты и, используя алгоритм Люка-Тремо, ищет выход (серую клетку).
Нашел - остановился. Перетащили серую клетку - снова ищет.
