#include <iostream>
#include <fstream>
#include <vector>

struct brackets
{
    int position_bracket_one;
    int position_bracket_two;
    brackets(int one = 0, int two = 0) : position_bracket_one(one), position_bracket_two(two)
    {
    }
};

struct file
{
    std::vector<brackets> brackets_stack;
    std::vector<char> file_vector;
    int read_position = 0;

    void check_if_all_brackets_matched()
    {
        for (const auto &i : brackets_stack)
            if (i.position_bracket_two == 0)
                throw 0;
    }

    int search_corresponding_bracket(int position_br_one)
    {
        int stack = 0;
        int i = 0;
        for (i = position_br_one + 1; i < file_vector.size(); i++)
        {
            if (file_vector[i] == '[')
                stack++;
            else if (file_vector[i] == ']' && stack != 0)
                stack--;
            else if (file_vector[i] == ']')
                return i;
        }
    }

    void find_brackets()
    {
        int counter;
        for (const auto &i : file_vector)
        {
            if (i == '[')
                brackets_stack.push_back(brackets(counter, search_corresponding_bracket(counter)));
            counter++;
        }
        check_if_all_brackets_matched();
    }

    file(std::vector<char> file_) : file_vector(file_)
    {
        find_brackets();
    }
    char get_current_instruction()
    {
        return file_vector[read_position];
    }
    brackets find_bracket_one_with_position(int position)
    {
        for (size_t i = 0; i < brackets_stack.size(); i++)
            if (brackets_stack[i].position_bracket_one == position)
            {
                std::swap(brackets_stack[0], brackets_stack[i]);
                return brackets_stack[0];
            }
        throw 1;
    }
    brackets find_bracket_two_with_position(int position)
    {
        for (size_t i = 0; i < brackets_stack.size(); i++)
            if (brackets_stack[i].position_bracket_two == position)
            {
                std::swap(brackets_stack[0], brackets_stack[i]);
                return brackets_stack[0];
            }
        throw 1;
    }
    char move_to_other_bracket()
    {
        if (get_current_instruction() == '[')
            read_position = find_bracket_one_with_position(read_position).position_bracket_two;
        else if (get_current_instruction() == ']')
            read_position = find_bracket_two_with_position(read_position).position_bracket_one;
    }
    void increase_read_position()
    {
        read_position++;
    }
};

struct tape
{
    int estimated_size;
    std::vector<char> tape_vector;
    int read_position = 0;
    tape(int es_size) : estimated_size(es_size), tape_vector(estimated_size, 0)
    {
    }
    void write_cell()
    {
        std::cout << tape_vector[read_position] << std::flush;
    }
    void input_cell()
    {
        std::cin >> tape_vector[read_position];
    }
    void increase_cell_position()
    {
        read_position++;
        if (read_position >= tape_vector.size())
        {
            for (int i = 0; i < 100; i++)
                tape_vector.push_back(0);
        }
    }
    void decrease_cell_position()
    {
        read_position--;
    }
    void increment_cell()
    {
        tape_vector[read_position]++;
    }
    void decrement_cell()
    {
        tape_vector[read_position]--;
    }
};

bool is_correct_character(char i)
{
    switch (i)
    {
    case '+':
    case '-':
    case '>':
    case '<':
    case '.':
    case ',':
    case '[':
    case ']':
        return true;
    default:
        return false;
    }
}

std::vector<char> get_file_as_vector(std::string name)
{
    std::vector<char> tmp;
    std::ifstream file(name);
    while (!(file.eof()))
    {
        std::string line;
        getline(file, line);
        for (const auto &i : line)
        {
            if (is_correct_character(i))
                tmp.push_back(i);
        }
    }
    return tmp;
}

inline void perform_operations(file _file, tape _tape)
{
    while (_file.read_position != _file.file_vector.size())
    {
        char operation = _file.get_current_instruction();
        switch (operation)
        {
        case '>':
            _tape.increase_cell_position();
            break;
        case '<':
            _tape.decrease_cell_position();
            break;
        case '+':
            _tape.increment_cell();
            break;
        case '-':
            _tape.decrement_cell();
            break;
        case '.':
            _tape.write_cell();
            break;
        case ',':
            _tape.input_cell();
            break;
        case '[':
            if (_tape.tape_vector[_tape.read_position] == 0)
                _file.move_to_other_bracket();
            break;
        case ']':
            if (_tape.tape_vector[_tape.read_position] != 0)
                _file.move_to_other_bracket();
            break;
        }
        _file.increase_read_position();
    }
}

int main(int argc, char *argv[])
{
    file _file(get_file_as_vector(std::string(argv[1])));
    tape _tape(_file.file_vector.size());
    perform_operations(_file, _tape);
}