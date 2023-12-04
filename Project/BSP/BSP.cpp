#include "BSP.h"
#include "..\\C++\\func.h"

namespace sy
{
BSP::BSP()
    : m_RoomNum(0), m_WallNum(1), m_BorderNum(4), m_RowNum(5), m_ColNum(6), m_MaxWidth(160), m_MaxHeight(90),
      m_RoomSize(20) // 방크기가 너무 작아져 버리면 ConnectRoom시 문제가 발생할 수 있음
{
}

BSP::~BSP()
{
}

int BSP::Init()
{
    CreateMap();
    Print();
    Save();

    return 0;
}

int BSP::Init(int width, int height)
{
    m_MaxWidth = width;
    m_MaxHeight = height;

    Init();

    return 0;
}

int BSP::CreateMap()
{
    // 맵의 모든값을 WallNum으로 초기화
    m_Map.resize(m_MaxHeight);
    for (size_t i = 0; i < m_MaxHeight; i++)
    {
        m_Map[i].resize(m_MaxWidth);
        std::fill(m_Map[i].begin(), m_Map[i].end(), m_WallNum);
    }

    // Rect root = Rect(m_MaxWidth / 4, m_MaxHeight / 4, m_MaxWidth / 2, m_MaxHeight / 2);
    Rect root = Rect(0, 0, m_MaxWidth, m_MaxHeight);

    DevideRoom(root, GetRandomInt(0, 1));

    return 0;
}

Rect BSP::DevideRoom(Rect rect, bool LineDir)
{
    if (rect.width < m_RoomSize || rect.height < m_RoomSize)
    {
        return CreateRoom(rect);
    }

    // row == 0, column == 1
    if (LineDir)
    {
        // 가로 분할
        // 분할 라인의 시작점 1/3 ~ 2/3 범위로 제한
        int divideline = rect.height / 3 + GetRandomInt(0, rect.height / 3);

        Rect parentRect = Rect(rect.x, rect.y, rect.width, divideline);
        Rect childRect = Rect(rect.x, rect.y + divideline, rect.width, rect.height - divideline);

        Rect parentRoom = DevideRoom(parentRect, !LineDir);
        Rect childRoom = DevideRoom(childRect, !LineDir);

        ConnectRoom(parentRoom, childRoom, rect.y + divideline, LineDir);

        return parentRoom;
    }
    else
    {
        // 세로 분할
        // 분할 라인의 시작점 1/3 ~ 2/3 범위로 제한
        int divideline = rect.width / 3 + GetRandomInt(0, rect.width / 3);

        Rect parentRect = Rect(rect.x, rect.y, divideline, rect.height);
        Rect childRect = Rect(rect.x + divideline, rect.y, rect.width - divideline, rect.height);
        Rect parentRoom = DevideRoom(parentRect, !LineDir);
        Rect childRoom = DevideRoom(childRect, !LineDir);

        ConnectRoom(parentRoom, childRoom, rect.x + divideline, LineDir);

        return parentRoom;
    }
}

Rect BSP::CreateRoom(Rect rect)
{
    // 가로 세로 2칸씩 뺀 크기의 방을 만든다.
    for (int y = 2; y <= rect.height - 2; y++)
    {
        for (int x = 2; x <= rect.width - 2; x++)
        {
            m_Map[rect.y + y][rect.x + x] = m_RoomNum;
        }
    }

    // 테두리 처리
    for (int x = 2; x <= rect.width - 2; x++)
    {
        m_Map[rect.y + 2][rect.x + x] = m_BorderNum;               // 상단 가로
        m_Map[rect.y + rect.height - 2][rect.x + x] = m_BorderNum; // 하단 가로
    }

    for (int y = 2; y <= rect.height - 2; y++)
    {
        m_Map[rect.y + y][rect.x + 2] = m_BorderNum;              // 좌측 세로
        m_Map[rect.y + y][rect.x + rect.width - 2] = m_BorderNum; // 우측 세로
    }

    return Rect(rect.x + 2, rect.y + 2, rect.width - 2, rect.height - 2);
}

void BSP::ConnectRoom(Rect parentRoom, Rect childRoom, int divideLine, bool LineDir)
{
    if (LineDir)
    {
        // 가로 분할
        // (시작위치 + 2, 테두리 + 1) - (방생성 오프셋 4, 끝위치 + 2, 테두리 + 1)
        int x1 = (parentRoom.x + 3) + GetRandomInt(0, parentRoom.width - 4 - 3);
        int x2 = (childRoom.x + 3) + GetRandomInt(0, childRoom.width - 4 - 3);

        int minX = std::min(x1, x2);
        int maxX = std::max(x1, x2);

        int i = 0;

        for (i = 0; minX + i <= maxX; i++)
            m_Map[divideLine][minX + i] = m_RowNum;

        for (i = 1; m_Map[divideLine - i][x1] == m_WallNum; i++)
            m_Map[divideLine - i][x1] = m_ColNum;

        // 통로 구멍 뚫기
        if (m_Map[divideLine - i][x1] == m_BorderNum)
            m_Map[divideLine - i][x1] = m_RoomNum;

        for (i = 1; m_Map[divideLine + i][x2] == m_WallNum; i++)
            m_Map[divideLine + i][x2] = m_ColNum;

        // 통로 구멍 뚫기
        if (m_Map[divideLine + i][x2] == m_BorderNum)
            m_Map[divideLine + i][x2] = m_RoomNum;
    }
    else
    {
        // 세로 분할
        // (시작위치 + 2, 테두리 + 1) - (방생성 오프셋 4, 끝위치 + 2, 테두리 + 1)
        int y1 = (parentRoom.y + 3) + GetRandomInt(0, parentRoom.height - 4 - 3);
        int y2 = (childRoom.y + 3) + GetRandomInt(0, childRoom.height - 4 - 3);

        int minY = std::min(y1, y2);
        int maxY = std::max(y1, y2);

        int i = 0;
        for (i = 0; minY + i <= maxY; i++)
            m_Map[minY + i][divideLine] = m_ColNum;

        for (i = 1; m_Map[y1][divideLine - i] == m_WallNum; i++)
            m_Map[y1][divideLine - i] = m_RowNum;

        // 통로 구멍 뚫기
        if (m_Map[y1][divideLine - i] == m_BorderNum)
            m_Map[y1][divideLine - i] = m_RoomNum;

        for (i = 1; m_Map[y2][divideLine + i] == m_WallNum; i++)
            m_Map[y2][divideLine + i] = m_RowNum;

        // 통로 구멍 뚫기
        if (m_Map[y2][divideLine + i] == m_BorderNum)
            m_Map[y2][divideLine + i] = m_RoomNum;
    }
}

void BSP::Print()
{
    system("cls"); // 콘솔창 clear

    for (size_t y = 0; y < m_Map.size(); y++)
    {
        for (size_t x = 0; x < m_Map[y].size(); x++)
        {
            std::cout << m_Map[y][x];
        }

        std::cout << std::endl;
    }
}

int BSP::Save()
{
    std::string line;
    std::ofstream file("..\\..\\External\\MAP\\MAP.txt");

    file << m_MaxHeight << " " << m_MaxWidth << std::endl;

    if (file.is_open())
    {
        for (size_t y = 0; y < m_Map.size(); y++)
        {
            for (size_t x = 0; x < m_Map[y].size(); x++)
            {
                file << m_Map[y][x];
            }

            file << std::endl;
        }

        file.close();
    }
    else
    {
        std::cout << "error" << std::endl;
        return -1;
    }

    return 0;
}

} // namespace sy
