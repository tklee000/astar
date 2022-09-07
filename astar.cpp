// astar.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <stdlib.h>

using namespace std;

int maph = 10;
int mapw = 10;
char map[10][10] =
/*
{
    { 0,0,0,1,0,1,0,1,0,0},
    { 1,1,0,1,0,1,0,1,0,0},
    { 0,1,0,1,0,1,0,1,0,0},
    { 0,1,0,1,0,1,0,0,0,0},
    { 0,1,0,1,0,1,1,1,1,0},
    { 0,1,0,1,0,1,0,0,0,0},
    { 0,1,0,1,0,1,0,1,0,1},
    { 0,1,0,0,0,1,0,1,0,0},
    { 0,1,1,1,0,1,0,1,0,0},
    { 0,0,0,1,0,0,0,1,1,9}
};
*/
{
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 1,1,0,0,0,0,0,0,0,0},
    { 0,1,0,0,0,0,0,0,0,0},
    { 2,1,0,0,0,0,0,0,0,9},
    { 0,1,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
};

constexpr float INF = (987654321.0f);

const int dir_x[4] = { 0, 0, 1, -1 };
const int dir_y[4] = { -1, 1, 0, 0 };

/*


열린목록 : 아직 방문하지 않은 노드
닫힌목록 : 이미 방문했거나 갈수없는 노드의 집합
셀 : f,g,h,부모의 위치를 가지고 있는 셀정보, 맵크기만큼 2차원 배열로 가짐


*/


// 한 정점의 비용
class CostCoord {
public:
    CostCoord(float _cost, int _x, int _y) : cost(_cost),x(_x), y(_y) {}
    float cost;
    int x, y;
};

struct Less {
    constexpr bool operator()(const CostCoord& _Left, const CostCoord& _Right) const {
        return _Left.cost > _Right.cost;
    }
};

/* Coord: 한 정점의 좌표 */
struct Coord {
    int x;
    int y;
};

// 셀 --> 지도의 동일한 크기의 셀[10][10] 이 필요~
// 노드의 비용과 부모의 정보를 담는다.
class Cell {

public:
    Cell()
    {
        f = g = h = INF;
        parent_x = parent_y = -1;
    }
    float f;
    float g;
    float h;
    int parent_x;
    int parent_y;
};

vector<Coord> TrackingQueue;



bool isInMap(int x, int y)
{
    if (x >= 0 && x < mapw && y >=0 && y < maph) return true;
    return false;
}

bool isDestination(int x, int y)
{
    if (map[y][x] == 9) return true;
    return false;
}

bool isWall(int x, int y)
{
    if (map[y][x] == 1) return true;
    return false;
}

float CalcCostHeuristic(int x, int y, int dest_x, int dest_y)
{
    return sqrt((dest_x - x)* (dest_x - x) + (dest_y - y)* (dest_y - y));
    //return (dest_x - x) + (dest_y - y);
}

// 모든셀에 무한대값을 넣고 부모를 -1로 만든다.
vector<vector<Cell>> cell(maph, vector<Cell>(mapw));
vector<vector<bool>> closedList(maph, vector<bool>(mapw));
priority_queue<CostCoord, vector<CostCoord>, Less> openList; // 정렬된 구조체로 Cost가 낮은 게 자동으로 정렬되어서 맨앞으로 온다. 

int start_x = 0;
int start_y = 5;
int dest_x = 9;
int dest_y = 5;


void init()
{
    // 출발지점의 비용값은 0, 부모를 출발지로 넣어준다.
    Cell& startcell = cell[start_y][start_x];
    startcell.f = startcell.g = startcell.h = 0.0f;
    startcell.parent_x = start_x;
    startcell.parent_y = start_y;

    openList.push(CostCoord(0.0f, start_x, start_y));
}

