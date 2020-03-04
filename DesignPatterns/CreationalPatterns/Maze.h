// Maze, P63
// 创建迷宫,
// 一个迷宫由一系列房间组成, 一个房间知道它的邻居.
// 可能的邻居: 另一个房间, 一堵墙, 到另一个房间的一扇门(控制能不能进入连接的房间)
// 所有的迷宫构件 类: Room, Door, Wall

// 一个房间的四个面
enum Direction { North, South, East, West};

// 迷宫构件的抽象基类 MapSite
// 为简化,  仅定义一个Enter 操作, 如果是房间, 则改变你的位置,
// 如果是门, 门开---> 进入另外一个房间, 门关---> 碰壁, 啥也不会发生
class MapSite
{
public:
	virtual void Enter(/*Pawn& */) = 0;

	virtual ~MapSite() { }
};

class Room : public MapSite
{
public:
	Room(int roomNo) : _roomNumber(roomNo) { }

	MapSite* GetSide(Direction) const;
	void SetSide(Direction, MapSite*); 

	virtual void Enter();

	virtual ~Room();
private:
	MapSite* _sides[4];
	int _roomNumber;
};

class Wall : public MapSite
{
public:
	Wall();

	virtual void Enter();

	virtual ~Wall() = default;
};

class Door : public MapSite
{
public:
	Door(Room* = 0, Room * = 0);

	virtual void Enter();

	Room* OtherSideFrom(Room*);

	virtual ~Door();
private:
	Room* _room1;
	Room* _room2;
	bool _isOpen;
};

// 表示并管理房间集合的类
// 根据RoomNo就能找到特定的房间
class Maze
{
public:
	Maze();

	void AddRoom(Room*);
	Room* RoomNo(int) const;
private:
	//...
};

// 创建迷宫的类
class MazeGame
{
public:
	// 用一系列操作将构建加入到迷宫中
	// 可以将room的四周初始化为墙以简化代码(但这不是问题本质)
	// 这种办法对迷宫布局进行硬编码, 不够灵活
	Maze* CreateMaze()
	{
		Maze* aMaze = new Maze;
		Room* r1 = new Room(1);
		Room* r2 = new Room(2);
		Door* theDoor = new Door(r1, r2);

		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);

		r1->SetSide(North, new Wall);
		r1->SetSide(East, theDoor);
		r1->SetSide(South, new Wall);
		r1->SetSide(West, new Wall);

		r2->SetSide(North, new Wall);
		r2->SetSide(East, new Wall);
		r2->SetSide(South, new Wall);
		r2->SetSide(West, theDoor);

		return aMaze;
	}
};