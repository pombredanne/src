// Based on a testcase by Bryan Weston <bryanw@bluemoon.sps.mot.com>
// egcs 1.1 fails to increment count

#include <cstdlib>

struct Base { Base() {} }; // removing the constructor fixes the problem
struct Derived : Base {}; // so does removing the base class

int main() {
  int count = 0;
  Derived* array[1]; // making this Base*[1] does not fix the problem
  array[count++] = new Derived (); // but then new Base() does
  if (count!=1)
    std::abort();
}
