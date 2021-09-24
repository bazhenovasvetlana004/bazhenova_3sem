#include <iostream>
#include <set>
#include <random>

class State
{
public:
    virtual bool contains(int s) const = 0;
    virtual std::set<int> return_all_elements() const = 0;
};

class DiscreteState :public State {
private:
    int const state;
public:
    DiscreteState(int state) : state(state) {}

    virtual bool contains(int s) const 
    {
        return (s == state);
    }
    virtual std::set<int> return_all_elements() const
    {
        std::set<int> temp;
        temp.insert(state);
        return temp;
    }
};

class SegmentState :public State {
private:
    int const beg, end;

public:
    SegmentState() : beg(0), end(-1) { }
    SegmentState(int beg, int end) : beg(beg), end(end) { }

   virtual bool contains(int s) const {
        return s >= beg && s <= end;
    }
   virtual std::set<int> return_all_elements() const
   {
       std::set<int> temp;
       for (int i = beg; i <= end; i++)
       {
           temp.insert(i);
       }
       return temp;
   }
};

class SetState :public State {
private:
    std::set<int> const states;

public:
    SetState() : states() { }
    SetState(std::set<int> const& src) : states(src) { }

    virtual bool contains(int s) const {
        return (states.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        return states;
    }
};

class UnionState : public State
{
private:
    std::set<int> states;
public:
    UnionState(State& s1, State& s2)
    {
        std::set<int> temp1 = s1.return_all_elements();
        std::set<int> temp2 = s2.return_all_elements();
        states.insert(temp1.begin(), temp1.end());
        states.insert(temp2.begin(), temp2.end());
    }
    virtual bool contains(int s) const {
        return (states.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        return states;
    }
};

class InterceptionState : public State
{
private:
    std::set<int> states;
public:
    InterceptionState(State& s1, State& s2)
    {
        std::set<int> temp1 = s1.return_all_elements();
        std::set<int> temp2 = s2.return_all_elements();
        for (int i = 0;i< temp1.size(); i++)
        {
            std::set<int>::iterator it =temp1.begin();
            for (int k = 0; k < i; k++) it++;
            int temp_value = *it;
            if (temp2.count(temp_value) > 0)
                states.insert(temp_value);
        }
    }
    virtual bool contains(int s) const {
        return (states.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        return states;
    }
};

class DopState : public State
{
private:
    std::set<int> states;
public:
    DopState(State& s1, State& s2) // лежат в s1 не лежат в s2
    {
        std::set<int> temp1 = s1.return_all_elements();
        std::set<int> temp2 = s2.return_all_elements();
        for (int i = 0;i< temp1.size(); i++)
        {
            std::set<int>::iterator it =temp1.begin();
            for (int k = 0; k < i; k++) it++;
            int temp_value = *it;
            if (temp2.count(temp_value) == 0)
                states.insert(temp_value);
        }
    }
    virtual bool contains(int s) const {
        return (states.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        return states;
    }
};

class SegMinusSetState :public State {
private:
    int const beg, end;
    std::set<int> erasing;
public:
    SegMinusSetState(int beg, int end, std::set<int> const& src) : beg(beg), end(end), erasing(src) { }

    virtual bool contains(int s) const {
        return (s >= beg && s <= end) && !(erasing.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        SegmentState temp1(beg, end);
        SetState temp2(erasing);
        DopState temp3(temp1, temp2);
        return temp3.return_all_elements();
    }
};

class SegPlusSetState :public State {
private:
    int const beg, end;
    std::set<int> dop;
public:
    SegPlusSetState(int beg, int end, std::set<int> const& src) : beg(beg), end(end), dop(src) { }

    virtual bool contains(int s) const {
        return (s >= beg && s <= end) || (dop.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        SegmentState temp1(beg, end);
        SetState temp2(dop);
        UnionState temp3(temp1, temp2);
        return temp3.return_all_elements();
    }
};

class SegPlusSetMinusSetState :public State {
private:
    int const beg, end;
    std::set<int> dop;
    std::set<int> erasing;

public:
    SegPlusSetMinusSetState(int beg, int end, std::set<int> const& src, std::set<int> const& src2) : beg(beg), end(end), dop(src),erasing(src2) { }

    virtual bool contains(int s) const {
        return (s >= beg && s <= end) && !(erasing.count(s) > 0) || (dop.count(s)>0);
    }
    virtual std::set<int> return_all_elements() const
    {
        SegmentState temp1(beg, end);
        SetState temp2(dop);
        UnionState temp3(temp1, temp2);
        SetState temp4(erasing);
        DopState temp5(temp3, temp4);
        return temp5.return_all_elements();
    }
};


class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

    float operator()(State& const s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }
};

int main(int argc, const char* argv[]) {
    DiscreteState d(1); //1
    SegmentState s(0, 10); //11
    SetState ss({ 1, 3, 5, 7, 23, 48, 57, 60, 90, 99}); //10
    UnionState u(s, ss); //17
    InterceptionState is(s, ss); //4
    DopState ds(ss, s); //6
    SegMinusSetState smss(4, 12, {9, 11}); //7
    SegPlusSetState spss(15, 21, {7, 86, 53, 44, 61, 57, 100, 3});//15
    SegPlusSetMinusSetState spsmss(87, 100, {23, 17, 92, 18, 69, 47, 51}, {90, 99, 93, 23});//16
    ProbabilityTest pt(10, 0, 100, 100000);
    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(ss) << std::endl;
    return 0;
}






