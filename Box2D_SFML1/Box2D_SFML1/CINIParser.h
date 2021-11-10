#pragma once

#include "NumptyBehavior.h"

class CINIParser : NumptyBehavior
{
	inline static void ReadFile(std::string _path)
	{
		std::ifstream file;
		std::string currentLine;

		file.open(_path);
		if (file.is_open())
		{
			while (std::getline(file, currentLine))
			{
				// Settings Header
				if (currentLine.find("[Settings]"))
				{
					
				}
				else
				{
					std::size_t pos = currentLine.find('=');
					std::string value = currentLine.substr(pos + 1);

					int FirstInventorySlot = std::stoi(value);
					std::cout << FirstInventorySlot << std::endl;
				}
			}
			file.close();
		}
	}
};

