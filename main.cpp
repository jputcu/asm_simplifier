#include <iostream>
#include <fstream>
#include <regex>
#include <map>


static std::string SimplifyAsm(const std::string line) {
    static const std::regex asm_r(R"(^[ ]+[[:d:][:xdigit:]]+:\t(\w+)(\t(.*))?$)");
    std::smatch asm_m;
    if( std::regex_match(line, asm_m, asm_r) ) {
        const std::string asm_cmd = asm_m[1];
        const std::string asm_str = asm_m[2];
        return asm_cmd;
    } else
        return ""; //line;
}

int main(int argc, char* argv[]) {
    if( argc != 2 ) {
        std::cerr << "Usage: listing file\n";
        return 1;
    }

    const std::string fn = argv[1];
    std::ifstream listing_file(fn);

    // 1) create map with sorted functions and their position in the file
    std::regex function_r("^[[:d:][:xdigit:]]+ <([[:print:]]+)>:$");
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
            auto simplifed = SimplifyAsm(line);
            if( !simplifed.empty() )
                function_lines += "\t| " + simplifed + '\n';
        }
    }
    listing_file.close();

    // 2) dump simplified output to stdout
    auto print_fun = [&](auto it) {
      std::cout << it.first << ":\n";
      std::cout << it.second << "\n";
    };

    for( const auto f: functions ) {
        print_fun(f);
    }

    return 0;
}
