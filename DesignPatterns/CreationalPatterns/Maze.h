#include <memory>
#include <iostream>
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

class Spell;
// ħ��Room
class EnchantedRoom : public Room
{
public:
	EnchantedRoom(int roomNo, std::shared_ptr<Spell> s) : Room(roomNo), spell(s){ }

private:
	std::shared_ptr<Spell> spell;
};
// ��ը���ķ���
class RoomWithABomb : public Room
{
public:
	RoomWithABomb(int roomNo) : Room(roomNo) { }
	
	std::shared_ptr<MapSite> GetSide(Direction) const;
	void SetSide(Direction, std::shared_ptr<MapSite>);

	virtual void Enter();

	virtual ~RoomWithABomb() = default;
private:
	std::shared_ptr<MapSite> _sides[4];
	int _roomNumber;
};


class Wall : public MapSite
{
public:
	Wall();
	Wall(const Wall&) = default;

	virtual std::shared_ptr<Wall> clone()
	{
		return std::make_shared<Wall>(*this);
	}

	virtual void Enter();

	virtual ~Wall() = default;
};

// ����ը��ǽ
class BombedWall : public Wall
{
public:
	BombedWall() = default;
	BombedWall(const BombedWall& bw) :Wall(bw)
	{
		_bomb = bw._bomb;
	}

	std::shared_ptr<Wall> clone() override 
	{
		return std::make_shared<BombedWall>(*this);
	}


	void Enter() override;

	bool HasBomb() { return _bomb; }

	virtual ~BombedWall() = default;
private:
	bool _bomb;
};

class Door : public MapSite
{
public:
	Door(std::shared_ptr<Room> r1 = 0, std::shared_ptr<Room> r2 = 0): _room1(r1), _room2(r2), _isOpen(false) { }

	// copy constrcutor
	Door(const Door& d)
	{
		_room1 = d._room1;
		_room2 = d._room2;
		_isOpen = d._isOpen;
	}

	virtual void Initialize(std::shared_ptr<Room>r1, std::shared_ptr<Room> r2)
	{
		_room1 = r1;
		_room2 = r2;
		_isOpen = false;
	}
	virtual std::shared_ptr<Door> clone() const
	{
		return std::make_shared<Door>(*this);
	}

	virtual void Enter();

	virtual std::shared_ptr<Room> OtherSideFrom(std::shared_ptr<Room>);

	virtual ~Door() = default;
private:
	std::shared_ptr<Room> _room1;
	std::shared_ptr<Room> _room2;
	bool _isOpen;
};

// ħ�俪�ŵ�Door
class DoorNeedingSpell : public Door
{
public:
	DoorNeedingSpell(std::shared_ptr<Room> r1 = 0, std::shared_ptr<Room> r2 = 0) : Door(r1, r2) { }

	virtual void Enter();
	virtual std::shared_ptr<Room> OtherSideFrom(std::shared_ptr<Room>);

	virtual ~DoorNeedingSpell() = default;

};

// ��ʾ�������伯�ϵ���
// ����RoomNo�����ҵ��ض��ķ���
class Maze
{
public:
	Maze();

	void AddRoom(std::shared_ptr<Room>);
	// �Ҳ����򷵻�nullptr
	std::shared_ptr<Room> RoomNo(int) const;
private:
	//...
};


// ��ͨ�Թ�����, ����ģʽ
class MazeFactory
{
public:
	MazeFactory() = default;

	// ��������
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

// ħ���Թ�����
class EnchantedMazeFactory : public MazeFactory
{
public:
	EnchantedMazeFactory() = default;

	// �������������������Roon ����EnchantedRoom
	// ��֤��DoorNeedingSpell����ȷ��
	virtual std::shared_ptr<Room> MakeDoor(int n) const
	{
		return std::make_shared<EnchantedRoom>(n, CastSpell());
	}

	virtual std::shared_ptr<Door> MakeDoor(std::shared_ptr<Room> r1, std::shared_ptr<Room> r2)
	{
		return std::make_shared<DoorNeedingSpell>(r1, r2);
	}
protected:
	std::shared_ptr<Spell> CastSpell() const;
};

// ը���Թ�����
class BombedMazeFactory : public MazeFactory
{
public:
	std::shared_ptr<Wall> MakeWall() const override
	{
		return std::make_shared<BombedWall>();
	}

