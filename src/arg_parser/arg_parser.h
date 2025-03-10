#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <sstream>

// --------------------- ArgumentParser ------------------ //
//                                                         //
// Very simple object for creating and parsing command     //
// line arguments. I was feeling lazy and needed an easy   //
// win so there you go. Gross but configurable so I can    //
// add additional settings in the future if I chose to     //
//                                                         //
// ------------------------------------------------------- //

class ArgumentParser {
private:
    // A vector of all positional arguments. Any non-annotated
    // arguments given will be treated as positional and will
    // be matched with the field the iterator is pointing to
    // within this vector.
    std::vector<std::string> positional;

    // Tooltip for positional arguments, these are assumed to
    // be self-evident and don't have an explanation (just 
    // the field is indicated).
    std::string positional_tooltips;

    // Variable to store to store the first command line
    // argument (the executable path) so that a proper help
    // description can be constructed.
    std::string name;

    // A map linking field names to the stored value from the
    // argument parsing, this is to be used to later access the
    // provided arguments.
    std::unordered_map<std::string, std::string> params;

    // A vector of tooltips for the optional arguments, these
    // have a bit more description and are typically displayed
    // with their long and shortform argment decorators.
    std::vector<std::string> optional_tooltips;

    // A map of decorator to field to ensure that the correct
    // field is populated through the invocation of the correct
    // decorator.
    std::unordered_map<std::string, std::string> decorator_map;
public:
    ArgumentParser() = default;

    // All arguments with non-positional characteristics (using - or --) are deemed OPTIONAL

    // Add positional argument. Note all positional arguments will
    // appear in the order invoked with this method.
    //
    // field: The name of the field the argument should populate.
    void add_argument(std::string field);

    // Add optional arguments. These may appear in any order at any
    // point and can not be filled.
    //
    // field: The name of the field the argument should populate
    // tooltip: The description in the help menu.
    // shortform: The shortform decorator (this should always start
    //         with a dash - )
    // longform: The longform decorator (this should always start
    //          with a double dash -- )
    void add_argument(std::string field, std::string tooltip, std::string shortform, std::string longform);

    // Output help (indicate how to call the code as well as what 
    // arguments to input / what they mean)
    void output_help();

    // Actually parse the input arguments (just give the argc and 
    // argv provided to main). Will invoke output_help() if
    // provided CLI arguments are not as expected.
    //
    // argc: The number of arguments
    // argv: The arguments themselves
    //
    // return: True if the parsing was successful, otherwise false.
    bool parse_arguments(int argc, char* argv[]);

    // Overloaded [] operator to make indexing and getting parsed
    // arguments easier (probably wholly unnecessary but this is
    // my code so you can't judge).
    std::string operator[](std::string field);



    // Function named in a way to confuse people because it's actually
    // not a stream but instead a very long string extracted from the
    // file. Does path checking and otherwise does the very boring
    // operation of turning a file into not a file.
    //
    // input_path: The path of the input file.
    //
    // return: A very long string that is all the contents of the file.
    std::string get_input_stream(std::string input_path);

    // A function that is not confusingly named because it does what it
    // sais on the tin, it opens an output stream and hands it to whoever
    // so desires it.
    //
    // output_path: The path of the output file
    //
    // return: R value for the output stream.
    std::unique_ptr<std::ofstream> get_output_stream(std::string output_path);
};