#include <iostream>
#include <stdio.h>
#include <fstream>
#include <bitset>

#include "json.hpp"

int main()
{
	// Unbuffered I/O
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	std::ofstream inputLogFile;
	inputLogFile.open("F:\\code\\IcpcContestantAll\\ICPC\\cpp\\Debug\\input.jsonl");

	while (true)
	{
		std::bitset<6> controller = 0b000001;
		std::bitset<6> thrust = 0b100000;
		std::bitset<6> clockwise = 0b010000;
		std::bitset<6> counterclockwise = 0b001000;
		std::bitset<6> bullet = 0b000100;
		std::bitset<6> hyperspace = 0b000010;

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

		inputLogFile << Input << std::endl;

		nlohmann::json Json = nlohmann::json::parse(Input);

		// Exit on game over.
		if (Json["gameOver"] == true)
		{
			break;
		}

		/// @TODO: Process input frame
		controller ^= bullet;
		controller ^= thrust;
		controller ^= clockwise;

		// Emit command.
		std::cout << controller << std::endl;
		fflush(stdout);
	}

	inputLogFile.close();

	return 0;
}

