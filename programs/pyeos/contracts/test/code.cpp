#include <eoslib/db.h>
#include <eoslib/types.hpp>
#include <eoslib/message.h>
#include <eoslib/print.hpp>

extern "C"{
   int pythonLoad(uint64_t name,void *codeptr,int codesize);
   int pythonCall(uint64_t name,uint64_t func,void* argsptr,int argssize);
}

int strlen(char *code){
   int length = 0;
   while(code[length++]);
   return length;
}

using namespace eos;

void read_length(char *raw_array,int& length,int& length_size){
   uint64_t v = 0; char b = 0; uint8_t by = 0;
   int index = 0;
   do {
       b = raw_array[index++];
       v |= uint32_t(uint8_t(b) & 0x7f) << by;
       by += 7;
   } while( uint8_t(b) & 0x80 );
   length_size = index;
   length = v;
}

extern "C" {
    void init()  {
       int size = 1024;
       int a[size];
       readMessage(a,sizeof(a));
    }
    void test(int size)  {
       int a[size];
       readMessage(a,sizeof(a));
    }

    /// The apply method implements the dispatch of events to this contract
    void apply( uint64_t code, uint64_t action ) {
       auto a = N(-);
       if( code == N(test) ) {
          if( action == N(test) ) {
             requireAuth(N(test));
             char *code = "def hello():\n" \
             "    print('hello,world')";
             pythonLoad(N(hello),code,strlen(code));
             pythonCall(N(hello),N(hello),0,0);
          }
       }
    }
}
