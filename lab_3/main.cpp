#include <vector>
#include <random>
#include <time.h>
#include <iostream>
#include<algorithm>
using namespace::std;


template <typename T> 
class Skiplist
{
public:
	struct Elem
	{
		Elem* next;
		Elem* prev;
		T value;
		Elem()
		{
			next = nullptr;
			value = 0;
			prev = nullptr;
		}
		Elem(Elem* n, Elem* p, T v)
		{
			next = n;
			prev = p;
			value = v;
		}
	};
	int size;
	vector<Elem*> Layers;
	int number_of_layers;
	double p;
public:
	//Конструктор "по умолчанию"
	Skiplist(double p)
	{
		size = 0;
		number_of_layers = 0;
		Layers.clear();
		this->p = p;
	};
	//Конструктор копироваания
	Skiplist(Skiplist& const that)
	{
		size = that.size;
		p = that.p;
		number_of_layers = that.number_of_layers;
		Layers.clear();
		for (int i = 1; i <= number_of_layers; i++)
		{
			Elem* temp = new Elem;
			temp->prev = nullptr;
			Elem* pointer = ((that.getLayers())[i - 1]);
			Layers.push_back(temp);

			while (true)
			{
				temp->value = pointer->value;
				if (pointer->next == nullptr)
				{
					temp->next = nullptr;
					break;
				}
				else
				{
					temp->next = new Elem;
					temp->next->prev = temp;
					pointer = pointer->next;
					temp = temp->next;
				}
			}
		}
	};
	//Оператор копирования
	Skiplist& operator=(Skiplist& const that)
	{
		size = that.size;
		p = that.p;
		number_of_layers = that.number_of_layers;
		Layers.clear();
		for (int i = 1; i <= number_of_layers; i++)
		{
			Elem* temp = new Elem;
			temp->prev = nullptr;
			Elem* pointer = ((that.getLayers())[i - 1]);
			Layers.push_back(temp);

			while (true)
			{
				temp->value = pointer->value;
				if (pointer->next == nullptr)
				{
					temp->next = nullptr;
					break;
				}
				else
				{
					temp->next = new Elem;
					temp->next->prev = temp;
					pointer = pointer->next;
					temp = temp->next;
				}
			}
		}
		return *this;
	};
	//Конструктор перемещения
	Skiplist(Skiplist<T>&& that)
	{
		size = that.size;
		number_of_layers = that.number_of_layers;
		p = that.p;
		for (int i = 0; i < number_of_layers; i++)
		{
			Layers.push_back(that.getLayers()[i]);
		}
		that.size = 0;
		that.number_of_layers = 0;
		that.p = 0;
		that.Layers.clear();
	};
	//Оператор перемещения
	Skiplist& operator=(Skiplist<T>&& that)
	{
		size = that.size;
		number_of_layers = that.number_of_layers;
		p = that.p;
		for (int i = 0; i < number_of_layers; i++)
		{
			Layers.push_back(that.getLayers()[i]);
		}
		that.size = 0;
		that.number_of_layers = 0;
		that.p = 0;
		that.Layers.clear();
		return *this;
	};
	vector<Elem*> getLayers() const
	{
		return Layers;
	};
	bool empty()const
	{
		if (size == 0)
			return true;
		else
			return false;
	};
	int Size()const
	{
		return size;
	};
	void insert(T newval)
	{
		size++;
		if (size == 1)
		{
			number_of_layers = 1;
			Layers.push_back(new Elem(nullptr, nullptr ,0));
			Layers[0]->next = new Elem(nullptr, nullptr, newval);
			return;
		}
		int k = 1;
		while (true)
		{
			if ((double)(rand() % 10000) / 10000 > p || k > number_of_layers)
				break;
			k++;
		}
		if (k > number_of_layers)
		{
			number_of_layers++;
			Layers.push_back(new Elem(nullptr, nullptr, 0));
			Layers[number_of_layers - 1]->next = new Elem(nullptr, nullptr, newval);
			k--;
		}
		for (int i = 0; i < k; i++)
		{
			Elem* temp = Layers[i];
			while (temp->next!=nullptr && temp->next->value <= newval)
				temp = temp->next;
				temp->next = new Elem(temp->next, temp, newval);
		}
	};
	void clear()
	{
		for (int i = 0; i < number_of_layers; i++)
		{
			Elem*  temp = Layers[i];
			while (true)
			{
				Elem* temp2 = temp;
				temp = temp->next;
				delete temp2;
				if (temp == nullptr)
					break;
			}
		}
		size = 0;
		number_of_layers = 0;
		Layers.clear();
	}
	void display_list()
	{
		for (int i = 0; i < number_of_layers; i++)
		{
			Elem* temp = Layers[i]->next;
			while (temp->next != nullptr)
			{
				cout << temp->value << "->";
				temp = temp->next;
			}
			cout << temp->value << endl;
		}
	}
	class skiplist_iterator
	{
	public:
		Elem* pointer;
		skiplist_iterator()
		{
			pointer = nullptr;
		}
		skiplist_iterator(skiplist_iterator& const iter)
		{
			pointer = iter.pointer;
		}
		skiplist_iterator(skiplist_iterator&& const iter)
		{
			pointer = iter.pointer;
		}
		skiplist_iterator(Elem* elem)
		{
			pointer = elem;
		}
		skiplist_iterator& operator=(skiplist_iterator&& const iter )
		{
			pointer = iter.pointer;
			return *this;
		};
		skiplist_iterator& operator=(skiplist_iterator& const iter)
		{
			pointer = iter.pointer;
			return *this;
		};
		skiplist_iterator& operator++()
		{
			pointer = pointer->next;
			return *this;
		}
		skiplist_iterator operator++(int)
		{
			skiplist_iterator temp = *this;
			++* this;
			return temp;
		}
		skiplist_iterator& operator--()
		{
			pointer = pointer->prev;
			return *this;
		}
		skiplist_iterator operator--(int)
		{
			skiplist_iterator temp = *this;
			--* this;
			return temp;
		}
		bool operator==(skiplist_iterator& const iter)
		{
			if (pointer == iter.pointer)
				return true;
			else return false;
		}
		bool operator==(skiplist_iterator&& const iter)
		{
			if (pointer == iter.pointer)
				return true;
			else return false;
		}
		bool operator!=(skiplist_iterator& const iter)
		{
			return !(*this == iter);
		}
		bool operator!=(skiplist_iterator&& const iter)
		{
			return !(*this == iter);
		}
		T operator*()
		{
			return pointer->value;
		}
	};
	skiplist_iterator begin() 
	{
		if (size != 0)
		{
			skiplist_iterator temp(Layers[0]->next);
			return temp;
		}
		else
		{
			skiplist_iterator temp;
			return temp;
		}
	}
	skiplist_iterator end()
	{
		if (size != 0)
		{
			skiplist_iterator temp(Layers[0]->next);
			while (temp.pointer->next != nullptr)
				temp++;
			return temp;
		}
		else
		{
			skiplist_iterator temp;
			return temp;
		}
	}
	template <class iter>
	void insert(iter begin, iter end)
	{
		while (begin != end)
		{
			this->insert(*begin);
			begin++;
		}
	};
};


int main()
{
	srand(time(0));
	Skiplist<int> a(0.8);
	for (int i = 1; i <=20; i++)
	{
		a.insert(i);
	}
	a.display_list();
	Skiplist<int> b(a);
	b.display_list();
	Skiplist<int> c(0.2);
	c = move(b);
	c.display_list();
	Skiplist<int> d(move(c));
	d.display_list();
	Skiplist<int> e(1);
	e = d;
	e.display_list();
	vector<int> vector_of_numbers = { -5,3,7 };
	a.insert(vector_of_numbers.begin(), vector_of_numbers.end());
	a.display_list();
	Skiplist<int>::skiplist_iterator it = a.begin();
	for (int i = 0; i < 5; i++)
		it++;
	cout << *it;
}