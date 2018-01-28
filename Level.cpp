#include "Level.h"

namespace GGJ
{
    static constexpr int level[6][12]
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
        { 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    };

    int GetLevelWidth() noexcept
    {
        return 12 * 128;
    }

    int GetLevelHeight() noexcept
    {
        return 6 * 128;
    }

    void LoadLevel(Player* outPlayer, std::vector<Block>* outMap) noexcept
    {
        for (int y = 0; y < 6; y++)
        {
            for (int x = 0; x < 12; x++)
            {
                if      (level[y][x] == 1) outMap->push_back(Block{ 64 + x * 128, -y * 128 - 64, 128, 128, 0 });
                else if (level[y][x] == 2) outPlayer->SetPosition(64 + x * 128, -y * 128 - 64);
            }
        }
    }
};
