#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <unordered_set>
#include <stack>
#include <time.h> // For choosing a random seed

#define RAND_ELEMENT(arr) arr[rand() % arr.size()]

int RandomInRange(int min, int max)
{
    int num = rand();
    num = num % (max - min);
    num += min;
    return num;
}

// Bigger = less likely
bool Spicy(int options)
{
    return !(rand() % options);
}
// Bigger = more likely
bool Unusual(int odds)
{
    return !!(rand() % odds);
}

const char* g_phonemes[] = {
    "a", "e", "i", "o", "u",
    "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "y", "z",
    "ck", "ch", "ng", "sh", "ph",
    "sch",
    "\'",
};

enum PhonemeID {
    ID_A, ID_E, ID_I, ID_O, ID_U,
    ID_B, ID_C, ID_D, ID_F, ID_G, ID_H, ID_J, ID_K, ID_L, ID_M, ID_N, ID_P, ID_Q, ID_R, ID_S, ID_T, ID_V, ID_W, ID_X, ID_Y, ID_Z,
    ID_CK, ID_CH, ID_NG, ID_SH, ID_PH,
    ID_SCH,
    ID_Apost,
};

std::vector<PhonemeID> g_allIDs = {
    ID_A, ID_E, ID_I, ID_O, ID_U,
    ID_B, ID_C, ID_D, ID_F, ID_G, ID_H, ID_J, ID_K, ID_L, ID_M, ID_N, ID_P, ID_Q, ID_R, ID_S, ID_T, ID_V, ID_W, ID_X, ID_Y, ID_Z,
    ID_CK, ID_CH, ID_NG, ID_SH, ID_PH,
    ID_SCH,
    ID_Apost,
};

std::vector<PhonemeID> g_uncommonIDs = {
    ID_J, ID_Q, ID_X, ID_Z,
};

std::vector<PhonemeID> g_notUncommonIDs = {
    ID_A, ID_E, ID_I, ID_O, ID_U,
    ID_B, ID_C, ID_D, ID_F, ID_G, ID_H, ID_K, ID_L, ID_M, ID_N, ID_P, ID_R, ID_S, ID_T, ID_V, ID_W, ID_Y,
    ID_CK, ID_CH, ID_NG, ID_SH, ID_PH,
    ID_SCH,
};

std::vector<PhonemeID> g_commonIDs = {
    ID_E, ID_T, ID_A, ID_I, ID_O, ID_N, ID_S, ID_H, ID_R,
};
std::vector<PhonemeID> g_commonStartingIDs = {
    ID_T, ID_A, ID_O, ID_D, ID_W,
};
std::vector<PhonemeID> g_commonEndingIDs = {
    ID_E, ID_S, ID_D, ID_T,
};

std::vector<PhonemeID> g_vowelIDs = {
    ID_A, ID_E, ID_I, ID_O, ID_U,
};
std::vector<PhonemeID> g_basicConsonantIDs = {
    ID_B, ID_C, ID_D, ID_F, ID_G, ID_H, ID_J, ID_K, ID_L, ID_M, ID_N, ID_P, ID_Q, ID_R, ID_S, ID_T, ID_V, ID_W, ID_X, ID_Y, ID_Z,
};
std::vector<PhonemeID> g_extendedConsonantIDs = {
    ID_CK, ID_CH, ID_NG, ID_SH, ID_PH,
    ID_SCH,
};

template<typename T>
bool Contains(const std::vector<T>& list, T value)
{
    for (T element : list)
    {
        if (value == element)
            return true;
    }
    return false;
}

template<typename T>
void Append(std::vector<T>& a, const std::vector<T>& b)
{
    a.reserve(a.size() + b.size());
    for (T elem : b)
    {
        a.push_back(elem);
    }
}

template<typename T>
void Remove(std::vector<T>& vec, T remove)
{
    std::stack<size_t> eliminate;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == remove)
            eliminate.push(i);
    }
    while (!eliminate.empty())
    {
        vec.erase(vec.begin() + eliminate.top());
        eliminate.pop();
    }
}
template<typename T>
void Remove(std::vector<T>& vec, const std::vector<T>& remove)
{
    std::stack<size_t> eliminate;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (Contains(remove, vec[i]))
            eliminate.push(i);
    }
    while (!eliminate.empty())
    {
        vec.erase(vec.begin() + eliminate.top());
        eliminate.pop();
    }
}

