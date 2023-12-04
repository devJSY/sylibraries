#include "func.h"

namespace sy
{
int GetRandomInt(int min, int max)
{
    if (max < 0)
        return 0;

    // STL Random
    std::random_device rndDevice;
    std::mt19937 eng(rndDevice());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(eng);
}
} // namespace sy