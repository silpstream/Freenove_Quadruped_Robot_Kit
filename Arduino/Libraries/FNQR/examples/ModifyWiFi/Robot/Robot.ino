/*
 * Sketch     Modify WiFi name and password for robot
 * Platform   Freenove Quadruped Robot (Arduino/Genuino Mega 2560)
 * Brief      This sketch is used to show how to modify WiFi name and password when using default function.
 * Author     Ethan Pan @ Freenove (support@freenove.com)
 * Date       2017/06/22
 * Copyright  Copyright © Freenove (http://www.freenove.com)
 * License    Creative Commons Attribution ShareAlike 3.0
 *            (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

// Include FNQR (Freenove Quadruped Robot) library
#include <FNQR.h>

FNQR robot;

void setup() {
  // Set WiFi name and password
  // Call this function before robot.Start()
  // The WiFi password is case sensitive and at least 8 characters
  robot.SetWiFi("name", "password");
  // Start Freenove quadruped robot with default function
  robot.Start(true);
}

void loop() {
  // Update Freenove quadruped robot
  robot.Update();
}

