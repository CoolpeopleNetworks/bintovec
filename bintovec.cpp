#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <argparse.hpp>

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

    argparse::ArgumentParser program(argv[0]);

    program.add_argument("input_filename")
        .help("The input file to turn into a C++ vector<>");

    program.add_argument("output_filename")
        .help("The resulting files containing the C++ representation of the input file (example.hpp/example.cpp)");

    program.add_argument("-n", "--name")
        .help("Specifies the C++ variable name that is used in the output file")
        .default_value(std::optional<std::string>())
        .action([](const std::string& value) { return std::optional<std::string>(value); });

    program.add_argument("-ns", "--namespace")
        .help("Specifies the C++ namespace the resulting vector will be placed into")
        .default_value(std::optional<std::string>())
        .action([](const std::string& value) { return std::optional<std::string>(value); });

    try 
    {
        program.parse_args(argc, argv);

        auto input_filename = program.get<std::string>("input_filename");

        std::string variable_name;
        auto specified_variable_name = program.get<std::optional<std::string>>("-n");
        if (specified_variable_name.has_value())
        {
            variable_name = create_variable_name_from(specified_variable_name.value());
        }
        else 
        {
            // If no variable name was specified, just use the filename
            variable_name = create_variable_name_from(input_filename);
        }

        auto ns = program.get<std::optional<std::string>>("-ns");

        std::ifstream input_file;
        input_file.open(input_filename, std::ios::in | std::ios::binary);
        if (!input_file.is_open())
        {
            std::cerr << "Unable to open input file: " << std::string(input_filename) << std::endl;
        }
        else
        {
            auto output_filename = program.get<std::string>("output_filename");

            std::ofstream output_file;
            output_file.open(output_filename + ".cpp", std::ios::out | std::ios::trunc);
            if (!output_file.is_open())
            {
                std::cerr << "Unable to open output file: " << output_filename + ".cpp" << std::endl;
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

                    read_buffer.resize(size_t(read_amount));

                    if (!output_started)
                    {
                        output_started = true;

                        output_file << "// Generated file - Do Not Modify" << std::endl;
                        output_file << "#include <vector>" << std::endl << std::endl;

                        if (ns.has_value())
                        {
                            output_file << "namespace " << ns.value() << std::endl;
                            output_file << "{" << std::endl;
                        }

                        output_file << (ns.has_value() ? "    " : "") << "const std::vector<unsigned char> " << variable_name << " =" << std::endl;
                        output_file << (ns.has_value() ? "    " : "") << "{";  // Note: no std::endl by design.
                    }

                    for (size_t i = 0; i < read_buffer.size(); ++i)
                    {
                        // if at the end of a line, add a CR and tab (as well as a comma if this isn't the first character)
                        if (i % 8 == 0)
                        {
                            if (i != 0)
                            {
                                output_file << ",";
                            }

                            output_file << std::endl << (ns.has_value() ? "    " : "") << "    ";
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
                    output_file << (ns.has_value() ? "    " : "") << "};" << std::endl;

                    if (ns.has_value())
                    {
                        output_file << "}" << std::endl;
                    }

                    // Write the header
                    std::ofstream header_file;
                    header_file.open(output_filename + ".hpp", std::ios::out | std::ios::trunc);
                    if (!header_file.is_open())
                    {
                        std::cerr << "Unable to open output file: " << output_filename + ".cpp" << std::endl;
                    }
                    else
                    {
                        header_file << "// Generated file - Do Not Modify" << std::endl;
                        header_file << "#include <vector>" << std::endl << std::endl;;

                        if (ns.has_value()) 
                        {
                            header_file << "namespace " << ns.value() << std::endl;
                            header_file << "{" << std::endl;
                        }

                        header_file << (ns.has_value() ? "    " : "") << "extern const std::vector<unsigned char> " << variable_name << ";" << std::endl;

                        if (ns.has_value()) 
                        {
                            header_file << "}" << std::endl;
                        }

                    }
                }

                exit_status = EXIT_SUCCESS;
            }
        }
    }
    catch (const std::runtime_error& err) 
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
    }

    return exit_status;
}
