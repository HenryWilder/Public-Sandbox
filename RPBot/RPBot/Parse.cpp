#include "Parse.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

std::vector<CustomTone*> g_tones;
std::vector<Use*> g_uses;
std::map<std::string, FileWord*> g_words;


void Load(const char* filename)
{
	std::ifstream file(filename);

	file.ignore(64, '#');
	while (!file.eof())
	{
		std::string data;
		file >> data;
		if (data == "t") // tone
		{
			file >> data;
			g_tones.push_back(new CustomTone);
			g_tones.back()->tag = data;
		}
		else if (data == "u") // use
		{
			file >> data;
			g_uses.push_back(new Use);
			g_uses.back()->tag = data;
		}
		else if (data == "w") // word
		{
			file >> data;
			FileWord* word = new FileWord;
			g_words.insert({ data, word });
			word->text = data;
			while (true)
			{
				file >> data;
				if (data == "/w") break;
				_ASSERT_EXPR(!file.eof(), "File did not close word definition.");

				if (data == "#t")
				{
					while (true)
					{
						file >> data;
						if (data == "/t") break;
						_ASSERT_EXPR(!file.eof() && data != "/w", "File did not close word tone list.");

						int id = stoi(data);
						word->tones.push_back(g_tones[id]);
					}
				}

				else if (data == "#u")
				{
					while (true)
					{
						file >> data;
						if (data == "/u") break;
						_ASSERT_EXPR(!file.eof() && data != "/w", "File did not close word use list.");

						int id = stoi(data);
						word->uses.push_back(g_uses[id]);
					}
				}
			}
		}
		file.ignore(64, '#');
	}

	file.close();
}