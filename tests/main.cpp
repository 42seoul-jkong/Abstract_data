#ifdef FT_USE_STL
#include <map>
#include <set>
#include <stack>
#include <vector>
namespace ft = std;
#else
#include <map.hpp>
#include <set.hpp>
#include <stack.hpp>
#include <vector.hpp>
#endif

#include <deque>

#include <cstdlib>
#include <iostream>
#include <string>

#include <cassert>
#include <iterator>
#include <sstream>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096

struct Buffer
{
    int idx;
    char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template <typename T>
class MutantStack : public ft::stack<T>
{
public:
    MutantStack() {}
    MutantStack(const MutantStack<T>& src) { *this = src; }
    MutantStack<T>& operator=(const MutantStack<T>& rhs)
    {
        this->c = rhs.c;
        return *this;
    }
    ~MutantStack() {}

    typedef typename ft::stack<T>::container_type::iterator iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
};

int int_vector()
{
    ft::vector<int> vector_int;
    typedef ft::vector<int>::size_type size_type;

    vector_int.insert(vector_int.end(), rand());
    vector_int.insert(vector_int.begin(), 21 + rand() % 42, rand());
    vector_int.insert(vector_int.begin() + vector_int.size() / 2, vector_int.begin(), vector_int.end());
    vector_int.erase(vector_int.end() - rand() % vector_int.size());
    vector_int.erase(vector_int.begin() + rand() % (vector_int.size() / 3), vector_int.end() - rand() % (vector_int.size() / 3));

    vector_int.resize(2 * vector_int.capacity() + 42);
    vector_int.resize(vector_int.capacity() - 42);

    assert(vector_int.data() == &(*vector_int.begin()));
    assert(vector_int.data() + vector_int.size() == &(*vector_int.end()));

    for (size_type i = size_type(); i < vector_int.size(); i++)
    {
        std::cout << vector_int[i]; // no endl
        vector_int[i] = rand();
    }
    std::cout << std::endl;

    for (size_type i = size_type(); i < vector_int.size(); i++)
    {
        std::cout << vector_int[i]; // no endl
    }
    std::cout << std::endl;

    try
    {
        vector_int.at(vector_int.size());

        // KO
        assert(false);
    }
    catch (...)
    {
        // OK
        assert(true);
    }

    size_type cap = vector_int.capacity();
    vector_int.reserve(cap << 1);
    assert(vector_int.capacity() == cap << 1);

    vector_int.assign(42, 42);
    assert(vector_int.size() == 42);

    std::stringstream ss("0 1 2 3 4 5 6 7 8 9 42");
    std::istream_iterator<int> begin(ss), end;
    vector_int.assign(begin, end);
    assert(vector_int.at(vector_int.size() - 1) == 42);

    ft::vector<int>::size_type size = vector_int.size();
    ft::vector<int> copy = vector_int;
    assert(copy.size() == size);
    assert(copy == vector_int);

    ft::vector<int>().swap(vector_int);
    assert(vector_int.size() == 0);
    assert(copy > vector_int);

    copy = vector_int;
    assert(copy.empty());
    assert(copy == vector_int);

    return 0;
}

int string_vector()
{
    ft::vector<std::string> vector_string;
    typedef ft::vector<std::string>::iterator iterator;

    vector_string.push_back("43");
    vector_string.insert(vector_string.begin(), 6, "42");
    vector_string.push_back("44");

    for (iterator it = vector_string.begin(); it != vector_string.end(); ++it)
    {
        std::cout << *it; // no endl
    }

    vector_string.pop_back();

    std::cout << vector_string.front(); // no endl
    std::cout << vector_string.back();  // no endl
    std::cout << std::endl;

    return 0;
}

int user_struct_vector()
{
    ft::vector<Buffer> vector_buffer;

    for (int i = 0; i < COUNT; i++)
    {
        vector_buffer.push_back(Buffer());
    }

    for (int i = 0; i < COUNT; i++)
    {
        const int idx = rand() % COUNT;
        vector_buffer[idx].idx = idx;
    }
    ft::vector<Buffer>().swap(vector_buffer);

    try
    {
        for (int i = 0; i < COUNT; i++)
        {
            const int idx = rand() % COUNT;
            vector_buffer.at(idx);
            std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        // NORMAL ! :P
    }

    {
        ft::vector<Buffer> copy = vector_buffer;
        copy = vector_buffer;
    }

    {
        ft::vector<Buffer>().swap(vector_buffer);
        assert(vector_buffer.size() == 0);
    }

    return 0;
}

int int_vector_stack()
{
    ft::stack<int> stack_int;

    stack_int.push(int());
    assert(stack_int.size() == 1);
    stack_int.top() = 42;
    assert(stack_int.top() == 42);
    stack_int.pop();
    assert(stack_int.empty());

    return 0;
}

int user_struct_deque_stack()
{
    ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;

    stack_deq_buffer.push(Buffer());
    assert(stack_deq_buffer.size() == 1);
    stack_deq_buffer.top().idx = 42;
    assert(stack_deq_buffer.top().idx == 42);
    stack_deq_buffer.pop();
    assert(stack_deq_buffer.empty());

    return 0;
}

int iter_mutant_stack()
{
    MutantStack<char> iterable_stack;

    for (char letter = 'a'; letter <= 'z'; letter++)
        iterable_stack.push(letter);

    for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
    {
        std::cout << *it;
    }
    std::cout << std::endl;

    return 0;
}

int int_int_map()
{
    const int number = 65536;
    ft::map<int, int> map_int;
    typedef ft::map<int, int>::size_type size_type;
    typedef ft::map<int, int>::iterator iterator;

    iterator hint = map_int.insert(ft::make_pair(42, rand())).first;
    for (int i = 0; i < number; ++i)
    {
        hint = map_int.insert(hint, ft::make_pair(rand(), rand()));
    }

    int sum = map_int.at(42);
    for (int i = 0; i < 10000; i++)
    {
        int access = rand();
        sum += map_int[access];
    }
    std::cout << "should be constant with the same seed: " << sum << ", " << map_int.find(42)->second << std::endl;
    std::cout << map_int.count(42) << " " << map_int.equal_range(42).first->first << " : " << map_int.equal_range(42).first->second << ", " << map_int.equal_range(42).second->first << " : " << map_int.equal_range(42).second->second << std::endl;

    size_type i = size_type();
    for (iterator it = map_int.begin(); it != map_int.end(); ++it)
    {
        std::cout << it->first << it->second; // no endl;
        i++;
    }
    std::cout << std::endl;

    i--;
    map_int.erase(--map_int.end());

    iterator pos = map_int.begin();
    std::advance(pos, map_int.size() / 5);
    i -= std::distance(map_int.begin(), pos);
    map_int.erase(map_int.begin(), pos);

    std::cout << map_int.lower_bound(42)->first << " : " << map_int.lower_bound(42)->second << ", " << map_int.lower_bound(42)->first << " : " << map_int.lower_bound(42)->second << std::endl;

    ft::map<int, int> copy = map_int;
    copy = map_int;
    assert(copy == map_int);

    copy.clear();
    assert(copy.empty());
    assert(map_int > copy);

    ft::map<int, int>().swap(map_int);
    assert(map_int.size() == 0);
    assert(copy == map_int);

    return 0;
}

int int_set()
{
    const int number = 65536;
    ft::set<int> set_int;
    typedef ft::set<int>::size_type size_type;
    typedef ft::set<int>::iterator iterator;

    iterator hint = set_int.insert(42).first;
    for (int i = 0; i < number; ++i)
    {
        hint = set_int.insert(hint, rand());
    }

    std::cout << set_int.count(42) << " " << *set_int.equal_range(42).first << ", " << *set_int.equal_range(42).second << std::endl;

    size_type i = size_type();
    for (iterator it = set_int.begin(); it != set_int.end(); ++it)
    {
        std::cout << *it; // no endl;
        i++;
    }
    std::cout << std::endl;

    i--;
    set_int.erase(--set_int.end());

    iterator pos = set_int.begin();
    std::advance(pos, set_int.size() / 5);
    i -= std::distance(set_int.begin(), pos);
    set_int.erase(set_int.begin(), pos);

    std::cout << *set_int.lower_bound(42) << ", " << *set_int.lower_bound(42) << std::endl;

    ft::set<int> copy = set_int;
    copy = set_int;
    assert(copy == set_int);

    copy.clear();
    assert(copy.empty());
    assert(set_int > copy);

    ft::set<int>().swap(set_int);
    assert(set_int.size() == 0);
    assert(copy == set_int);

    return 0;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./test seed" << std::endl;
        std::cerr << "Provide a seed please" << std::endl;
        std::cerr << "Count value:" << COUNT << std::endl;
        return 1;
    }

    const int seed = atoi(argv[1]);
    srand(seed);

    int result = int();

    result |= int_vector();
    result |= string_vector();
    result |= user_struct_vector();

    result |= int_vector_stack();
    result |= user_struct_deque_stack();
    result |= iter_mutant_stack();

    result |= int_int_map();
    result |= int_set();

    return result;
}
