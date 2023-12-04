#include "BSP//BSP.h"
#include "AStar//AStar.h"

#ifdef _DEBUG
#pragma comment(lib, "BSP//BSP_d")
#pragma comment(lib, "AStar//Astar_d")
#else
#pragma comment(lib, "BSP//BSP")
#pragma comment(lib, "AStar//Astar")
#endif

int main()
{
    // sy::BSP_Init();
    // sy::Astar_Init("MAP.txt");

    //sy::BSP::GetInst()->Init();
    // sy::BSP::GetInst()->Init(64, 64);

    sy::AStar::GetInst()->Init("MAP.txt");

    return 0;
}
