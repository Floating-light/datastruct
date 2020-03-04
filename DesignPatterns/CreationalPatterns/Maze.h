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

// ��ʾ�������伯�ϵ���
// ����RoomNo�����ҵ��ض��ķ���
class Maze
{
public:
	Maze();

	void AddRoom(Room*);
	Room* RoomNo(int) const;
private:
	//...
};

// �����Թ�����
class MazeGame
{
public:
	// ��һϵ�в������������뵽�Թ���
	// ���Խ�room�����ܳ�ʼ��Ϊǽ�Լ򻯴���(���ⲻ�����Ȿ��)
	// ���ְ취���Թ����ֽ���Ӳ����, �������
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