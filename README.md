# Velociraptor-Robot
## Project's description
This project is a differential drive robot with an ultrasonic sensor that implements ROS 2. The robot can move forward, backward, left, and right by commands, and if it detects an obstacle, it automatically stops.

## Purpose and inspiration of the project
1. Creating our first GIT project  
2. Learning to use ROS2 and connect publisher-subscriber nodes through topics  

## Parts of the project
-	ROS2 nodes in Python
-	ESP32 transmission in C++ using PlatformIO

## Hardware components
-	2 DC motors for robot movement  
-	1 ultrasonic sensor (HC-SR04)
-	ESP32 microcontroller
-	Robot chassis with wheels and power supply system  

## Software architecture
### ESP32
The ESP32 is a TCP server on the port 8080; it receives the measures done by the ultrasound sensor, sending the command to the DC motors to stop (velocity=0) or to change their direction.
### ROS2 nodes
The ROS 2 system is based on a publisher-subscriber architecture, where nodes communicate through topics to control the robot.

**Publisher**  
The publisher node is executed from the terminal and is responsible for sending movement commands. It uses commands defined by the user on the topic `/wifi_transmitter`, in the format `<direction>:<velocity>`.

**Subscriber node**  
This node acts as a bridge between ROS2 and the ESP32. It subscribes to the topic `/wifi_transmitter` to receive the commands sent by the publisher, which are then sent to the ESP32 via Wi-Fi.

**Topics**  
The system uses the topic `/wifi_transmitter`. Its type is `std_msgs/msg/String` and its purpose is to transmit the movement commands from ROS2 to the ESP32.

**Node interaction**  
The user publishes a command on /wifi_transmitter -> The subscriber node receives the message -> The subscriber sends the command to the ESP32 -> The ESP32 executes the movement

## Configuration and installation

### ESP32 configuration
1. Download the package titled WifiESP
Open the package in the terminal with the route <user>/<directory>/<WifiESP32>
Open the code with the command `code . `
2. Check the port to which the ESP32 is connected  
   a. Go to PlatformIO's configuration  
   b. Follow the route  
     `Default` → `general` → `devices` 
   c. Check the port's number on the terminal  
     For example:  
     `/dev/ttyUSB0`  
3. Grant permits to the port
Use the command:
`sudo chmod 777 /dev/ttyUSB0`   
**Note:** check the number of the port the USB is connected to and change it in this command if needed.   
4. Upload the code  
a. Go to the serial monitor  
b. Look for the IP  
c. Copy the IP
5. Disconnect the USB cable from the ESP32  

### ROS2 configuration
1. Create workspace
Download the package
2. Go to the workspace
a. Use the command:
`<route> /ras_ws`  
b. Open the code with the command `code .`
3. Open 2 terminals on Visual Studio Code
One of them will be the publisher and the other one will be the subscriber.
4. Go to the subscriber file
`wifi/transmitter.py`
Modify the IP address: paste the IP copied during the ESP32 configuration. 
**Note:** give permissions 755 to the executable transmitter file.
5. Go to the subscriber’s terminal
a. (Only the first time this process is done)
```bash
source install/setup.bash
```
b. From the worskpace
```bash
colcon build
cd ~/ras_ws
```
6. On the publisher’s terminal
The publisher is going to communicate through commands.
Execute (Only the first time):
```bash
source install/setup.bash
```
7. Execute the subscriber 
```bash
ros2 run <package_name> <subscriber_name>
```
In this case run the command:
```bash
ros2 run WifiESP wifi_transmitter
```
8. On the publisher’s terminal run the following command
To publish on the topic `wifi_transmitter`: 
```bash
ros2 topic pub wifi_transmitter std_msgs/msgs/string “data: ‘R:40’” -- once
```
**Note:** The command `“data: ‘R:40’”` has the structure ‘letter:number’, which represent ‘directon:velocity’


### Robot operating principles
The robot’s motors move with a speed and direction determined by the user, sent as a publisher on the terminal. When the ultrasound senses an obstacle among the set range (0-10cm), it stops the DC motors to stop the robot’s motion. 
For the user to determine the speed and direction of the robot, they will be able to control two parameters on the command shown below, which has already been explained:
```bash
ros2 topic pub wifi_transmitter std_msgs/msgs/string “data: ‘R:40’” -- once
```
- To modify the direction, the letters used are A (advance), R (reverse), I (turn to the left) and D (turn to the right).
- To modify the velocity of the motors, any number between 10 and 255 can be used. If the number is lower than 10, the established velocity will be 10, and if the number is higher than 255, the established velocity will be 255.

## Block diagram of the system


## Final product
https://youtube.com/shorts/Rtv1JAVCud8

## Authors
- *Valeria Andrea Parra García* – valeriaparrag@javeriana.edu.co
- *María Alejandra Díaz Ortiz* – mariaa-diaz@javeriana.edu.co
