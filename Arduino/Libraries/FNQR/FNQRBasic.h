/*
 * File				Basic classes for Freenove Quadruped Robot
 * Author			Ethan Pan @ Freenove (support@freenove.com)
 * Date				2017/06/24
 * Copyright	Copyright © Freenove (http://www.freenove.com)
 * License		Creative Commons Attribution ShareAlike 3.0
 *						(http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

#pragma once
#if defined(__AVR_ATmega2560__)

#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#include <FlexiTimer2.h>

class RobotShape
{
public:
	static const float a = 35;
	static const float b = 35;
	static const float c = 15.75;
	static const float d = 22.75;
	static const float e = 55;
	static const float f = 70;
};

class EepromAddresses
{
public:
	static const float robotState = 0;
	
	static const float servo22 = 10;
	static const float servo23 = 12;
	static const float servo24 = 14;
	static const float servo25 = 16;
	static const float servo26 = 18;
	static const float servo27 = 20;
	static const float servo39 = 22;
	static const float servo38 = 24;
	static const float servo37 = 26;
	static const float servo36 = 28;
	static const float servo35 = 30;
	static const float servo34 = 32;
};

class Point
{
public:
	Point();
	Point(float x, float y, float z);

	static float GetDistance(Point point1, Point point2);

	volatile float x, y, z;
};

class RobotLegsPoints
{
public:
	RobotLegsPoints();
	RobotLegsPoints(Point leg1, Point leg2, Point leg3, Point leg4);

	Point leg1, leg2, leg3, leg4;
};

class RobotJoint
{
public:
	RobotJoint();
	void Set(int servoPin, float jointZero, bool jointDir, float jointMinAngle, float jointMaxAngle, int offsetAddress);

	void SetOffset(float offset);
	void SetOffsetEnableState(bool state);

	void RotateToDirectly(float jointAngle);

	float GetJointAngle(float servoAngle);

	bool CheckJointAngle(float jointAngle);

	volatile float jointAngleNow;
	volatile float servoAngleNow;

private:
	Servo servo;
	int servoPin;
	float jointZero;
	bool jointDir;
	float jointMinAngle;
	float jointMaxAngle;
	int offsetAddress;
	volatile float offset = 0;
	volatile bool isOffsetEnable = true;
	volatile bool isFirstRotate = true;
	const int firstRotateDelay = 50;
};

class RobotLeg
{
public:
	RobotLeg();
	void Set(float xOrigin, float yOrigin);

	void SetOffsetEnableState(bool state);

	void CalculatePoint(float alpha, float beta, float gamma, volatile float &x, volatile float &y, volatile float &z);
	void CalculatePoint(float alpha, float beta, float gamma, Point &point);
	void CalculateAngle(float x, float y, float z, float &alpha, float &beta, float &gamma);
	void CalculateAngle(Point point, float &alpha, float &beta, float &gamma);

	bool CheckPoint(Point point);
	bool CheckAngle(float alpha, float beta, float gamma);

	void MoveTo(Point point);
	void MoveToRelatively(Point point);
	void WaitUntilFree();

	void MoveToDirectly(Point point);
	void MoveToDirectlyRelatively(Point point);

	void RotateToDirectly(float alpha, float beta, float gamma);

	void ServosRotateTo(float degreeA, float degreeB, float degreeC);

	RobotJoint jointA, jointB, jointC;
	Point pointNow, pointGoal;

	static const float defaultStepDistance = 2;
	volatile float stepDistance = defaultStepDistance;

	volatile bool isBusy = false;

private:
	float xOrigin, yOrigin;
	volatile bool isFirstMove = true;
};

class Robot
{
public:
	Robot();
	void Start();

	enum State { Install, Calibrate, Boot, Action };
	State state = State::Boot;

	void InstallState();
	void CalibrateState();
	void CalibrateServos();
	void CalibrateVerify();
	void BootState();

	void MoveTo(RobotLegsPoints points);
	void MoveTo(RobotLegsPoints points, float speed);
	void MoveToRelatively(Point point);
	void MoveToRelatively(Point point, float speed);
	void WaitUntilFree();

	void SetSpeed(float speed);
	void SetSpeed(float speed1, float speed2, float speed3, float speed4);

	void GetPointsNow(RobotLegsPoints &points);

	void UpdateAction();

	RobotLeg leg1, leg2, leg3, leg4;

	RobotLegsPoints calibrateStatePoints = RobotLegsPoints(
		Point(-80, 125, 35),
		Point(-80, -125, 35),
		Point(80, 125, 35),
		Point(80, -125, 35));
	RobotLegsPoints calibratePoints = RobotLegsPoints(
		Point(-80, 125, 0),
		Point(-80, -125, 0),
		Point(80, 125, 0),
		Point(80, -125, 0));
	RobotLegsPoints bootPoints = RobotLegsPoints(
		Point(-90, 90, 0),
		Point(-90, -90, 0),
		Point(90, 90, 0),
		Point(90, -90, 0));

private:
	void CalibrateLeg(RobotLeg &leg, Point calibratePoint);

	void UpdateLegAction(RobotLeg &leg);

	void MoveToDirectly(RobotLegsPoints points);

	void SetOffsetEnableState(bool state);

	const float minDistance = 0.1;
};

class RobotAction
{
public:
	RobotAction();
	void Start();

	void ActiveMode();
	void SleepMode();
	void SwitchMode();

	void CrawlForward();
	void CrawlBackward();

	void TurnLeft();
	void TurnRight();

	void MoveBody(float x, float y, float z);
	void RotateBody(float x, float y, float z, float angle);

	void InitialState();

	Robot robot;

private:
	void ActionState();

	enum Mode { Active, Sleep };
  Mode mode = Mode::Sleep;
  
  enum LegsState { Initial, Feet12Long, Feet34Long, Move, Rotate };
	LegsState legsState = LegsState::Initial;

  const float legLift = 25;
  const float bodyLift = 15;

	const float crawlOffsetX = 15;
	const float crawlOffsetY = 15;

	const float crawlSpeedLeg = 7.5;
	const float crawlSpeedBody = crawlSpeedLeg * (crawlOffsetY / (7 * crawlOffsetY + 2 * legLift));

  void FirstStepForward();
  void FirstStepBackward();

  void LegStepTo(RobotLeg &leg, Point point, float speed);
	void LegStepToRelatively(RobotLeg &leg, Point point, float speed);
  void LegMoveToRelatively(RobotLeg &leg, Point point, float speed);
  void LegsMoveTo(RobotLegsPoints points);
	void LegsMoveTo(RobotLegsPoints points, float speed);
	void LegsMoveToRelatively(Point point, float speed);

  void GetCrawlPoints(RobotLegsPoints &points, Point point);
  void GetCrawlPoint(Point &point, Point direction);

  const float turnAngle = 22.5;
  
  const float turnOffset = 15;

	const float turnSpeedLeg = crawlSpeedLeg;
	const float turnSpeedBody = turnSpeedLeg / 6.5;

  void Turn(float degree);

  void GetTurnPoints(RobotLegsPoints &points, float angle);
	void GetTurnPoint(Point &point, float angle);

  const float speedMoveBody = 1.25;
  const float speedRotateBody = 1.25;

  void GetMovePoints(RobotLegsPoints &points, Point point);
  void GetMovePoint(Point &point, Point direction);

  void GetRotatePoints(RobotLegsPoints &points, float x, float y, float z, float angle);
  void GetRotatePoint(Point &point, float x, float y, float z, float angle);
};

#endif
