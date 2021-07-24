#include <iostream>
#include <fstream>
#include <regex>
#include <map>

int main(int argc, char* argv[]) {
    if( argc != 2 ) {
        std::cerr << "Usage: listing file\n";
        return 1;
    }

    const std::string fn = argv[1];
    std::ifstream listing_file(fn);

    // 1) create map with sorted functions and their position in the file
    std::regex function_r("^[[:d:][:xdigit:]]+ <([[:print:]]+)>:$");
    //std::regex asm_r("^[:blank:]+[[:d:][:xdigit:]]+:[:blank:]([[:print:]]+)");
    std::regex asm_r(R"(^[ ]+[[:d:][:xdigit:]]+:\t(\w+)\t(.*)$)");
    std::map<std::string, std::string> functions;
    std::string current_function;
    std::string function_lines;
    size_t lss_line = 0;
    for (std::string line; std::getline(listing_file, line); ++lss_line) {
        std::smatch m;
        if( std::regex_match(line, m, function_r) ) {
            if( !current_function.empty() ) {
                // FIXME: function name isn't unique, could be duplicates with different address
                functions[current_function] = function_lines;
                function_lines.clear();
            }
            current_function = m[1];
        } else if( !current_function.empty() ){
            std::smatch asm_m;
            if( std::regex_match(line, asm_m, asm_r) ) {
                std::string asm_cmd = asm_m[1];
                std::string asm_str = asm_m[2];
                function_lines += "\t| " + asm_cmd + "\t'" + asm_str + "'\n";
            }
        }
    }
    listing_file.close();

    // 2) copy all functions and hide addresses
    auto print_fun = [&](auto it) {
      std::cout << it.first << ":\n";
      std::cout << it.second << "\n";
    };

    for( const auto f: functions ) {
        print_fun(f);
    }
    //print_fun(*functions.find("__vectors"));

    return 0;
}
