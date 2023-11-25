#pragma once
#include "..\\C++\\define.h"
#include "..\\C++\global.h"

namespace sy
{
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

class BSP
{
    SINGLETON(BSP)

  public:
    int Init();
    int Init(int width, int height);

  public:
    void Print();
    int Save();

  private:
    int CreateMap();
    Rect DevideRoom(Rect rect, bool LineDir);
    Rect CreateRoom(Rect rect);
    void ConnectRoom(Rect parentRoom, Rect childRoom, int divideLine, bool LineDir);

  private:
    std::vector<std::vector<int>> m_Map;

    int m_RoomNum;
    int m_WallNum;
    int m_BorderNum;
    int m_RowNum;
    int m_ColNum;
    int m_MaxWidth;
    int m_MaxHeight;
};
} // namespace sy