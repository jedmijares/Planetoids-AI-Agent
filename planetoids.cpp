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

	// finds the closest point in a parallel universe
	Vector2 closestParallelPoint(const Vector2 other);

	// returns orientation (degrees from axis) to ideally point this to goal
	double orientToGoal(const Vector2 goal);
};

int main()
{
	// Unbuffered I/O
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	// std::ofstream inputLogFile;
	// inputLogFile.open("F:\\code\\IcpcContestantAll\\ICPC\\cpp\\Debug\\input.jsonl");

	const std::bitset<6> thrust = 0b100000;
	const std::bitset<6> clockwise = 0b010000;
	const std::bitset<6> counterclockwise = 0b001000;
	const std::bitset<6> bullet = 0b000100;
	const std::bitset<6> hyperspace = 0b000010;

	Vector2 lastPos{ 0, 0 };

	while (true)
	{
		std::bitset<6> controller = 0b000001;

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

		// inputLogFile << Input << std::endl;

		nlohmann::json Json = nlohmann::json::parse(Input);

		// Exit on game over.
		if (Json["gameOver"] == true)
		{
			break;
		}

		controller ^= bullet;

		Vector2 shipPos{ Json["shipPos"][0], Json["shipPos"][1] };
		Vector2 artPos{ Json["artfPos"][0], Json["artfPos"][1] };

		Vector2 shipVel{ shipPos.x - lastPos.x, shipPos.y - lastPos.y };

		double shipR = Json["shipR"];

		Vector2 goal = shipPos.closestParallelPoint(artPos);

		/*double deviation = 20.0;
		if (shipR <= 180.0 - deviation)
		{
			controller ^= counterclockwise;
		}
		else if (shipR >= 180.0 + deviation)
		{
			controller ^= clockwise;
		}*/
		if (false) {}
		else 
		{
			double rotDif = shipPos.orientToGoal(goal) - shipR;
			if (rotDif < 0)
			{
				rotDif += 360;
			}
			if (rotDif < 180.0)
			{
				controller ^= counterclockwise;
			}
			else
			{
				controller ^= clockwise;
			}
		}

		double mag = shipVel.magnitudeSquared();
		double goalDist = shipPos.distanceSquared(goal);

		if (shipVel.magnitudeSquared() > 300 && shipPos.distanceSquared(goal) < 700000)
		{

		}
		else
		{
			controller ^= thrust;
		}

		lastPos = shipPos;

		// Emit command.
		std::cout << controller << std::endl;
		fflush(stdout);
	}

	// inputLogFile.close();

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
