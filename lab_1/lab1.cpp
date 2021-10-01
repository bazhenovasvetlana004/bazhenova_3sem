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
        std::set<int>::iterator it = temp1.begin();
        for (int i = 0;i< temp1.size(); i++) 
        {
            int temp_value = *it;
            if (temp2.count(temp_value) > 0)
                states.insert(temp_value);
            it++;
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
        std::set<int>::iterator it = temp1.begin();
        for (int i = 0;i< temp1.size(); i++)
        {
            int temp_value = *it;
            if (temp2.count(temp_value) == 0)
                states.insert(temp_value);
            it++;
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
    std::set<int> states;
public:
    SegMinusSetState(State& segment, State& setstate)
    {
        DopState temp(segment, setstate);
        states = temp.return_all_elements();
    }
    virtual bool contains(int s) const {
        return (states.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        return states;
    }
};

class SegPlusSetState :public State {
private:
    std::set<int> states;
public:
    SegPlusSetState(State& segment, State& setstate)
    {
        UnionState temp(segment, setstate);
        states = temp.return_all_elements();
    }
    virtual bool contains(int s) const {
        return (states.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        return states;
    }
};

class SegPlusSetMinusSetState :public State {
private:
    std::set<int> states;
public:
    SegPlusSetMinusSetState(State& segment, State& setstate1, State& setstate2)
    {
        UnionState temp1(segment, setstate1);
        DopState temp2(temp1, setstate2);
        states = temp2.return_all_elements();
    }
    virtual bool contains(int s) const {
        return (states.count(s) > 0);
    }
    virtual std::set<int> return_all_elements() const
    {
        return states;
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
    SegmentState smss1(4, 12);
    SetState smss2({ 9, 11 });
    SegMinusSetState smss (smss1, smss2); //7
    SegmentState spss1(15, 21);
    SetState spss2({7, 86, 53, 44, 61, 57, 100, 3});
    SegPlusSetState spss(spss1, spss2);//15
    SegmentState spsmss1(87, 100);
    SetState spsmss2({ 23, 17, 92, 18, 69, 47, 51 });
    SetState spsmss3({ 90, 99, 93, 23 });
    SegPlusSetMinusSetState spsmss(spsmss1, spsmss2, spsmss3);//16
    ProbabilityTest pt(10, 0, 100, 100000);
    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(ss) << std::endl;
    return 0;
}
