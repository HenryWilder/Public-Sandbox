#pragma once
#include <string>
#include <vector>
#include <map>

struct CustomTone
{
	std::string tag;
};
extern std::vector<CustomTone*> g_tones;

struct Use
{
	std::string tag;
};
extern std::vector<Use*> g_uses;

struct FileWord
{
	std::string text;
	std::vector<CustomTone*> tones;
	std::vector<Use*> uses;
	std::vector<FileWord*> synonyms;
};
extern std::map<std::string, FileWord*> g_words;

void Load(const char* filename);