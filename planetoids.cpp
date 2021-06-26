#include <iostream>
#include <stdio.h>
#include <fstream>
#include <bitset>
#include <cmath>
#include <math.h>

#include "json.hpp"

const unsigned int SCREEN_WIDTH = 7600;
const unsigned int SCREEN_HEIGHT = 4200;

constexpr auto PI = 3.14159265;

class Vector2 {
public:
	double x;
	double y;

	double distanceSquared(const Vector2 other);

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

		/// @TODO: Process input frame
		controller ^= bullet;
		controller ^= thrust;

		Vector2 shipPos;
		shipPos.x = Json["shipPos"][0];
		shipPos.y = Json["shipPos"][1];

		Vector2 artPos;
		artPos.x = Json["artfPos"][0];
		artPos.y = Json["artfPos"][1];

		double shipR = Json["shipR"];

		if (shipPos.x < artPos.x)
		{
			shipPos.x += SCREEN_WIDTH;
		}

		double deviation = 20.0;
		if (shipR <= 180.0 - deviation)
		{
			controller ^= counterclockwise;
		}
		else if (shipR >= 180.0 + deviation)
		{
			controller ^= clockwise;
		}
		else 
		{
			if (shipPos.orientToGoal(artPos) < 180.0)
			{
				controller ^= counterclockwise;
			}
			else
			{
				controller ^= clockwise;
			}
		}

		// Emit command.
		std::cout << controller << std::endl;
		fflush(stdout);
	}

	// inputLogFile.close();

	return 0;
}

double Vector2::distanceSquared(const Vector2 other)
{
	return (other.x - this->x) * (other.x - this->x) + (other.y - this->y) * (other.y - this->y);
}

double Vector2::orientToGoal(const Vector2 goal)
{
	double slope = (this->y - goal.y) / (this->x - goal.x);
	double returnVal = atan(slope) * 180 / PI;
	if (returnVal < 0)
	{
		returnVal += 360;
	}
	return returnVal;
}
