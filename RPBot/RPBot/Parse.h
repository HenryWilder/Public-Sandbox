#pragma once
#include <string>
#include <vector>
#include <map>

struct FileTone
{
	std::string tag;
};
extern std::vector<FileTone*> g_tones;

struct FilePOS
{
	std::string tag;
};
extern std::vector<FilePOS*> g_partsOfSpeach;

struct FileWord
{
	std::string text;
	std::vector<FileTone*> tones;
	std::vector<FilePOS*> partsOfSpeach;
	std::vector<FileWord*> synonyms;
};
extern std::map<std::string, FileWord*> g_words;

void Load(const char* filename);
void Save(const char* filename);
void Unload();