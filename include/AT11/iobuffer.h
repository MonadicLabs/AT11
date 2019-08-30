#pragma once

#include <string>
#include <deque>
#include <vector>

namespace monadic {

namespace AT11 {

class Interpreter;
class IOBuffer
{

    friend class Interpreter;

public:
    IOBuffer( bool echoEnabled = true )
        :_echoEnabled(echoEnabled)
    {

    }

    virtual ~IOBuffer()
    {

    }

    void setEcho( bool v )
    {
        _echoEnabled = v;
    }

    void write_output( const std::string& str )
    {
        _outputBuffer.push_back( str );
    }

    size_t output_size()
    {
        return _outputBuffer.size();
    }

    bool get_output( std::string& str )
    {
        if( _outputBuffer.size() )
        {
            str = _outputBuffer.front();
            _outputBuffer.pop_front();
            return true;
        }
        return false;
    }


private:

    //
    void write_input( const std::string& str )
    {
        if( _echoEnabled )
            write_output( str );
        _inputString += str;
        cerr << "_intputString=" << _inputString << endl;
        updateInput();
    }

    void write_input( uint8_t* buffer, size_t buffer_size )
    {
        std::string strBuffer = (char*)buffer;
        write_input( strBuffer );
    }

    bool get_input( std::string& str )
    {
        if( _inputBuffer.size() )
        {
            str = _inputBuffer.front();
            _inputBuffer.pop_front();
            return true;
        }
        return false;
    }

    size_t input_size()
    {
        return _inputBuffer.size();
    }
    //

    std::deque< std::string > _outputBuffer;
    std::deque< std::string > _inputBuffer;
    std::string _inputString;
    void updateInput()
    {
        std::vector< string > lines = stringToLines( _inputString );
        cerr << "--------" << endl;
        for( std::string l : lines )
        {
            cerr << "*** line = #" << l << "#" << endl;
            // Remove characters from input buffer
            for( int k = 0; k < l.size(); ++k )
            {
                _inputString.erase(0);
            }
            //            interpret_line( l );
            _inputBuffer.push_back( l );
        }
    }

    std::vector<std::string> stringToLines(std::string string)
    {
        std::vector<std::string> result;
        std::string temp;
        int markbegin = 0;
        int markend = 0;

        for (int i = 0; i < string.length(); ++i) {
            if (string[i] == '\r' || string[i] == '\n') {
                markend = i;
                result.push_back(string.substr(markbegin, markend - markbegin));
                markbegin = (i + 1);
            }
        }
        return result;
    }

    bool _echoEnabled;

protected:

};

}

}
