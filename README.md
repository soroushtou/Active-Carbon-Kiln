# Active Carbon Kiln
This is the code for my active carbon kiln project.

- "EX27_no_preheat.ino" is the final arduino file used for the main controller
- "Project_main.c" is the program for the conveying scale
- "Project(CAVR file).prj" is the CodevisionAVR file format of the "Project_main.c"
- "test0.xls" is the Excel file for receiving data from the min controller

This kiln makes active carbon from pistachio crusts. Pistachio crusts enter from one side, hot air and mist enter from the other side, a chemical reaction occurs, and active carbon powder is created. I did the Electrical part of this kiln.

The whole setup was made to optimize the conditions of the chemical reaction. A separate weighting conveyor unit calculates the crust's mass/time that sends the data to the central controller. The entire data is sent to a computer and can be viewed live from Excel.
Main Features:

  - Controllable Temperature
  - Controllable Mist amount
  - Controllable Feeding rate
  - Controllable Rotation speed
  - Measured Exhausted gas 
  - All parameters and data are sent to Excel

________________________________________________________________________________________________________________________
![1](https://github.com/soroushtou/Active-Carbon-Kiln/blob/main/Project1/1%20(7).jpg)
This is the whole Kiln structure. Pistachio crusts are fed through the hopper (right side of the kiln) and moved to the kiln with a screw conveyor. The kiln itself rotates slowly with a wheelchair motor and some belt-gear transmitters. By having a slight angle and rotation, crusts are moved slowly to the left side(end of the kiln). 

Heat and water mist are generated by a combustion heater(methane gas) and three ultrasonic piezoelectrics respectively on the left side.

There is a double-sided cylinder in the middle of the kiln. The Hot air and Mist move inside the cylinder to the right side and travel back to the left side, and exit.

The active carbon powder made from the crusts moves to a long pipe floating inside a polymer, bigger pipe filled with water to cool down and then exits to the small weighing conveyor.

________________________________________________________________________________________________________________________
![2](https://github.com/soroushtou/Active-Carbon-Kiln/blob/main/Project1/1%20(11).jpg)
You can see the combustion heater, fueled by methane gas. The electrical motor-valve, controls the inlet methane(and thus temperature) and the position is aquired by potentiometer. The temperature is measured by a K-type Thermocouple amplified by an op-amp circuit.

The mist is created by three piezoelectrics inside the metal box and an AC fan blows it inside the kiln.

The water level is measured and the controller opens a water valve if water level is low. 

________________________________________________________________________________________________________________________
![3](https://github.com/soroushtou/Active-Carbon-Kiln/blob/main/Project1/1%20(14).jpg)
![4](https://github.com/soroushtou/Active-Carbon-Kiln/blob/main/Project1/1%20(2).jpg)
The cooled powder crosses over this conveyor belt placed on 3 loadcells which measures the output mass/time of the powder. The controller board is based on an Atmega32 and 3 HX711 loadcell Modules. The program is written in C language and Codevision AVR. The data is sent to the main controller via Serial Rx/Tx protocol.

________________________________________________________________________________________________________________________
![5](https://github.com/soroushtou/Active-Carbon-Kiln/blob/main/Project1/1%20(9).jpg)
![6](https://github.com/soroushtou/Active-Carbon-Kiln/blob/main/Project1/1%20(4).jpg)
This is the main board. The main controller is an Arduino Mega2560 board, and the program is written in C++ language and ArduinoIDE. For the kiln and feeder motors, we used 2 BTS7960 modules, and for the fuel inlet valve, an L298N module.

The exhausted gas inside the kiln is pumped to the analyzing section, with MQ-2,3,4,7,9,135 and 214 sensors. There are two valves, one will allow exhausted gas to go through the analyzer, and the other is used to cleanse the sensors with filtered air. 10 Relays are used for the On/Off function of mist makers, gas pump, mist blower fan, water level adjustment, water pump, and cooler pipe motor. An MPU9650 sensor measures the angle of the kiln in the beginning. A 24V SMPS, a 12V SMPS, and three 5V regulated power supplies are used to power the entire system.

________________________________________________________________________________________________________________________
![7](https://github.com/soroushtou/Active-Carbon-Kiln/blob/main/Project1/1%20(5).jpg)
Current time, temperature, voltages of all 7 MQ sensors, current methane valve position, feeder RPM and mass/time of the powder are sent to the computer. The data is received via Excel and PLX-DAQ add-in. A live chart is drawn based on the MQ sensors.
