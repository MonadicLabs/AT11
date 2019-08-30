#include <iostream>
using namespace std;

#include <AT11/AT11.h>

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
      cmd->io_output( mm->msg + "\n" );
      cmd->io_ok();
  },

  []( monadic::AT11::Command* cmd, std::vector< std::string > params ){
      if( params.size() )
      {
          cerr << "DFKDLFKLDKFLDFLDFK" << endl;
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

  std::string line;
  uint8_t outbuffer[ 1024 ];
  while(true)
  {
    std::getline( cin, line );
    std::string line2 = line + "\n";
    ati.process_input( (uint8_t*)line2.c_str(), line2.size() );
    std::string at_output;
    while( ati.read_output( at_output ) )
    {
        cerr << at_output;
    }
  }
  return 0;
}
