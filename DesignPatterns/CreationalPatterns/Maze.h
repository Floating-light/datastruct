#include <memory>

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

	virtual ~MapSite() = default;
};

class Room : public MapSite
{
public:
	Room(int roomNo) : _roomNumber(roomNo) { }

	std::shared_ptr<MapSite> GetSide(Direction) const;
	void SetSide(Direction, std::shared_ptr<MapSite>); 

	virtual void Enter();

	virtual ~Room();
private:
	std::shared_ptr<MapSite> _sides[4];
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
	Door(std::shared_ptr<Room> r1 = 0, std::shared_ptr<Room> r2 = 0): _room1(r1), _room2(r2) { }

	virtual void Enter();

	std::shared_ptr<Room> OtherSideFrom(std::shared_ptr<Room>);

	virtual ~Door();
private:
	std::shared_ptr<Room> _room1;
	std::shared_ptr<Room> _room2;
	bool _isOpen;
};

// 表示并管理房间集合的类
// 根据RoomNo就能找到特定的房间
class Maze
{
public:
	Maze();

	void AddRoom(std::shared_ptr<Room>);
	std::shared_ptr<Room> RoomNo(int) const;
private:
	//...
};

class MazeFactory
{
public:
	MazeFactory() = default;

	virtual std::shared_ptr<Maze> MakeMaze() const
	{
		return std::make_shared<Maze>();
	}

	virtual std::shared_ptr<Wall> MakeWall() const
	{
		return std::make_shared<Wall>();
	}

	virtual std::shared_ptr<Room> MakeRoom(int roomNo) const
	{
		return std::make_shared<Room>(roomNo);
	}

	virtual std::shared_ptr<Door> MakeDoor(std::shared_ptr<Room> r1, std::shared_ptr<Room> r2) const
	{
		return std::make_shared<Door>(r1, r2);
	}

	virtual ~MazeFactory() = default;
};

// 创建迷宫的类
class MazeGame
{
public:
	// 直接调用各个构建的构造函数
	// 用一系列操作将构建加入到迷宫中
	// 可以将room的四周初始化为墙以简化代码(但这不是问题本质)
	// 这种办法对迷宫布局进行硬编码, 不够灵活
	std::shared_ptr<Maze> CreateMaze()
	{
		std::shared_ptr<Maze> aMaze = std::make_shared<Maze>();
		std::shared_ptr<Room> r1 = std::make_shared<Room>(1);
		std::shared_ptr<Room> r2 = std::make_shared<Room>(2);
		std::shared_ptr<Door> theDoor = std::make_shared<Door>(r1, r2);

		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);

		r1->SetSide(North, std::make_shared<Wall>());
		r1->SetSide(East, theDoor);
		r1->SetSide(South, std::make_shared<Wall>());
		r1->SetSide(West, std::make_shared<Wall>());

		r2->SetSide(North, std::make_shared<Wall>());
		r2->SetSide(East, std::make_shared<Wall>());
		r2->SetSide(South, std::make_shared<Wall>());
		r2->SetSide(West, theDoor);

		return aMaze;
	}

	// 使用工厂
	// 方便使用不同的构件创建迷宫(派生MazeFactory)
	std::shared_ptr<Maze> CreateMaze(MazeFactory& factory)
	{
		std::shared_ptr<Maze> aMaze = factory.MakeMaze();
		std::shared_ptr<Room> r1 = factory.MakeRoom(1);
		std::shared_ptr<Room> r2 = factory.MakeRoom(2);
		
		std::shared_ptr<Door> aDoor = factory.MakeDoor(r1, r2);
		
		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);

		r1->SetSide(North, factory.MakeWall());
		r1->SetSide(East, aDoor);
		r1->SetSide(South, factory.MakeWall());
		r1->SetSide(West, factory.MakeWall());

		r2->SetSide(North, factory.MakeWall());
		r2->SetSide(East, factory.MakeWall());
		r2->SetSide(South, factory.MakeWall());
		r2->SetSide(West, aDoor);

		return aMaze;
	}
};



