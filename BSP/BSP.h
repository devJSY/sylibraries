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
    void Init();
    void Init(int width, int height);

  private:
    void CreateMap();
    Rect DevideRoom(Rect rect, bool LineDir);
    Rect CreateRoom(Rect rect);
    void ConnectRoom(Rect parentRoom, Rect childRoom, int divideLine, bool LineDir);
    void Print();
    void Save();

  private:
    std::vector<std::vector<int>> Map;

    int RoomNum;
    int WallNum;
    int BorderNum;
    int RowNum;
    int ColNum;
    int MaxWidth;
    int MaxHeight;
};
} // namespace sy