struct Word
{
    Word()
    {
        type = PartOfSpeech::noun;
        emph = Emphasis::pathetic;
        emote = Emotion::neutral_weak;
        form = Formality::casual;

        size_t size = RandomInRange(1,10);

        std::vector<PhonemeID> phonemes;

        phonemes.reserve(size);

        for (size_t i = 0; i < size; ++i)
        {
            phonemes.push_back((PhonemeID)rand());
        }

        for (size_t i = 0; i < size; ++i)
        {
            // Q is always followed by U
            if (i >= 1 && phonemes[i - 1] == ID_Q)
            {
                phonemes[i] = ID_U;
                continue;
            }

            std::vector<PhonemeID> options;

            // One-letter word
            if (size == 1)
            {
                options = { ID_A, ID_I };
            }
            // Start of word
            else if (i == 0)
            {
                if (Spicy(20))
                    options = g_uncommonIDs;
                else if (Spicy(10))
                    options = g_notUncommonIDs;
                else
                    options = g_commonStartingIDs;

                if (Spicy(8))
                    Append(options, { ID_CH, ID_SH, ID_PH, ID_SCH });
            }
            // End of word
            else if (i == (size - 1))
            {
                if (Spicy(20))
                    options = g_uncommonIDs;
                else if (Spicy(10))
                    options = g_notUncommonIDs;
                else
                    options = g_commonEndingIDs;

                // Extended consonants more likely at the end of a word
                if (Spicy(4))
                    Append(options, { ID_CK, ID_CH, ID_NG, ID_SH });
            }
            // Mid-word
            else
            {
                // Following a vowel
                if (Contains(g_vowelIDs, phonemes[i - 1]))
                {
                    if (Spicy(4))
                        options = g_vowelIDs;

                    Append(options, g_basicConsonantIDs);
                    Append(options, g_extendedConsonantIDs);
                }
                // Following a consonant
                else
                {
                    options = g_vowelIDs;

                    if (Spicy(10))
                        Append(options, g_basicConsonantIDs);

                    if (Spicy(20))
                        Append(options, g_extendedConsonantIDs);

                    Remove(options, g_uncommonIDs);

                    if (Spicy(3))
                        options.push_back(ID_Apost);
                }
            }

            if (i >= 2)
            {
                // Following two vowels
                if (Contains(g_vowelIDs, phonemes[i - 1]) && Contains(g_vowelIDs, phonemes[i - 2]))
                {
                    Remove(options, g_vowelIDs);
                }
                // Following two consonants
                else if (Contains(g_basicConsonantIDs, phonemes[i - 1]) && Contains(g_basicConsonantIDs, phonemes[i - 2]))
                {
                    Remove(options, g_basicConsonantIDs);
                }
            }

            if (i > 0)
            {
                // Remove letters that should not be twice in a row
                switch (phonemes[i - 1])
                {
                default: break;
                case ID_A: Remove(options, { ID_A }); break;
                case ID_B: Remove(options, ID_B); Remove(options, g_extendedConsonantIDs); break;
                case ID_D: Remove(options, ID_D); Remove(options, g_extendedConsonantIDs); break;
                case ID_H: Remove(options, ID_H); Remove(options, g_extendedConsonantIDs); break;
                case ID_I: Remove(options, ID_I); break;
                case ID_J: Remove(options, ID_J); Remove(options, g_extendedConsonantIDs); break;
                case ID_K: Remove(options, ID_K); Remove(options, g_extendedConsonantIDs); break;
                case ID_M: Remove(options, { ID_SH, ID_SCH }); break;
                case ID_N: Remove(options, { ID_SH }); break;
                case ID_P: Remove(options, { ID_CH, ID_CK, ID_K }); break;
                case ID_S: Remove(options, { ID_SH, ID_SCH }); break;
                case ID_U: Remove(options, { ID_U }); break;
                case ID_V: Remove(options, ID_V); Remove(options, g_extendedConsonantIDs); break;
                case ID_W: Remove(options, ID_W); break;
                case ID_Y: Remove(options, ID_Y); break;
                case ID_Apost: Remove(options, ID_Apost); break;
                }
            }

            if (options.empty())
                options = g_commonIDs;

            phonemes[i] = options[rand() % options.size()];
        }
        for (size_t i = 0; i < size; ++i)
        {
            text += g_phonemes[phonemes[i]];
        }
    }

    enum class PartOfSpeech : char {
        noun,
        verb,
        adjective,
    } type;

    enum class Emphasis : char {
        pathetic,
        weak,
        neutral,
        strong,
        violent,
    } emph;

    enum class Emotion : char {
        // Has no emotion ("the", "and", "there"; words that do not affect or represent contextual emotion)
        neutral_weak = 0,

        // Anger, sadness, etc.
        negative_self,
        negative_general,
        negative_target,

        // Bitterness, apethy, etc.
        cold_self,
        cold_general,
        cold_target,

        // Has emotion, that emotion being neutral (Melancholy, meaninglessness, tired, etc.)
        neutral_strong,

        // Happiness, satisfaction, etc.
        positive_self,
        positive_general,
        positive_target,
    } emote;

    enum class Formality : char {
        slang,
        friendly,
        casual,
        respectful,
        formal,
        cordial,
    } form;

    std::string text;
};

std::ostream& operator<<(std::ostream& stream, const Word& word)
{
    return stream << word.text;
}

int main()
{
    srand(clock());
    for (size_t i = 0; i < 100; ++i)
    {
        std::cout << Word() << ' ';
    }
    std::cout << ".\n";
    return 0;
}
