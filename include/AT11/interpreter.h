#pragma once

#include <vector>
#include <deque>
#include <map>
#include <sstream>

#include <cstddef>
#include <cstdint>

// DEBUG
#include <iostream>
using namespace std;
//

#include "command.h"
#include "iobuffer.h"

namespace monadic {

namespace AT11 {

class Interpreter
{
public:
    Interpreter()
        :_echoEnabled(true)
    {
        _iobuffer.setEcho( true );
        // Insert HELP command by default
//        Command * helpCmd = new Command( &_iobuffer );
//        helpCmd->_executeFunc = [](Command* cmd){ cmd->io_output( "BITE\n" ); };
//        _commands.insert( std::make_pair( "HELP", helpCmd ) );
        add( "HELP", [this](Command* cmd){
            cmd->io_output("Command list:\n\r");
            auto cc = commandList();
            for( auto c : cc )
            {
                cmd->io_output( c + "\r\n" );
            }
        });
    }

    virtual ~Interpreter()
    {

    }

    Command* add( const std::string& cmdName,
                  std::function<void(Command*)> execFunc = [](Command* cmd){ cmd->io_undefined(); },
                  std::function<void(Command*, std::vector<std::string>)> setFunc = [](Command* cmd, std::vector<std::string> params){ cmd->io_undefined(); },
                  std::function<void(Command*)> queryFunc = [](Command* cmd){ cmd->io_undefined(); },
                  std::function<void(Command*)> testFunc = [](Command* cmd){ cmd->io_undefined(); }
    )
    {
        Command * cmd = new Command( &_iobuffer );
        cmd->_executeFunc = execFunc;
        cmd->_setFunc = setFunc;
        cmd->_queryFunc = queryFunc;
        cmd->_testFunc = testFunc;
        _commands.insert( std::make_pair( cmdName, cmd ) );
        return cmd;
    }

size_t output_size()
{
    return _iobuffer.output_size();
}

bool read_output( std::string& output_string )
{
    //        if( _iobuffer.output_size() )
    //        {
    //            output_string = _outputBuffer.front();
    //            _outputBuffer.pop_front();
    //            return true;
    //        }
    //        return false;
    return _iobuffer.get_output( output_string );
}

void process_input( uint8_t* buffer, size_t buffer_size )
{
    _iobuffer.write_input( buffer, buffer_size );
    std::string istr;
    while( _iobuffer.get_input( istr ) )
    {
        cerr << "will interpret line: " << istr << endl;
        interpret_line( istr );
    }
}

std::vector< std::string > commandList()
{
    std::vector< std::string > ret;
    for( auto kv : _commands )
    {
        ret.push_back( kv.first );
    }
    return ret;
}

private:

bool _echoEnabled;

typedef enum
{
    OP_NONE = 0,
    OP_TEST,
    OP_QUERY,
    OP_SETUP,
    OP_EXEC
} OperationType;

//    std::deque< std::string > _outputBuffer;
//    std::string _inputBuffer;
IOBuffer _iobuffer;
std::map< std::string, Command* > _commands;

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void interpret_line( std::string str )
{
    std::string output;
//    cerr << "LINE: #" << str << "#" << endl;
    if( str == "AT" )
    {
        output = "OK\n";
        _iobuffer.write_output( output );
        return;
    }
    else if( str == "ATE0" )
    {
        _iobuffer.setEcho( false );
    }
    else if( str == "ATE1" )
    {
        _iobuffer.setEcho( true );
    }
    else
    {
        std::string subject = "";
        std::string params = "";
        OperationType op = OP_NONE;
        if( str.substr( 0, 3 ) == "AT+" )
        {
            std::string remainder = str.substr( 3, str.size() - 3 );
//            cerr << "rem: #" << remainder << "#" << endl;
            {
                size_t eqPos = remainder.find_first_of( '=' );
                if( eqPos != std::string::npos ) // Does it contain '=' ?
                {
                    subject = remainder.substr( 0, eqPos );
                    // cerr << "TEST_SET_SUBJET=" << subject << endl;
                    eqPos++;
                    params = remainder.substr( eqPos, remainder.size() - eqPos );
//                    cerr << "PARAMS=" << params << endl;
                    if( params == "?" )
                    {
                        // TEST
//                        cerr << "TEST " << subject << endl;
                        op = OP_TEST;
                    }
                    else
                    {
                        // SETUP
//                        cerr << "SETUP " << subject << " PARAMS=" << params << endl;
                        op = OP_SETUP;
                    }
                }
                else
                {
                    // This means EXEC or QUERY
                    // If the last character of remainder is a '?', it's a QUERY
                    if( remainder[ remainder.size() - 1 ] == '?' )
                    {
                        subject = remainder.substr( 0, remainder.size() - 1 );
//                        cerr << "QUERY " << subject << endl;
                        op = OP_QUERY;
                    }
                    else // It's an EXEC
                    {
                        subject = remainder;
//                        cerr << "EXEC " << subject << endl;
                        op = OP_EXEC;
                    }
                }
            }
        }
        else
        {
            _iobuffer.write_output( "\r\nInvalid input " + str + "\r\n" );
        }

        if( subject.size() )
        {
            execute_operation( subject, op, params );
        }

    }
}

void execute_operation( const std::string& commandName, OperationType op, const std::string& commandParams = "" )
{
    if( _commands.find( commandName ) != _commands.end() )
    {
        Command * cmd = _commands[ commandName ];

        // Found the command
        switch( op )
        {
        case OP_EXEC:
        {
            cmd->exec();
            break;
        }

        case OP_SETUP:
        {
            std::vector< std::string > prms = split( commandParams, ',' );
            cmd->setup( prms );
        }

        default:
            break;
        }
    }
    else
    {
        _iobuffer.write_output( "\r\nUnknown command " + commandName + "\r\n" );
    }
}

protected:

};

}
}
