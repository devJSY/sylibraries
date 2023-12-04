#include "AStar.h"

namespace sy
{
AStar::AStar() : m_Map{}, m_Start{}, m_Goal{}, m_Row(0), m_Column(0), m_SearchDir(4)
{
}
AStar::~AStar()
{
}

int AStar::Init(string FilePath)
{
    m_Map = Fileload("..\\..\\External\\MAP\\" + FilePath);

    if (m_Map.empty())
        return -1;

    Astar();
    Print();

    return 0;
}
int AStar::Init(vector<vector<char>> &map, pair<int, int> start, pair<int, int> goal)
{
    m_Map = map;
    m_Start = start;
    m_Goal = goal;

    if (m_Map.empty())
        return -1;

    Astar();
    Print();

    return 0;
}

int AStar::Astar()
{
    priority_queue<Node, vector<Node>, cmp> open; // 우선순위 큐

    vector<vector<bool>> close(m_Row, vector<bool>(m_Column, false));
    vector<Node> close_list;
    Node s_node; // 시작 노드

    int cx[8] = {0, 1, 0, -1, 1, 1, -1, -1}; // 방향 좌표 ↑→↓←↘↗↙↖
    int cy[8] = {-1, 0, 1, 0, 1, -1, 1, -1};

    // 시작지점 초기화
    s_node.x = m_Start.second;
    s_node.y = m_Start.first;
    s_node.G = 0;
    s_node.H = (abs(m_Goal.second - s_node.x) + abs(m_Goal.first - s_node.y)) * 10; // Manhattan distance
    s_node.parent = make_pair(-1, -1); // 시작 노드의 부모 노드는 -1,-1
    open.push(s_node);
    close[s_node.y][s_node.x] = true; // 폐쇄 노드

    // 복사본으로 체크
    vector<vector<char>> result = m_Map;
    while (open.size())
    {
        int x = open.top().x; // 우선순위 큐에서 top 정보 추출
        int y = open.top().y;
        int G = open.top().G;
        close_list.push_back(open.top());
        result[y][x] = '8'; // 폐쇄 리스트에 추가
        open.pop();
        if (x == m_Goal.second && y == m_Goal.first)
            break; // 도착 지점이 나오면 끝

        Node add;
        for (int i = 0; i < m_SearchDir; i++)
        { // top 노드에서 상하좌우 4방향으로 탐색(i<8이면 8방향)
            int nextX = x + cx[i];
            int nextY = y + cy[i];
            if (nextX >= 0 && nextX < m_Column && nextY >= 0 && nextY < m_Row)
            {
                // 1: 낭떨어지 4: 테두리
                if (m_Map[nextY][nextX] != '1' && m_Map[nextY][nextX] != '4' && close[nextY][nextX] == false)
                {
                    add.x = nextX;
                    add.y = nextY;
                    add.G = i < 4 ? G + 10 : G + 14; // 상하좌우면 10, 대각선이면 14(√200)
                    add.H = (abs(m_Goal.second - add.x) + abs(m_Goal.first - add.y)) * 10;
                    add.parent = make_pair(y, x); // 기존 top노드를 부모 노드로 설정
                    close[nextY][nextX] = true;
                    result[nextY][nextX] = '9'; // 우선순위 큐에 들어감
                    open.push(add);             // 우선순위 큐에 삽입
                                                // system("cls");
                    // Print_map(result);
                }
            }
        }
    }

    // 원본 Map에 Astar 경로 체크
    int px = close_list.back().x;
    int py = close_list.back().y;
    while (close_list.size())
    { // close_list를 역추적해 경로 탐색
        if (px == close_list.back().x && py == close_list.back().y)
        { // 목표 노드부터 부모 노드를 탐색해 역추적
            m_Map[py][px] = '*';
            px = close_list.back().parent.second;
            py = close_list.back().parent.first;
        }
        close_list.pop_back();
    }

    return 0;
}

void AStar::Print()
{
    system("cls");

    for (int i = 0; i < m_Map.size(); i++)
    {
        for (int j = 0; j < m_Map[i].size(); j++)
            cout << m_Map[i][j];
        cout << endl;
    }
}

std::vector<std::vector<char>> AStar::Fileload(string FilePath)
{
    std::ifstream ifs(FilePath);

    if (ifs.is_open())
    {
        ifs >> m_Row >> m_Column;
        std::vector<std::vector<char>> result(m_Row, std::vector<char>(m_Column));

        for (int i = 0; i < m_Row; ++i)
        {
            for (int j = 0; j < m_Column; ++j)
            {
                ifs >> result[i][j];

                if (result[i][j] == '2')
                {
                    m_Start.first = i;
                    m_Start.second = j;
                }
                else if (result[i][j] == '3')
                {
                    m_Goal.first = i;
                    m_Goal.second = j;
                }
            }
        }

        return result;
    }

    return std::vector<std::vector<char>>();
}
} // namespace sy
