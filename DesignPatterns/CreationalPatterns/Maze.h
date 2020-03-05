#include <memory>

// Maze, P63
// �����Թ�,
// һ���Թ���һϵ�з������, һ������֪�������ھ�.
// ���ܵ��ھ�: ��һ������, һ��ǽ, ����һ�������һ����(�����ܲ��ܽ������ӵķ���)
// ���е��Թ����� ��: Room, Door, Wall

// һ��������ĸ���
enum Direction { North, South, East, West};

// �Թ������ĳ������ MapSite
// Ϊ��,  ������һ��Enter ����, ����Ƿ���, ��ı����λ��,
// �������, �ſ�---> ��������һ������, �Ź�---> ����, ɶҲ���ᷢ��
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

// ��ʾ�������伯�ϵ���
// ����RoomNo�����ҵ��ض��ķ���
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

// �����Թ�����
class MazeGame
{
public:
	// ֱ�ӵ��ø��������Ĺ��캯��
	// ��һϵ�в������������뵽�Թ���
	// ���Խ�room�����ܳ�ʼ��Ϊǽ�Լ򻯴���(���ⲻ�����Ȿ��)
	// ���ְ취���Թ����ֽ���Ӳ����, �������
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

	// ʹ�ù���
	// ����ʹ�ò�ͬ�Ĺ��������Թ�(����MazeFactory)
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



