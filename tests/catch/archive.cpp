
#include "catch.hpp"
#include <../core/ngcore.hpp>
using namespace ngcore;
using namespace std;

class CommonBase
{
  public:
  virtual ~CommonBase() {}

  virtual void DoArchive(Archive& archive) { }
};

class SharedPtrHolder : virtual public CommonBase
{
public:
  vector<shared_ptr<string>> names;
  virtual ~SharedPtrHolder()
  { }

  virtual void DoArchive(Archive& archive) {  archive & names; }
};

class PtrHolder : virtual public CommonBase
{
public:
  vector<int*> numbers;
  virtual ~PtrHolder() {}

  virtual void DoArchive(Archive& archive) {  archive & numbers; }
};

class SharedPtrAndPtrHolder : public SharedPtrHolder, public PtrHolder
{
public:
  virtual ~SharedPtrAndPtrHolder() {}
  virtual void DoArchive(Archive& archive)
  {
    SharedPtrHolder::DoArchive(archive);
    PtrHolder::DoArchive(archive);
  }
};

class NotRegisteredForArchive : public SharedPtrAndPtrHolder {};

class OneMoreDerivedClass : public SharedPtrAndPtrHolder {};

static RegisterClassForArchive<CommonBase> regb;
static RegisterClassForArchive<SharedPtrHolder, CommonBase> regsp;
static RegisterClassForArchive<PtrHolder, CommonBase> regp;
static RegisterClassForArchive<SharedPtrAndPtrHolder, SharedPtrHolder, PtrHolder> regspp;
static RegisterClassForArchive<OneMoreDerivedClass, SharedPtrAndPtrHolder> regom;

void testSharedPointer(Archive& in, Archive& out)
{
  SECTION("Same shared ptr")
    {
      static_assert(has_DoArchive<SharedPtrHolder>::value, "");
      SharedPtrHolder holder, holder2;
      holder.names.push_back(make_shared<string>("name"));
      holder2.names = holder.names; // same shared ptr
      out & holder & holder2;
      out.FlushBuffer();
      SharedPtrHolder inholder, inholder2;
      in & inholder & inholder2;
      CHECK(inholder.names.size() == 1);
      CHECK(inholder.names[0] == inholder2.names[0]);
      CHECK(inholder.names[0].use_count() == 3); // one shared ptr is still kept in the archive
      CHECK(*inholder.names[0] == "name");
    }
}

void testPointer(Archive& in, Archive& out)
{
  SECTION("Same pointer")
    {
      PtrHolder holder, holder2;
      holder.numbers.push_back(new int(3));
      holder2.numbers = holder.numbers; // same shared ptr
      out & holder & holder2;
      out.FlushBuffer();
      PtrHolder inholder, inholder2;
      in & inholder & inholder2;
      CHECK(inholder.numbers.size() == 1);
      CHECK(inholder.numbers[0] == inholder2.numbers[0]);
      CHECK(*inholder.numbers[0] == 3);
    }
}

void testMultipleInheritance(Archive& in, Archive& out)
{
  PtrHolder* p = new OneMoreDerivedClass;
  p->numbers.push_back(new int(2));
  auto p2 = dynamic_cast<SharedPtrHolder*>(p);
  p2->names.push_back(make_shared<string>("test"));
  auto sp1 = shared_ptr<PtrHolder>(p);
  auto sp2 = dynamic_pointer_cast<SharedPtrHolder>(sp1);
  auto checkPtr = [] (auto pin, auto pin2)
  {
      CHECK(typeid(*pin) == typeid(*pin2));
      CHECK(typeid(*pin) == typeid(OneMoreDerivedClass));
      CHECK(*pin2->names[0] == "test");
      CHECK(*pin->numbers[0] == 2);
      CHECK(dynamic_cast<SharedPtrAndPtrHolder*>(pin) == dynamic_cast<SharedPtrAndPtrHolder*>(pin2));
      REQUIRE(dynamic_cast<SharedPtrAndPtrHolder*>(pin2) != nullptr);
      CHECK(*dynamic_cast<SharedPtrAndPtrHolder*>(pin2)->numbers[0] == 2);
      CHECK(*pin->numbers[0] == *dynamic_cast<SharedPtrAndPtrHolder*>(pin2)->numbers[0]);
      REQUIRE(dynamic_cast<SharedPtrAndPtrHolder*>(pin) != nullptr);
      CHECK(dynamic_cast<SharedPtrAndPtrHolder*>(pin)->names[0] == pin2->names[0]);
  };
  SECTION("Archive ptrs to leaves of mult. inh.")
    {
      out & p & p2;
      out.FlushBuffer();
      PtrHolder* pin;
      SharedPtrHolder* pin2;
      in & pin & pin2;
      checkPtr(pin, pin2);
    }
  SECTION("Archive shared ptrs to leaves of mult. inh.")
    {
      out & sp1 & sp2;
      out.FlushBuffer();
      shared_ptr<PtrHolder> pin;
      shared_ptr<SharedPtrHolder> pin2;
      in & pin & pin2;
      checkPtr(pin.get(), pin2.get());
    }
  SECTION("Virtual base class")
    {
      CommonBase* b = dynamic_cast<CommonBase*>(p);
      out & b & p;
      PtrHolder* pin;
      CommonBase* bin;
      in & bin & pin;
      checkPtr(pin, dynamic_cast<SharedPtrHolder*>(bin));
    }
}

void testArchive(Archive& in, Archive& out)
{
  SECTION("SharedPtr")
    {
      testSharedPointer(in, out);
    }
  SECTION("Pointer")
    {
      testPointer(in, out);
    }
  SECTION("Multiple inheritance")
    {
      testMultipleInheritance(in, out);
    }
  SECTION("Not registered")
    {
      SharedPtrAndPtrHolder* p = new NotRegisteredForArchive;
      REQUIRE_THROWS(out & p, Catch::Contains("not registered for archive"));
    }
}

TEST_CASE("BinaryArchive")
{
  auto stream = make_shared<stringstream>();
  BinaryOutArchive out(stream);
  BinaryInArchive in(stream);
  testArchive(in, out);
}

TEST_CASE("TextArchive")
{
  auto stream = make_shared<stringstream>();
  TextOutArchive out(stream);
  TextInArchive in(stream);
  testArchive(in, out);
}
