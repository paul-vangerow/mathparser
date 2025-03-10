#include "arg_parser/arg_parser.h"

void ArgumentParser::add_argument(std::string field){
    params[field] = "";
    positional.push_back(field);
    positional_tooltips += " [" + field + "]";
}

void ArgumentParser::add_argument(std::string field, std::string tooltip, std::string shortform, std::string longform){
    params[field] = "";

    std::string full_tooltip = shortform + " | " + longform + " {" + field + "}" + " : " + tooltip;

    optional_tooltips.push_back(full_tooltip);

    // Add semantic links
    decorator_map[shortform] = field;
    decorator_map[longform] = field;
}

void ArgumentParser::output_help(){
    std::cout << name + positional_tooltips + " [OPTIONS]" << "\n\nAvailable Options:\n";
    for (auto argument : optional_tooltips){
        std::cout << "  " + argument << "\n";
    }
}

bool ArgumentParser::parse_arguments(int argc, char* argv[]){
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

std::string ArgumentParser::operator[](std::string field){
    return params[field];
}

std::string ArgumentParser::get_input_stream(std::string input_path){
    auto in_path = std::filesystem::absolute(input_path);
    std::stringstream out_buffer;
    if (std::filesystem::exists(in_path)){
        auto in_stream = std::ifstream(in_path);
        out_buffer << in_stream.rdbuf();
    } else {
        output_help();
    }
    return out_buffer.str();
}

std::unique_ptr<std::ofstream> ArgumentParser::get_output_stream(std::string output_path){
    auto out_path = std::filesystem::absolute(output_path);
    if (std::filesystem::exists(out_path)){
        return std::make_unique<std::ofstream>(out_path);;
    } else {
        output_help();
        return nullptr;
    }
}