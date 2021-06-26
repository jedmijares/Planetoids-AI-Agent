#include <iostream>
#include <stdio.h>
#include <fstream>
#include <bitset>
#include <cmath>
#include <math.h>
#include <vector>

#include "json.hpp"

const unsigned int SCREEN_WIDTH = 7600;
const unsigned int SCREEN_HEIGHT = 4200;

constexpr auto PI = 3.14159265;

class Vector2 {
public:
	double x;
	double y;

	Vector2(double xIn, double yIn);

	double distanceSquared(const Vector2 other);
	double magnitudeSquared();

	// finds the closest point in any "parallel universe"
	Vector2 closestParallelPoint(const Vector2 other);

	// returns orientation (degrees from axis) to ideally point this to goal
	double orientToGoal(const Vector2 goal);
};

int main()
{
	// Unbuffered I/O
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	// controls
	const std::bitset<6> thrust = 0b100000;
	const std::bitset<6> clockwise = 0b010000;
	const std::bitset<6> counterclockwise = 0b001000;
	const std::bitset<6> bullet = 0b000100;
	const std::bitset<6> hyperspace = 0b000010;

	Vector2 lastPos{ 0, 0 };

	while (true)
	{

		// Read simulation frame.
		std::string Input;
		std::getline(std::cin, Input);

		// Handle end of stream.
		if (feof(stdin) || std::cin.eof())
		{
			break;
		}
		// Skip empty lines.
		else if (Input.length() == 0)
		{
			continue;
		}

		nlohmann::json Json = nlohmann::json::parse(Input);

		// Exit on game over.
		if (Json["gameOver"] == true)
		{
			break;
		}

		std::bitset<6> controller = 0b000001;
		controller ^= bullet;

		Vector2 shipPos{ Json["shipPos"][0], Json["shipPos"][1] };
		Vector2 artPos{ Json["artfPos"][0], Json["artfPos"][1] };

		Vector2 shipVel{ lastPos.x - shipPos.x, lastPos.y - shipPos.y };

		double shipRot = Json["shipR"];

		Vector2 goal = shipPos.closestParallelPoint(artPos);

		// find difference between current orientation and direction we want to go
		double rotDif = shipPos.orientToGoal(goal) - shipRot;
		if (rotDif < 0)
		{
			rotDif += 360;
		}

		// choose which direction to turn
		if (rotDif < 180.0)
		{
			controller ^= counterclockwise;
		}
		else
		{
			controller ^= clockwise;
		}
		
		// find direction of current velocity
		double shipVelDir = atan(shipVel.y / shipVel.x) * 180 / PI;
		if (shipRot > 90 && shipRot < 270)
		{
			shipVelDir += 180;
		}
		if (shipVelDir < 0)
		{
			shipVelDir += 360;
		}
		else if (shipVelDir > 360)
		{
			shipVelDir -= 360;
		}

		double shipToGoalDir = shipPos.orientToGoal(goal);
		double goalDist = shipPos.distanceSquared(goal);
		double velMag = shipVel.magnitudeSquared();
		double shipDirGoalDif = abs(shipVelDir - shipToGoalDir);

		// do not thrust if it might make us miss the goal
		if (goalDist < 100)
		{
			controller ^= thrust;
		}
		else if ((velMag > 500 && goalDist < 8000 && shipDirGoalDif > 50.0) || (velMag > 200 && goalDist < 800000 && shipDirGoalDif > 35.0) || (shipDirGoalDif > 50.0 && velMag > 100))
		{
		}
		else
		{
			controller ^= thrust;
		}

		// warp if far from the goal and not pointing there
		if (goalDist > 12000000 && abs(rotDif) > 100.0)
		{
			controller ^= hyperspace;
		}

		lastPos = shipPos;

		// Emit command.
		std::cout << controller << std::endl;
		fflush(stdout);
	}
	return 0;
}

Vector2::Vector2(double xIn, double yIn)
{
	x = xIn;
	y = yIn;
}

double Vector2::distanceSquared(const Vector2 other)
{
	return (other.x - this->x) * (other.x - this->x) + (other.y - this->y) * (other.y - this->y);
}

double Vector2::magnitudeSquared()
{
	return this->x * this->x + this->y * this->y;
}

Vector2 Vector2::closestParallelPoint(const Vector2 other)
{
	std::vector<Vector2> parallelPoints;
	parallelPoints.emplace_back(Vector2{ other.x + SCREEN_WIDTH, other.y });
	parallelPoints.emplace_back(Vector2{ other.x - SCREEN_WIDTH, other.y });
	parallelPoints.emplace_back(Vector2{ other.x, other.y + SCREEN_HEIGHT});
	parallelPoints.emplace_back(Vector2{ other.x, other.y - SCREEN_HEIGHT });
	parallelPoints.emplace_back(Vector2{ other.x + SCREEN_WIDTH, other.y + SCREEN_HEIGHT });
	parallelPoints.emplace_back(Vector2{ other.x + SCREEN_WIDTH, other.y - SCREEN_HEIGHT });
	parallelPoints.emplace_back(Vector2{ other.x - SCREEN_WIDTH, other.y + SCREEN_HEIGHT });
	parallelPoints.emplace_back(Vector2{ other.x - SCREEN_WIDTH, other.y - SCREEN_HEIGHT });

	double minDistance = this->distanceSquared(other);
	Vector2 returnVal = other;
	for (auto point : parallelPoints)
	{
		if (this->distanceSquared(point) < minDistance)
		{
			returnVal = point;
			minDistance = this->distanceSquared(point);
		}
	}
	return returnVal;
}

double Vector2::orientToGoal(const Vector2 goal)
{
	double returnVal = atan2(goal.y - this->y, goal.x - this->x) * 180 / PI;
	if (returnVal < 0)
	{
		returnVal += 360;
	}
	return returnVal;
}
