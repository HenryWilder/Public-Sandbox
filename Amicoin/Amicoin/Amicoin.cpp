// DISCLAIMER: joke. This code should compile, but it's not an actual blockchain.

#include <iostream>
#include <unordered_map>
#include <stack>
#include <string>
#include <fstream>

class Amicoin // See? It's a class! That means it's secure!
{
    std::unordered_map<std::string, float> m_wallets;
    float m_value = 1.0f;
public:
    void AddWallet(const std::string& name)
    {
        m_wallets.insert({ name, 0.0f });
    }
    float ReadWallet(const std::string& name)
    {
        return m_wallets.find(name)->second;
    }

    void Mine(const std::string& walletToReward, float units = 1.0f)
    {
        float* solution;
#pragma GCC push_options
#pragma GCC optimize ("O0") // Gotta make sure they have proof of work!
        for (float i = 0; i <= units; i += 1.0f / m_value)
        {
            solution = new float((float)rand() / (float)rand());
            delete solution;
        }
#pragma GCC pop_options
        m_wallets.find(walletToReward)->second += units;
    }
};

int main()
{
    Amicoin pointOfFailure;
    pointOfFailure.AddWallet("Amy Smile");
    pointOfFailure.Mine("Amy Smile", 30.0f);
    std::cout << pointOfFailure.ReadWallet("Amy Smile");
}