#include <stddef.h>
#include <iostream>

// YOUR CODE (no global variables!)

#include <cstring>

class Mikrosha;
class Program;
class Instruction;
class UnaryIntInstruction;
class BinaryIntInstruction;
class MovInstruction;
class IncInstruction;
class DecInstruction;
class JumpFalseInstruction;
class JumpInstruction;


class Mikrosha
{
public:
    int & registers(size_t);
    void run(const Program &);

private:
    int _registers[20] {0};

    // YOUR CODE (no public fields!)
public:
    int & address();

private:
    int _address = 0;
};

class Instruction
{
public:
    virtual void run(Mikrosha &) const = 0;

    // YOUR CODE (no public fields!)
    virtual ~Instruction();
};

class Program
{
public:
    Program();
    ~Program();
    Program(const Program &) = delete;
    Program & operator= (const Program &) = delete;

    size_t size() const;
    const Instruction & operator[] (int) const;
    void add(Instruction &);

private:
    const Instruction **_instructions;
    size_t _size;

    // YOUR CODE (no public fields!)
    size_t _real_size;
};

class BinaryIntInstruction: public Instruction
{
public:
    BinaryIntInstruction(int, int);

    void run(Mikrosha &) const override = 0;

    int first() const;
    int second() const;

private:
    int _first;
    int _second;
};

class UnaryIntInstruction: public Instruction
{
public:
    UnaryIntInstruction(int);

    void run(Mikrosha &) const override = 0;

    int first() const;

private:
    int _first;

    // YOUR CODE (no public fields!)
};

class MovInstruction: public BinaryIntInstruction
{
public:
    MovInstruction(int, int);

    void run(Mikrosha & obj) const;
};

class IncInstruction: public BinaryIntInstruction
{
public:
    IncInstruction(int, int);

    // YOUR CODE
    void run(Mikrosha &obj) const;
};

class DecInstruction: public BinaryIntInstruction
{
public:
    DecInstruction(int, int);

    // YOUR CODE
    void run(Mikrosha &) const;
};

class JumpFalseInstruction: public BinaryIntInstruction
{
public:
    JumpFalseInstruction(int, int);

    // YOUR CODE
    void run(Mikrosha &) const;
};

class JumpInstruction: public UnaryIntInstruction
{
public:
    JumpInstruction(int);

    // YOUR CODE
    void run(Mikrosha &) const;
};

// YOUR CODE

Instruction::~Instruction() {}


Program::Program()
: _instructions(nullptr)
, _size(0)
, _real_size(10)
{
    auto arr = new Instruction*[_real_size];
    _instructions = const_cast<const Instruction **>(arr);
}

Program::~Program()
{
    delete [] _instructions;
}

void Program::add(Instruction & obj)
{
    if (_size >= _real_size)
    {
        size_t old_size = _real_size;
        _real_size *= 2;
        auto arr = new Instruction*[_real_size];
        memcpy(arr, _instructions, sizeof(Instruction*) * old_size);
        delete [] _instructions;
        _instructions = const_cast<const Instruction **>(arr);
    }

    _instructions[_size++] = &obj;
}

// YOUR CODE


size_t Program::size() const
{
    return _size;
}

const Instruction & Program::operator[] (int i) const
{
    return *(_instructions[i]);
}


int & Mikrosha::registers(size_t i)
{
    return _registers[i];
}


BinaryIntInstruction::BinaryIntInstruction(int f, int s)
: _first(f), _second(s) {}

int BinaryIntInstruction::first() const
{
    return _first;
}

int BinaryIntInstruction::second() const
{
    return _second;
}


UnaryIntInstruction::UnaryIntInstruction(int f) : _first(f) {}

int UnaryIntInstruction::first() const
{
    return _first;
}

IncInstruction::IncInstruction(int f, int s)
: BinaryIntInstruction(f, s) {}

void IncInstruction::run(Mikrosha & obj) const
{
    obj.registers(first()) += obj.registers(second());
    obj.address()++;
}


DecInstruction::DecInstruction(int f, int s)
: BinaryIntInstruction(f, s) {}

void DecInstruction::run(Mikrosha & obj) const
{
    obj.registers(first()) -= obj.registers(second());
    obj.address()++;
}


MovInstruction::MovInstruction(int f, int s) : BinaryIntInstruction(f, s) {}

void MovInstruction::run(Mikrosha & obj) const
{
    obj.registers(first()) = obj.registers(second());
    obj.address()++;
}


JumpInstruction::JumpInstruction(int f) : UnaryIntInstruction(f) {}

void JumpInstruction::run(Mikrosha & obj) const
{
    obj.address() = first();
}


JumpFalseInstruction::JumpFalseInstruction(int f, int s)
: BinaryIntInstruction(f, s) {}

void JumpFalseInstruction::run(Mikrosha & obj) const
{
    if (obj.registers(first()) == 0)
    {
        obj.address() = second();
    }
    else
    {
        obj.address()++;
    }
}


void Mikrosha::run(const Program & program)
{
    _address = 0;

    while (_address >= 0 && static_cast<size_t>(_address) < program.size())
    {
        program[_address].run(*this);
    }
}

int & Mikrosha::address()
{
    return _address;
}

int main()
{
    // YOUR CODE

    int num1, num2;

    std::cin >> num1 >> num2;

    Mikrosha m;

    Program p;

    // build program

    m.registers(2) = num1;
    m.registers(3) = num2;
    m.registers(18) = 1;

    auto **arr = new Instruction*[12];

    arr[0] = new DecInstruction(19, 19);
    arr[1] = new MovInstruction(1, 19);
    arr[2] = new MovInstruction(17, 2);
    arr[3] = new DecInstruction(17, 19);
    arr[4] = new JumpFalseInstruction(17, -1);
    arr[5] = new MovInstruction(17, 3);
    arr[6] = new DecInstruction(17, 19);
    arr[7] = new JumpFalseInstruction(17, -1);
    arr[8] = new IncInstruction(1, 2);
    arr[9] = new DecInstruction(3, 18);
    arr[10] = new JumpFalseInstruction(3, -1);
    arr[11] = new JumpInstruction(8);

    for (int i = 0; i < 12; i++)
    {
        p.add(*(arr[i]));
    }

    m.run(p);

    std::cout << m.registers(1);

    for (int i = 0; i < 12; i++)
    {
        delete arr[i];
    }

    delete [] arr;

    return 0;
}

