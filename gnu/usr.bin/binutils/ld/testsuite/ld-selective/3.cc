struct A
{
  virtual void foo();
  virtual void bar();
};

void A::foo() { }			// keep
void A::bar() { }			// loose

struct B : public A
{
  virtual void foo();
};

void B::foo() { }			// keep

void _start() __asm__("_start");	// keep

A a;					// keep
B b;
A *getme() { return &a; }		// keep

void _start()
{
  getme()->foo();
}

// In addition, keep A's virtual table.

// We'll wind up keeping `b' and thus B's virtual table because
// `a' and `b' are both referenced from the constructor function.

extern "C" void __main() { }