int TrackingPath(int dx, int dy)
{
    stack<Coord> TrackingStack;
    int cx, cy;

    cx = dx;
    cy = dy;

    TrackingStack.push({ cx, cy });

    while (!(cell[cy][cx].parent_x == cx &&
        cell[cy][cx].parent_y == cy)) {
        int temp_x = cell[cy][cx].parent_x;
        int temp_y = cell[cy][cx].parent_y;
        cx = temp_x;
        cy = temp_y;
        TrackingStack.push({ cx, cy });
    }

    while (!TrackingStack.empty()) {
        TrackingQueue.push_back(TrackingStack.top());
        TrackingStack.pop();
    }

    return 0;
}


int search()
{

    while (!openList.empty()) {

        // OpenList의 맨앞 CostCoord를 가지고 온다.

        CostCoord current = openList.top();
        openList.pop();

        int cx = current.x;
        int cy = current.y;

        // 현지점의 CloseList를 방문처리
        closedList[cy][cx] = true;

        float nf, ng, nh;

        // 동서남북 방향에 대해서 계산
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dir_x[i];
            int ny = cy + dir_y[i];
            
                
            // 여기가 지도안인가? 당연히 쳐다볼필요가 없지...^^;
            if (isInMap(nx, ny)) {

                Cell& nextcell = cell[ny][nx];

                // 도착했나?
                if (isDestination(nx, ny)) {
                    nextcell.parent_x = cx;
                    nextcell.parent_y = cy;

                    // 도착되어 cell의 parent를 역추적해서 경로를 빼내올수 있다.
                    TrackingPath(dest_x,dest_y);

                    return true;
                }
                // closedList 첵크해서 전에 방문했다고 해둔곳 은 또 볼필요없고...
                // 벽이 아닌곳인가?
                else if (!closedList[ny][nx] && !isWall( nx, ny)) {
                    ng = cell[cy][cx].g + 1.000f;
                    nh = CalcCostHeuristic(nx, ny, dest_x, dest_y);
                    nf = ng + nh;

                    // 다음 가볼곳이 f 값이 무한대(처음 가보는곳) 혹은 지금의 f 보다는 작은곳은 계산된 결과를 갱신한다.
                    if (nextcell.f == INF || nextcell.f > nf) {

                        // 계산된 fhg를 넣어주고
                        nextcell.g = ng;
                        nextcell.h = nh;
                        nextcell.f = nf;

                        // 바로 직전 좌표도 넣어주고
                        nextcell.parent_x = cx;
                        nextcell.parent_y = cy;

                        // 오픈리스트에 추가~~
                        openList.push(CostCoord(nf,nx,ny));
                        /* 4방향 살펴본후 갈수 있는 곳은 계산해서 전부 노드 넣어줌...
                         그리고 큐에서 꺼낼때는 자동으로 배열이 소팅된 상태로 비용이 가장 적은 녀석을 우선적으로 꺼내진다.
                         그래서 가장 빠른 지점으로 갈수 있는 방향대로 간다.
                        */
                    }
                }
            }
        }

    }

    return 0;
}



void print_map(int cx, int cy)
{
    for (int y = 0; y < maph; y++)
    {
        for (int x = 0; x < mapw; x++) {
            
            if (x == cx && y == cy)
                cout << "@";
            else
                if (map[y][x] == 0 || map[y][x] == 2) cout << " ";
                else if (map[y][x] == 9) cout << "*";
                else cout << "#";
        }
        cout << endl;
    }
}

void autoset_start_dest_position()
{
    for (int y = 0; y < maph; y++)
    {
        for (int x = 0; x < mapw; x++) {
            if (map[y][x] == 2) {
                start_x = x;
                start_y = y;
            }
            else if (map[y][x] == 9) {
                dest_x = x;
                dest_y = y;

            }
        }
    }
}


int main()
{
    autoset_start_dest_position();
    init();
    search();

    for (auto v = TrackingQueue.begin(); v < TrackingQueue.end(); v++)
    {
        system("cls");
        print_map(v->x, v->y);
    }
    
    return 0;
}
