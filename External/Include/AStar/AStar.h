#pragma once
#include "define.h"
#include "global.h"

namespace sy
{
using namespace std;

typedef struct node
{
    int x, y;              // 좌표
    int G, H;              // g(node), h(node)
    pair<int, int> parent; // 역추적에 쓰일 이전 노드
} Node;

struct cmp // 우선순위 큐 비교 함수
{
    bool operator()(const node u, const node v)
    {
        if (u.G + u.H > v.G + v.H)
            return true; // F는 작은게 위로 오게
        else if (u.G + u.H == v.G + v.H)
        { // F가 같다면 G가 큰게 위로 오게
            if (u.G < v.G)
                return true;
            else
                return false;
        }
        else
            return false;
    }
};

class AStar
{
    SINGLETON(AStar)

  public:
    int Init(string FilePath);
    int Init(vector<vector<char>> &map, pair<int, int> start, pair<int, int> goal);

  public:
    int Astar();
    void Print();

  private:
    std::vector<std::vector<char>> Fileload(string FilePath);

  private:
    std::vector<std::vector<char>> m_Map;
    pair<int, int> m_Start;
    pair<int, int> m_Goal;

    int m_Row;
    int m_Column;

    int m_SearchDir; // 4방향 or 8방향 
};

} // namespace sy
