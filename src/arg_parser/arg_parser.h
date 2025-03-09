#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

class ArgumentParser {
private:
    std::vector<std::string> positional;
    std::string positional_tooltips;
    std::string name;

    std::unordered_map<std::string, std::string> params;
    std::vector<std::string> optional_tooltips;

    std::unordered_map<std::string, std::string> decorator_map;
public:
    ArgumentParser() = default;

    // All arguments with non-positional characteristics (using - or --) are deemed OPTIONAL

    // Necessary arguments
    void add_argument(std::string field){
        params[field] = "";
        positional.push_back(field);
        positional_tooltips += " [" + field + "]";
    }

    // Optional arguments
    void add_argument(std::string field, std::string tooltip, std::string shortform, std::string longform){
        params[field] = "";

        std::string full_tooltip = shortform + " | " + longform + " {" + field + "}" + " : " + tooltip;

        optional_tooltips.push_back(full_tooltip);

        // Add semantic links
        decorator_map[shortform] = field;
        decorator_map[longform] = field;
    }

    void output_help(){
        std::cout << name + positional_tooltips + "[OPTIONS]" << "\n\nAvailable Options:\n";
        for (auto argument : optional_tooltips){
            std::cout << "  " + argument << "\n";
        }
    }

    bool parse_arguments(int argc, char* argv[]){
        std::size_t positional_iterator = 0;
        name = std::string(argv[0]);

        try {
            for (int arg = 1; arg < argc; arg++){
                std::string argument(argv[arg]);

                if (argument == "--help" || argument == "-h"){
                    output_help();
                    return false;
                }

                if (argument[0] == '-'){
                    // Optional
                    params[decorator_map[argument]] = std::string(argv[++arg]);
                } else {
                    // Positional
                    params[positional[positional_iterator++]] = argument;
                }
            }
            if (positional_iterator != positional.size()){
                output_help();
                return false;
            }
            return true;
        } catch (...) {
            output_help();
            return false;
        }
    }

    std::string operator[](std::string field){
        return params[field];
    }
};