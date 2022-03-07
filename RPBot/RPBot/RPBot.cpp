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
    Surprised, // Also curious
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

    { Tone::Surprised, { { ":o", ".o." }, { ":O", ":0", ".O." }, { "o.o" }, { "o.0", "0.o", "0.0" }, { "@-@", "@.@" } } },
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
    Article,
};
std::ostream& operator<<(std::ostream& stream, const POS& pos)
{
    switch (pos)
    {
    case POS::Noun:         return stream << "noun";
    case POS::Pronoun:      return stream << "pronoun";
    case POS::Verb_Action:  return stream << "action verb";
    case POS::Verb_Helping: return stream << "helping verb";
    case POS::Verb_Linking: return stream << "linking verb";
    case POS::Adverb:       return stream << "adverb";
    case POS::Adjective:    return stream << "adjective";
    case POS::Conjunction:  return stream << "conjunction";
    case POS::Article:      return stream << "article";
    }
}
std::istream& operator>>(std::istream& stream, POS& pos)
{
    std::string input;
    stream >> input;
    if (input == "noun") pos = POS::Noun;
    else if (input == "pronoun") pos = POS::Pronoun;
    else if (input == "verb") pos = POS::Verb_Action;
    else if (input == "action") { stream >> input; pos = POS::Verb_Action; }
    else if (input == "verb") pos = POS::Verb_Action;
    return stream;
}
struct Word
{
    std::string word;
    POS partOfSpeach;
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
    std::cout
        << "Hello! I am a bot! " << Emote(Tone::Happy, 0.5f) << '\n'
        << "I am still learning English, so I'm sorry if I have trouble understanding! " << Emote(Tone::Worried, 0.0f) << '\n'
        << "Teach me a new word? " << Emote(Tone::Happy, 0.0f) << '\n';
    g_vocab.push_back(new Word);
    std::cin >> *g_vocab.back();

    std::cout << "What part of speach (noun, pronoun, verb, etc.) is \"" << *g_vocab.back() << "\"? " << Emote(Tone::Surprised, 0.0f) << '\n';
    std::cin >> g_vocab.back()->partOfSpeach;

    for (Word* word : g_vocab)
    {
        delete word;
    }
}
