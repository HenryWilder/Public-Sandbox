#include "Parse.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

std::vector<FileTone*> g_tones;
std::vector<FilePOS*> g_partsOfSpeach;
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
			g_tones.push_back(new FileTone);
			g_tones.back()->tag = data;
		}
		else if (data == "u") // part of speach
		{
			file >> data;
			g_partsOfSpeach.push_back(new FilePOS);
			g_partsOfSpeach.back()->tag = data;
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
						word->partsOfSpeach.push_back(g_partsOfSpeach[id]);
					}
				}

				else if (data == "#s")
				{
					while (true)
					{
						file >> data;
						if (data == "/s") break;
						_ASSERT_EXPR(!file.eof() && data != "/w", "File did not close word use list.");

						auto it = g_words.find(data);
						if (it != g_words.end())
							word->synonyms.push_back(it->second);
					}
				}
			}
		}
		file.ignore(64, '#');
	}

	file.close();
}

void Save(const char* filename)
{
	std::ofstream file(filename);
	file << "0.0.0\n";
	for (const FileTone* tone : g_tones) // tone
	{
		file << "#t " << tone->tag << '\n';
	}
	for (const FilePOS* pos : g_partsOfSpeach) // part of speach
	{
		file << "#u " << pos->tag << '\n';
	}
	for (const auto& [str, word] : g_words) // word
	{
		file << "#w " << str << '\n';
		if (!word->tones.empty())
		{
			file << "\t#t ";
			for (FileTone* tone : word->tones)
			{
				auto it = std::find(g_tones.begin(), g_tones.end(), tone);
				if (it != g_tones.end())
					file << std::distance(g_tones.begin(), it) << ' ';
			}
			file << "/t\n";
		}
		if (!word->partsOfSpeach.empty())
		{
			file << "\t#u ";
			for (FilePOS* pos : word->partsOfSpeach)
			{
				auto it = std::find(g_partsOfSpeach.begin(), g_partsOfSpeach.end(), pos);
				if (it != g_partsOfSpeach.end())
					file << std::distance(g_partsOfSpeach.begin(), it) << ' ';
			}
			file << "/u\n";
		}
		if (!word->synonyms.empty())
		{
			file << "\t#s ";
			for (FileWord* syn : word->synonyms)
			{
				file << syn->text << ' ';
			}
			file << "/s\n";
		}
		file << "/w\n";
	}
}

void Unload()
{
	for (const FileTone* tone : g_tones)
	{
		delete tone;
	}
	for (const FilePOS* pos : g_partsOfSpeach)
	{
		delete pos;
	}
	for (const auto&[str, word] : g_words)
	{
		delete word;
	}
}
