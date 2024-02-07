#include "CControl.h"

int main ( int argc, char * argv [] )
{
    size_t init_size;
    std::string horizontal_line;
    std::string vertical_line;
    std::istream * in_stream = nullptr;
    std::ostream * out_stream = nullptr;
    std::ifstream ifs;
    std::ofstream ofs;
    std::string config_file;

    if ( argc == 1 )
        config_file = "examples/config";
    else if ( argc != 2 )
        return config_parameter_fail();
    else
        config_file = argv [1];

    if ( ! LoadConfig ( init_size, horizontal_line, vertical_line, in_stream, out_stream, ifs, ofs, config_file ) )
        return config_fail();

    CControl control ( init_size, horizontal_line, vertical_line, * in_stream, * out_stream );
    control . Start();
    return 0;
}