#pragma once
#include "define.h"
#include "global.h"

namespace sy
{
using namespace std;

typedef struct node
{
    int x, y;              // ��ǥ
    int G, H;              // g(node), h(node)
    pair<int, int> parent; // �������� ���� ���� ���
} Node;

struct cmp // �켱���� ť �� �Լ�
{
    bool operator()(const node u, const node v)
    {
        if (u.G + u.H > v.G + v.H)
            return true; // F�� ������ ���� ����
        else if (u.G + u.H == v.G + v.H)
        { // F�� ���ٸ� G�� ū�� ���� ����
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

    int m_SearchDir; // 4���� or 8���� 
};

} // namespace sy
