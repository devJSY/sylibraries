#include "..\\C++\CommonInclude.h"

// 보완할점
// 1. 가로,세로 크기 최소크기 지정해서 막대기같이 생긴방 안나오게 변경
// 2. 버그 수정
// -  GetRandomInt(0, parentRoom.width - 6) 시 width값이 6보다 작을때 에러남
// 3. 지정된 크기에서 방을 생성할때 2값이 아닌 랜덤값으로 바꿔 방을 다이나믹하게 생성되도록 변경하기
// 4. 맵 연결 중 일자로 연결되는경우 3343 이렇게 나오는거 보완하기
// 5 클래스화 해보기 - static class

// 참고 링크
// https://www.youtube.com/watch?v=Yo0zYwPs2Ok&t=460s
// https://m.blog.naver.com/PostView.naver?blogId=jh20s&logNo=222343029141&proxyReferer=

namespace sy
{
#define RoomNum 0
#define WallNum 1
#define BorderNum 4
#define RowNum 5
#define ColNum 6

#define MaxWidth 128
#define MaxHeight 128

struct Rect
{
    Rect(int _x, int _y, int _w, int _h) : x(_x), y(_y), width(_w), height(_h)
    {
    }

    int x;
    int y;
    int width;
    int height;
};

std::vector<std::vector<int>> Map; // 0: 방 1 : 벽, 3: 가로분할 길, 4: 세로분할 길

// 전방선언
void BSP_Init();
void BSP_CreateMap();
Rect BSP_DevideRoom(Rect rect, bool dir);
Rect BSP_CreateRoom(Rect rect);
void BSP_ConnectRoom(Rect parentRoom, Rect childRoom, int divideLine, bool LineDir);
void BSP_Print();
void BSP_Save();

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

void BSP_Init()
{
    BSP_CreateMap();
    BSP_Print();
    BSP_Save();
}

void BSP_CreateMap()
{
    // 맵의 모든값을 WallNum으로 초기화
    Map.resize(MaxHeight);
    for (size_t i = 0; i < MaxHeight; i++)
    {
        Map[i].resize(MaxWidth);
        std::fill(Map[i].begin(), Map[i].end(), WallNum);
    }

    // Rect root = Rect(MaxWidth / 4, MaxHeight / 4, MaxWidth / 2, MaxHeight / 2);
    Rect root = Rect(0, 0, MaxWidth, MaxHeight);

    BSP_DevideRoom(root, GetRandomInt(0, 1));
}

Rect BSP_DevideRoom(Rect rect, bool LineDir)
{
    if (rect.width < 20 || rect.height < 20)
    {
        return BSP_CreateRoom(rect);
    }

    // row == 0, column == 1
    if (LineDir)
    {
        // 가로 분할
        // 분할 라인의 시작점 1/3 ~ 2/3 범위로 제한
        int divideline = rect.height / 3 + GetRandomInt(0, rect.height / 3);

        Rect parentRect = Rect(rect.x, rect.y, rect.width, divideline);
        Rect childRect = Rect(rect.x, rect.y + divideline, rect.width, rect.height - divideline);

        Rect parentRoom = BSP_DevideRoom(parentRect, !LineDir);
        Rect childRoom = BSP_DevideRoom(childRect, !LineDir);

        BSP_ConnectRoom(parentRoom, childRoom, rect.y + divideline, LineDir);

        return parentRoom;
    }
    else
    {
        // 세로 분할
        // 분할 라인의 시작점 1/3 ~ 2/3 범위로 제한
        int divideline = rect.width / 3 + GetRandomInt(0, rect.width / 3);

        Rect parentRect = Rect(rect.x, rect.y, divideline, rect.height);
        Rect childRect = Rect(rect.x + divideline, rect.y, rect.width - divideline, rect.height);
        Rect parentRoom = BSP_DevideRoom(parentRect, !LineDir);
        Rect childRoom = BSP_DevideRoom(childRect, !LineDir);

        BSP_ConnectRoom(parentRoom, childRoom, rect.x + divideline, LineDir);

        return parentRoom;
    }
}

Rect BSP_CreateRoom(Rect rect)
{
    // 가로 세로 2칸씩 뺀 크기의 방을 만든다.
    for (int y = 2; y <= rect.height - 2; y++)
    {
        for (int x = 2; x <= rect.width - 2; x++)
        {
            Map[rect.y + y][rect.x + x] = RoomNum;
        }
    }

    // 테두리 처리
    for (int x = 2; x <= rect.width - 2; x++)
    {
        Map[rect.y + 2][rect.x + x] = BorderNum;               // 상단 가로
        Map[rect.y + rect.height - 2][rect.x + x] = BorderNum; // 하단 가로
    }

    for (int y = 2; y <= rect.height - 2; y++)
    {
        Map[rect.y + y][rect.x + 2] = BorderNum;              // 좌측 세로
        Map[rect.y + y][rect.x + rect.width - 2] = BorderNum; // 우측 세로
    }

    return Rect(rect.x + 2, rect.y + 2, rect.width - 2, rect.height - 2);
}

void BSP_ConnectRoom(Rect parentRoom, Rect childRoom, int divideLine, bool LineDir)
{
    if (LineDir)
    {
        // 가로 분할
        // (시작위치 + 2) - (방생성 오프셋 4)
        int x1 = (parentRoom.x + 2) + GetRandomInt(0, parentRoom.width - 6);
        int x2 = (childRoom.x + 2) + GetRandomInt(0, childRoom.width - 6);

        int minX = std::min(x1, x2);
        int maxX = std::max(x1, x2);

        int i = 0;

        for (i = 0; minX + i <= maxX; i++)
            Map[divideLine][minX + i] = RowNum;

        for (i = 1; Map[divideLine - i][x1] == WallNum; i++)
            Map[divideLine - i][x1] = ColNum;

        // 통로 구멍 뚫기
        if (Map[divideLine - i][x1] == BorderNum)
            Map[divideLine - i][x1] = RoomNum;

        for (i = 1; Map[divideLine + i][x2] == WallNum; i++)
            Map[divideLine + i][x2] = ColNum;

        // 통로 구멍 뚫기
        if (Map[divideLine + i][x2] == BorderNum)
            Map[divideLine + i][x2] = RoomNum;
    }
    else
    {
        // 세로 분할
        // (시작위치 + 2) - (방생성 오프셋 4)
        int y1 = (parentRoom.y + 2) + GetRandomInt(0, parentRoom.height - 6);
        int y2 = (childRoom.y + 2) + GetRandomInt(0, childRoom.height - 6);

        int minY = std::min(y1, y2);
        int maxY = std::max(y1, y2);

        int i = 0;
        for (i = 0; minY + i <= maxY; i++)
            Map[minY + i][divideLine] = ColNum;

        for (i = 1; Map[y1][divideLine - i] == WallNum; i++)
            Map[y1][divideLine - i] = RowNum;

        // 통로 구멍 뚫기
        if (Map[y1][divideLine - i] == BorderNum)
            Map[y1][divideLine - i] = RoomNum;

        for (i = 1; Map[y2][divideLine + i] == WallNum; i++)
            Map[y2][divideLine + i] = RowNum;

        // 통로 구멍 뚫기
        if (Map[y2][divideLine + i] == BorderNum)
            Map[y2][divideLine + i] = RoomNum;
    }
}

void BSP_Print()
{
    system("cls"); // 콘솔창 clear

    for (size_t y = 0; y < Map.size(); y++)
    {
        for (size_t x = 0; x < Map[y].size(); x++)
        {
            std::cout << Map[y][x];
        }

        std::cout << std::endl;
    }
}

void BSP_Save()
{
    std::string line;
    std::ofstream file("..\\BSP\\MAP.txt");

    file << MaxHeight << " " << MaxWidth << std::endl;

    if (file.is_open())
    {
        for (size_t y = 0; y < Map.size(); y++)
        {
            for (size_t x = 0; x < Map[y].size(); x++)
            {
                file << Map[y][x];
            }

            file << std::endl;
        }

        file.close();
    }
    else
    {
        std::cout << "error" << std::endl;
        return;
    }
}
} // namespace sy