	std::shared_ptr<Room> MakeRoom(int roomNo) const override
	{
		return std::make_shared<RoomWithABomb>(roomNo);
	}
};

// �Թ�����������
class MazeBuilder
{
public:
	virtual void BuildMaze() { }
	virtual void BuildRoom(int roomNo){ }
	virtual void BuildDoor(int roomFrom, int roomTo){ }

	virtual std::shared_ptr<Maze> GetMaze() { return 0; }

protected:
	MazeBuilder() = default;
};

// Builderģʽ
// ��ͨ�Թ�
class StandardMazeBuilder : public MazeBuilder
{
public:
	//template <class U> explicit shared_ptr (U* p);
	StandardMazeBuilder():_currentMaze(nullptr) {  }

	void BuildMaze() override
	{
		_currentMaze = std::make_shared<Maze>();
	}
	void BuildRoom(int i) override
	{
		if (!_currentMaze->RoomNo(i))
		{
			std::shared_ptr<Room> room = std::make_shared<Room>(i);
			_currentMaze->AddRoom(room);

			room->SetSide(North, std::make_shared<Wall>());
			room->SetSide(East, std::make_shared<Wall>());
			room->SetSide(South, std::make_shared<Wall>());
			room->SetSide(West, std::make_shared<Wall>());
		}
	}
	void BuildDoor(int n1, int n2) override
	{
		std::shared_ptr<Room> r1 = _currentMaze->RoomNo(n1);
		std::shared_ptr<Room> r2 = _currentMaze->RoomNo(n2);
		std::shared_ptr<Door> d = std::make_shared<Door>(r1, r2);

		r1->SetSide(CommonWall(r1, r2), d);
		r2->SetSide(CommonWall(r2, r1), d);
	}
	std::shared_ptr<Maze> GetMaze() override
	{
		return _currentMaze;
	}
private:
	// ȷ����������֮��Ĺ���ǽ�ڷ�λ
	Direction CommonWall(std::shared_ptr<Room> r1, std::shared_ptr<Room> r2)
	{
		// TODO
	}
	std::shared_ptr<Maze> _currentMaze;
};

// �Թ��ṹ����
// �Թ��Ľṹ��MazeGame����
class CountingMazeBuilder : public MazeBuilder
{
public:
	CountingMazeBuilder():MazeBuilder(), _doors(0), _rooms(0) { }
	void BuildMaze() override
	{
		_doors = 0;
		_rooms = 0;
	}
	void BuildRoom(int) override
	{
		_rooms++;
	}
	void BuildDoor(int, int) override
	{
		_doors++;
	}
	virtual void AddWall(int, Direction);

	void GetCounts(int& rooms, int& doors) const
	{
		rooms = _rooms;
		doors = _doors;
	}
private:
	int _doors;
	int _rooms;
};

// Prototype ԭ��ģʽ
class MazePrototypeFactory : public MazeFactory
{
public:
	MazePrototypeFactory(std::shared_ptr<Maze>, std::shared_ptr<Wall>, std::shared_ptr<Room>, std::shared_ptr<Door>);
	std::shared_ptr<Maze> MakeMaze() const override;
	std::shared_ptr<Room> MakeRoom(int) const override;
	std::shared_ptr<Wall> MakeWall() const override;
	std::shared_ptr<Door> MakeDoor(std::shared_ptr<Room> r1, std::shared_ptr<Room> r2) const override;
private:
	std::shared_ptr<Maze> _prototypeMaze;
	std::shared_ptr<Room> _prototypeRoom;
	std::shared_ptr<Wall> _prototypeWall;
	std::shared_ptr<Door> _prototypeDoor;
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

	// ʹ��Builder������
	std::shared_ptr<Maze> CreateMaze(MazeBuilder& builder)
	{
		builder.BuildMaze();

		builder.BuildRoom(1);
		builder.BuildRoom(2);
		builder.BuildDoor(1, 2);

		return builder.GetMaze();
	}
};

int main()
{
	std::shared_ptr<Maze> maze;
	MazeGame game;
	StandardMazeBuilder builder;

	maze = game.CreateMaze(builder);
	// maze = builder.GetMaze();

	int rooms, doors;
	CountingMazeBuilder countBuilder;
	auto _ = game.CreateMaze(countBuilder);

	countBuilder.GetCounts(rooms, doors);

	std::cout << "The maze has "
		<< rooms << " rooms and "
		<< doors << "doors" << std::endl;
}

