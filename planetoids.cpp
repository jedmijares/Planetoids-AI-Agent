#include <iostream>
#include <stdio.h>
#include <fstream>

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

		// Emit command.
		printf("110101\n");
		fflush(stdout);
	}

	inputLogFile.close();

	return 0;
}

