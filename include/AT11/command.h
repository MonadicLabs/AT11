#pragma once

#include <functional>

#include "iobuffer.h"

namespace monadic
{
namespace AT11
{
    class Interpreter;
    class Command
    {

        friend class Interpreter;

    public:
        virtual ~Command()
        {

        }

        std::function<void(Command*)> _queryFunc;
        std::function<void(Command*)> _testFunc;
        std::function<void(Command*, std::vector<std::string>)> _setFunc;
        std::function<void(Command*)> _executeFunc;

        void io_output( const std::string& str )
        {
            if( _iobuffer )
            {
                _iobuffer->write_output( str );
            }
        }

        void io_undefined()
        {
            if( _iobuffer )
            {
                _iobuffer->write_output( "undefined\r\n" );
            }
        }

        void io_ok()
        {
            if( _iobuffer )
            {
                _iobuffer->write_output( "\r\nOK\r\n" );
            }
        }

        void io_error()
        {
            if( _iobuffer )
            {
                _iobuffer->write_output( "\r\nERROR" );
            }
        }

        void * user_data;

    private:
        IOBuffer * _iobuffer;

        Command( IOBuffer * b = nullptr )
            :_iobuffer(b), user_data(nullptr)
        {

        }

        void exec()
        {
            _executeFunc( this );
        }

        void setup( const std::vector< std::string >& params )
        {
            _setFunc( this, params );
        }

        void query()
        {
            _queryFunc( this );
        }

        void test()
        {
            _testFunc( this );
        }

    protected:

    };
}
}
