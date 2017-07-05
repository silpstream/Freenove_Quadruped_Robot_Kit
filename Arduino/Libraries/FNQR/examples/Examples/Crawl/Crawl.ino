/*
 * Sketch     Crawl function example
 * Platform   Freenove Quadruped Robot (Arduino/Genuino Mega 2560)
 * Brief      This sketch is used to show how to control Freenove Quadruped Robot.
 *            You can easily achieve custom function by using FNQR library we provide.
 * Author     Ethan Pan @ Freenove (support@freenove.com)
 * Date       2017/05/18
 * Copyright  Copyright © Freenove (http://www.freenove.com)
 * License    Creative Commons Attribution ShareAlike 3.0
 *            (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

// Include FNQR (Freenove Quadruped Robot) library
#include <FNQR.h>

FNQR robot;

void setup() {
  // Custom setup code start

  // Custom setup code end
  // Start Freenove quadruped robot
  robot.Start();
}

void loop() {
  // Custom loop code start
  robot.CrawlForward();
  robot.CrawlBackward();
  robot.TurnLeft();
  robot.TurnRight();
  robot.SleepMode();
  robot.ActiveMode();
  robot.SwitchMode();
  while (true);
  // Custom loop code end
}

