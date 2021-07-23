#include <cctype>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

void usage(const std::string &app)
{
    std::cerr << "Usage:" << std::endl << "\t" << app << " <input file> <output file>" << std::endl;
}

std::string create_variable_name_from(const std::string &from)
{
    std::string variable_name;
    variable_name.reserve(from.size());
    for (auto c : from)
    {
        // Only support alpha/numbers/underscores
        if (c == '_')
        {
            variable_name.append(1, c);
        }
        else if (std::isalnum(c))
        {
            // Prepend an underscore if the first character isn't alphapetical
            if (variable_name.empty() && !std::isalpha(c))
            {
                variable_name.append(1, '_');
            }

            variable_name.append(1, c);
        }
        else
        {
            variable_name.append(1, '_');
        }
    }

    return variable_name;
}

int main(int argc, const char *argv[])
{
    int exit_status = EXIT_FAILURE;

    if (argc != 3)
    {
        std::cerr << "Incorrect number of arguments." << std::endl;
        usage(argv[0]);
    }
    else
    {
        std::ifstream input_file;
        input_file.open(argv[1], std::ios::in | std::ios::binary);
        if (!input_file.is_open())
        {
            std::cerr << "Unable to open input file: " << std::string(argv[1]) << std::endl;
        }
        else
        {
            const std::string output_variable_name = create_variable_name_from(argv[1]);

            std::ofstream output_file;
            output_file.open(argv[2], std::ios::out | std::ios::trunc);
            if (!output_file.is_open())
            {
                std::cerr << "Unable to open output file: " << std::string(argv[2]) << std::endl;
            }
            else 
            {
                const std::streamsize read_buffer_size(32768);
                std::vector<unsigned char> read_buffer;
                read_buffer.resize(read_buffer_size);
                bool output_started = false;

                for(;;)
                {
                    input_file.read(reinterpret_cast<char *>(read_buffer.data()), read_buffer_size);
                    const auto read_amount = input_file.gcount();
                    if (read_amount == 0)
                    {
                        break;
                    }

                    read_buffer.resize(read_amount);

                    if (!output_started)
                    {
                        output_started = true;

                        output_file << "#include <vector>" << std::endl << std::endl;
                        output_file << "const std::vector<unsigned char> " << output_variable_name << std::endl;
                        output_file << "{";  // Note: no std::endl by design.
                    }

                    for (auto i = 0; i < read_buffer.size(); ++i)
                    {
                        // if at the end of a line, add a CR and tab (as well as a comma if this isn't the first character)
                        if (i % 8 == 0)
                        {
                            if (i != 0)
                            {
                                output_file << ",";
                            }

                            output_file << std::endl << "    ";
                        }
                        else
                        {
                            output_file << ", ";
                        }

                        output_file << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(read_buffer[i]);
                    }
                }

                if (output_started)
                {
                    output_file << std::endl;
                    output_file << "};" << std::endl;
                }

                exit_status = EXIT_SUCCESS;
            }
        }
    }

    return exit_status;
}