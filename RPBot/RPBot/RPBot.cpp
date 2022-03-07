#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <string>

enum class Tone
{
    Blank,
    Happy,
    Silly,
    Intrigued,
    Flirty,
    Mischief,
    Surprised,
    Sad,
    Mad, // Also disappointed
    Worried, // Also apologetic
    Scared,
};
// In order of intensity, grouped by interchangeable
std::map<Tone, std::vector<std::vector<std::string>>> g_emotes = 
{
    { Tone::Blank, { { ".c.", ".3." }, { ".-.", "._.", ":|", ":/", ":T", ":P" } } },

    { Tone::Happy, { { ":>", ":3" }, { "^^", "^v^", "^u^", "^w^" } } },
    { Tone::Silly, { { "x3", "x>" }, { "xD" } } },

    { Tone::Intrigued, { { ".c." }, { "o3o", "owo" }, { "O3o", "o3O", "owO", "Owo" }, { "ow0", "0wo", "uwU", "Uwu" }, { "O3O", "OwO", "0w0" }, { "UwU" } } },
    { Tone::Flirty, { { "uwo", "owu", ";3", ";>" }, { ">;3", ">;>" }, { "UwU" } } },
    { Tone::Mischief, { { "uwu" }, { ">:3", ">:)" }, { "UWU", "OWO" } } },

    { Tone::Surprised, { { ":o" }, { ":O", ":0" }, { "o.o" }, { "o.0", "0.o", "0.0" }, { "@-@", "@.@" } } },
    { Tone::Sad, { { ":<", ":c" }, { ":'c" }, { ";w;" }, { "qwq" } } },
    { Tone::Mad, { { "-3-" }, { "-3-\"", "-3-;" }, { "-.-", "-_-", "-~-" }, { "-.-\"", "-.-;", "-~-\"", "-~-;" }, { ">:T", ">:|", ">:/" }, { ">:<", ">:c" }, { ">:C" }, { ">:'c" }, { ">:'C" } } },

    { Tone::Worried, { { "^^\"", "^^;", "^w^\"", "^w^;" }, { ">.<" }, { ">~<" }, { ">~>" } } },
    { Tone::Scared, { { "o~o", "o.o" }, { "o~o\"", "o.o\"", "o~o;", "o.o;" }, { "O~O", "O.O" }, { "O~O\"", "O.O\"", "O~O;", "O.O;" }, { "0~0" }, { "0~0\"", "0~0;" } } },
};
const std::string& Emote(Tone tone, float intensity)
{
    if (intensity < 0) intensity = 0;
    else if (intensity > 1) intensity = 1;
    const auto& emotesForTone = g_emotes.find(tone)->second;
    long group = lroundf(intensity * (float)(emotesForTone.size() - 1));
    const auto& options = emotesForTone[group];
    return options[rand() % options.size()];
}

// Part of speach
enum class POS
{
    Noun,
    Pronoun,
    Verb_Action,
    Verb_Helping,
    Verb_Linking,
    Adverb,
    Adjective,
    Conjunction,
};
struct Word
{
    std::string word;

    std::map<Word*, float> synonyms;
};
std::ostream& operator<<(std::ostream& stream, const Word& word)
{
    return stream << word.word;
}
std::istream& operator>>(std::istream& stream, Word& word)
{
    return stream >> word.word;
}

std::vector<Word*> g_vocab =
{

};

int main()
{
    std::cout << "Hello! I am a bot! I am still learning English, so I'm sorry if I have trouble understanding! " << Emote(Tone::Worried, 0.0f) << "\n";
    std::cout << "Teach me a new word?\n";
    g_vocab.push_back(new Word);
    std::cin >> *g_vocab.back();

    std::cout << "What part of speach (noun, pronoun, verb, etc.) is \"" << *g_vocab.back() << "\"?\n";

    for (Word* word : g_vocab)
    {
        delete word;
    }
}
