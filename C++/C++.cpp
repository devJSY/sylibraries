#include "..\\BSP\\\BSP.h"
#include "..\\AStar\\\AStar.h"

int main()
{
    //sy::BSP_Init();
    //sy::Astar_Init("MAP.txt");

    sy::BSP::GetInst()->Init();
    //sy::BSP::GetInst()->Init(64, 64);

    //sy::AStar::GetInst()->Init("MAP.txt");

    return 0;
}

