#include "read.hpp"

void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " [-d] [-f output] input\n"
        << "\t-d         debug mode (output unknown fields)\n"
        << "\t-f output  write output to file\n"
        << "\t-h         display this help message";
}

int main(int argc, char* argv[])
{
    bool debug = false;
    std::string input;
    std::string output = "-1";
    if (argc < 2)
    {
        show_usage(argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help"))
        {
            show_usage(argv[0]);
            return 0;
        } else if (arg == "-d")
            debug = true;
        else if (arg == "-f")
        {
            if (i + 1 < argc)
                output = argv[++i];
            else {
                  std::cerr << "-f requires one argument." << std::endl;
                return 1;
            }
        } else {
            input = argv[i];
        }
    }
    return to_text(input, output, debug);
    //std::clog << "Conversion returned " << to_text(input, output, debug);
    //return 0;
}
