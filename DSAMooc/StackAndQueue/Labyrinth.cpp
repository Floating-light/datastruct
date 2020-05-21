#include <vector>
#include <iostream>
#include <queue>
typedef enum { AVAILABLE, ROUTE, BACKTBACKED, WALL} Status; // labyrinth cell status

typedef enum { UNKNOWN, EAST, SOUTH, WEST, NORTH, NO_WAY } ESWN;

inline ESWN nextESWN( ESWN eswn ) { return ESWN ( eswn + 1); }

struct Cell 
{
    int x, y;
    Status status;
    ESWN incoming, outgoing;
};
#define LABYRINTH_MAX 24
Cell laby[LABYRINTH_MAX][LABYRINTH_MAX];
inline Cell* neighbor( Cell* cell)
{
    switch( cell->outgoing)
    {
        case EAST:
            return cell + LABYRINTH_MAX;
        case SOUTH: 
            return cell + 1;
        case WEST: 
            return cell - LABYRINTH_MAX;
        case NORTH:
            return cell - 1;   
        default:
            return nullptr;
    }
}

inline Cell* advance( Cell* cell)
{
    Cell* next;
    switch( cell->outgoing )
    {
        case EAST:
            next = cell + LABYRINTH_MAX;
            next->incoming = WEST;
            break;
        case SOUTH:
            next = cell + 1;
            next->incoming = NORTH;
            break;
        case WEST:
            next = cell - LABYRINTH_MAX;
            next->incoming = EAST;
            break;
        case NORTH:
            next = cell - 1;
            next->incoming = SOUTH;
            break;
        default:
            next = nullptr;
    }
    return next;
}

bool labyrinth(Cell Laby[LABYRINTH_MAX][LABYRINTH_MAX], Cell* source, Cell* target)
{
    if(source == nullptr || source->status != AVAILABLE)
        return false;
    std::vector<Cell* > path;
    path.push_back(source);
    source->incoming = UNKNOWN;
    source->status = ROUTE;

    while(path.size() != 0)
    {
        Cell* current = path.back();
        if(current == target)
            return true;
        while((current->outgoing = nextESWN(current->outgoing)) < NO_WAY) // probing
        {
            if(neighbor(current)->status == AVAILABLE)
                break;
        }
        if(current->outgoing == NO_WAY) // backtracking
        {
            current->status = BACKTBACKED;
            path.pop_back();
        }
        else
        {
            path.push_back((current = advance(current)));
            current->outgoing = UNKNOWN;
            current->status = ROUTE;
        }
    }
    return false;
}

void printMap()
{
    for(int i = 0; i < LABYRINTH_MAX; ++i)
    {
        for(int j = 0; j < LABYRINTH_MAX; ++j)
        {
            if(laby[i][j].status == ROUTE)
            {
                std::cout << " *";
            }
            else if(laby[i][j].status == WALL)
            {
                std::cout << " |";
            }
            else
            {
                std::cout << " o";
            }
            
        }
        std::cout << std::endl;
    }
}

int main() 
{
    for(int i = 0; i < LABYRINTH_MAX; ++i)
    {
        laby[0][i].status = WALL;
        laby[LABYRINTH_MAX - 1][i].status = WALL;
        laby[i][0].status = WALL;
        laby[i][LABYRINTH_MAX - 1].status = WALL;
        for(int j = 0; j < LABYRINTH_MAX; ++j)
        {
            laby[i][j].x = i;
            laby[i][j].y = j;
        }
    }
    srand(time(NULL)*10);
    for(int n = 0; n < 100; ++n)
    {
        laby[rand()%24][rand()%24].status = WALL;
    }
    laby[1][1].status = AVAILABLE;
    laby[LABYRINTH_MAX - 2][LABYRINTH_MAX -2].status = AVAILABLE;
    printMap();
    std::cout << "<<<------------------------------------------>>>" << std::endl;
    if(labyrinth(laby,&laby[1][1], &laby[LABYRINTH_MAX - 2][LABYRINTH_MAX -2]))
    {
        printMap();
    }
    else
    {
        std::cout << "no way" << std::endl;
    }

}
