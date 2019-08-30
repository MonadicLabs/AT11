#include <iostream>
using namespace std;

#include <AT11/AT11.h>

#include <serial/serial.h>

typedef struct
{
    std::string msg;
} myMessage;

int main( int argc, char** argv )
{

    myMessage msg;
    msg.msg = "";
    monadic::AT11::Interpreter ati;

    //
    monadic::AT11::Command * testCmd = ati.add( "TEST",

                                                [](monadic::AT11::Command* cmd){
            myMessage * mm = (myMessage*)cmd->user_data;
            cmd->io_output( std::string("\r\n") + mm->msg + std::string("\r\n") );
},

            []( monadic::AT11::Command* cmd, std::vector< std::string > params ){
        if( params.size() )
        {
            myMessage * mm = (myMessage*)cmd->user_data;
            mm->msg = params[0];
            cmd->io_ok();
        }
        else
        {
            cmd->io_error();
        }
    });
    testCmd->user_data = &msg;
    //

    serial::Serial s( argv[1], 115200 );

    std::string line;
    uint8_t outbuffer[ 1024 ];
    while(true)
    {
        line = s.read();
        if( line.size() <= 0 )
            continue;
        cerr << "line=" << line << endl;
        ati.process_input( (uint8_t*)line.c_str(), line.size() );
        std::string at_output;
        while( ati.read_output( at_output ) )
        {
            s.write( at_output );
        }
    }
    return 0;
}
