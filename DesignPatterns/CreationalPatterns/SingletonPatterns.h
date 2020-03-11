#include "Maze.h"

class Singleton
{
public:
	static Singleton* Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new Singleton();
		}
		return _instance;
	}
protected:
	Singleton();
private:
	static Singleton* _instance;
};

Singleton* Singleton::_instance = nullptr;