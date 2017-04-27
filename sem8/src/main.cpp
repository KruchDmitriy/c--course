#include <vector>

#include "my_ostream.h"

my_ostream_con cout;                    // console output
my_ostream_file cerr("errors.txt");     // file output
// my_ostream_db dbout(...);               // db output


int main()
{
  std::vector<my_ostream*> stream_ptrs = {&cout, &cerr};//, &dbout};
  for (auto stream_ptr : stream_ptrs)
    (*stream_ptr) << "Hello world on console!" << "\n"
                  << 1 << 2 << 3 << 4 << "\n"
                  << 777.777 <<  "\n";
  return 0;
}
