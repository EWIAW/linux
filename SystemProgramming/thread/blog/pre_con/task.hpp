class Task
{
public:
    int _x_;
    int _y_;

    Task(int x = 0, int y = 0)
        : _x_(x),
          _y_(y)
    {
    }

    int operator()()
    {
        return _x_ + _y_;
    }
};