// loxpp.cpp: 定义应用程序的入口点。
//

#include <sstream>
#include "loxpp.h"

using namespace std;

int run_file(std::string);
int run_prompt();
int run(std::wistream&);
void print_info();

int main(int argc, char** argv)
{
//	cout << "Hello CMake." << endl;
//	const token t{token_type::IDENTIFIER, "tmp", 1, 1, };
//	cout << t << endl;
    if (argc == 2) {
        return run_file(argv[1]);
    }else if (argc == 1) {
        return run_prompt();
    } else {
        print_info();
    }
	return 0;
}

int run_file(std::string file_name){
    wifstream ifs {file_name};
    if (!ifs){
        return 1;
    }
    return run(ifs);
}

int run_prompt(){
    std::wstring line;
    for (;!std::cin.eof();) {
        std::getline(std::wcin, line);
        wistringstream is{line};
        run(is);
    }
    return 0;
}

int run(std::wistream& is){
    scanner sc{is};
    try {
        for (auto &t : sc.get_tokens()) {
            std::wcout << t << std::endl;
        }
        return 0;
    } catch (std::exception& e){
        for (auto& er : sc.get_tokens()){
            std::wcerr << er <<std::endl;
        }
        return 66;
    }
}

void print_info(){
    std::cout<<"Usage: "<<std::endl;
    std::cout<<"1. loxpp your_script_name.lp"<<std::endl;
    std::cout<<"2. loxpp" << std::endl;
}
