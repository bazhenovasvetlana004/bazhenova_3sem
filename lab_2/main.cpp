#include <iostream>

template <typename T> class Grid;
template <typename T>
class Grid {
private:
    T* memory;
    size_t x_size, y_size;
public:
    Grid(size_t x, size_t y)
    {
        x_size = x;
        y_size = y;
        memory = new T[y_size * x_size];
    };
    ~Grid()
    {
        delete memory;
    };
    T operator()(size_t x_idx, size_t y_idx) const
    {
        return memory[y_idx * x_size + x_idx];
    };
    T& operator()(size_t x_idx, size_t y_idx)
    {
        return memory[y_idx * x_size + x_idx];
    };
    size_t get_xsize() const
    {
        return x_size;
    };
    size_t get_ysize() const
    {
            return y_size;
    };
    Grid<T>& operator=(T element)
    {
        for (int i = 0; i < y_size * x_size; i++)
        {
            memory[i] = element;
        }
        return *this;
    };
    friend std::ostream& operator<<(std::ostream& os, Grid<T> const& grid)
    {
        for (int i = 0; i < grid.get_ysize(); i++)
        {
            for (int j = 0; j < grid.get_xsize(); j++)
            {
                os << grid(j, i) << ' ';
            }
            os << std::endl;
        }
        return os;
    };
    friend std::istream& operator>>(std::istream& is, Grid<T>& grid)
    {
        for (int i = 0; i < grid.get_ysize(); i++)
        {
            for (int j = 0; j < grid.get_xsize(); j++)
            {
                is >> grid(j, i);
            }
        }
        return is;
    };
};

int main()
{
	Grid<int> grid1(3, 3);
	std::cin >> grid1; //ввод
	std::cout << grid1 << std::endl;
	
	Grid<int> grid2(2, 2);
    grid2 = 13; //все
    grid2(1, 1) = 7; //отдельные значения
    grid2(0, 1) = 94;
	std::cout << grid2 << std::endl;

    Grid<int> grid3(8, 7);

    std::cout << grid2(1, 0) << std::endl; //значение
	std::cout << grid3.get_xsize() << ' ' << grid3.get_ysize() << std::endl; //размер
    std::cout << grid1 << std::endl; //вывод

    return 0;
}