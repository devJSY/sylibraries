#include "..\\C++\CommonInclude.h"

// 참고링크 
// https://blog.naver.com/PostView.naver?blogId=oyh951416&logNo=222045386773&parentCategoryNo=&categoryNo=8&viewDate=&isShowPopularPosts=false&from=search
// https://velog.io/@dpmawile/astar

namespace sy
{
	using namespace std;

	int ROW = 0, COL = 0;
	pair<int, int> start, goal;

	typedef struct node 
	{
		int x, y;	//좌표
		int G, H;	//g(node), h(node)
		pair<int, int> parent; //역추적에 쓰일 이전 노드
	}Node;

	struct cmp //우선순위 큐 비교 함수
	{
		bool operator()(const node u, const node v) 
		{
			if (u.G + u.H > v.G + v.H) return true;//F는 작은게 위로 오게
			else if (u.G + u.H == v.G + v.H) {//F가 같다면 G가 큰게 위로 오게
				if (u.G < v.G) return true;
				else return false;
			}
			else return false;
		}
	};
	
	void Print_map(const vector<vector<char>>& map) 
	{ 
		for (int i = 0; i < map.size(); i++) {
			for (int j = 0; j < map[i].size(); j++) 
				cout << map[i][j] << " ";
			cout << '\n';
		}
	}

	int Astar(vector<vector<char>> map, pair<int, int> start, pair<int, int> goal)
	{
		priority_queue<Node, vector<Node>, cmp> open;	// 우선순위 큐

		vector<vector<bool>> close(ROW, vector<bool>(COL, false));
		vector<Node> close_list;
		Node s_node;									// 시작 노드

		int cx[8] = { 0,1,0,-1,1,1,-1,-1 };				// 방향 좌표 ↑→↓←↘↗↙↖
		int cy[8] = { -1,0,1,0,1,-1,1,-1 };

		// 시작지점 초기화
		s_node.x = start.second;
		s_node.y = start.first;
		s_node.G = 0;
		s_node.H = (abs(goal.second - s_node.x) + abs(goal.first - s_node.y)) * 10; // Manhattan distance
		s_node.parent = make_pair(-1, -1);				// 시작 노드의 부모 노드는 -1,-1
		open.push(s_node);
		close[s_node.y][s_node.x] = true;				// 폐쇄 노드

		vector<vector<char>> result = map;
		while (open.size()) 
		{
			int x = open.top().x;						// 우선순위 큐에서 top 정보 추출
			int y = open.top().y;
			int G = open.top().G;
			close_list.push_back(open.top());
			result[y][x] = '8';	// 폐쇄 리스트에 추가
			open.pop();
			if (x == goal.second && y == goal.first) break;	// 도착 지점이 나오면 끝

			Node add;
			for (int i = 0; i < 8; i++) {// top 노드에서 상하좌우 4방향으로 탐색(i<8이면 8방향)
				int nextX = x + cx[i];
				int nextY = y + cy[i];
				if (nextX >= 0 && nextX < COL && nextY >= 0 && nextY < ROW) {
					if (map[nextY][nextX] != '1' && close[nextY][nextX] == false) {
						add.x = nextX;
						add.y = nextY;
						add.G = i < 4 ? G + 10 : G + 14;// 상하좌우면 10, 대각선이면 14(√200)
						add.H = (abs(goal.second - add.x) + abs(goal.first - add.y)) * 10;
						add.parent = make_pair(y, x);	// 기존 top노드를 부모 노드로 설정
						close[nextY][nextX] = true;
						result[nextY][nextX] = '9';		// 우선순위 큐에 들어감
						open.push(add);					// 우선순위 큐에 삽입	
						system("cls");
						Print_map(result);
					}
				}
			}

		}


		int px = close_list.back().x;
		int py = close_list.back().y;
		while (close_list.size()) {						// close_list를 역추적해 경로 탐색
			if (px == close_list.back().x && py == close_list.back().y) {// 목표 노드부터 부모 노드를 탐색해 역추적
				result[py][px] = '*';
				px = close_list.back().parent.second;
				py = close_list.back().parent.first;
				system("cls");
				Print_map(result);
			}
			close_list.pop_back();
		}


		return 0;
	}

	std::vector<std::vector<char>> fileload(std::string filepath) {
		std::ifstream ifs(filepath);

		if (ifs.is_open()) {
			ifs >> ROW >> COL;
			std::vector<std::vector<char>> result(ROW, std::vector<char>(COL));

			for (int i = 0; i < ROW; ++i) {
				for (int j = 0; j < COL; ++j) {
					ifs >> result[i][j];

					if (result[i][j] == '2')
					{
						start.first = i;
						start.second = j;
					}
					else if (result[i][j] == '3')
					{
						goal.first = i;
						goal.second = j;
					}
				}
			}

			return result;
		}

		return std::vector<std::vector<char>>();
	}

	int Astar_Init(std::string FileName) {

		// *: 최종 Astar 결과 경로 
		// 0: 길, 1: 벽 
		// 2: 출발점, 3: 도착점
		// 8 : 폐쇄 리스트에 들어간 노드, 9 : 우선순위 큐에 들어간 노드

		std::vector<std::vector<char>> map = fileload("..\\Astar\\" + FileName);
		if (map.empty()) return -1;

		// 입력받은 맵 출력
		Print_map(map);

		Astar(map, start, goal);

		return 0;
	}
}