#ifndef OBJECT_H
#define OBJECT_H

#include "../types.h"
#include "variable.h"

namespace binom {

class Object {
  union types {
      void* ptr;
      VarType* type;
      byte* bytes;
      types(void* ptr) : ptr(ptr) {}
  } data;

  inline ui64& length() const {return *reinterpret_cast<ui64*>(data.bytes + 1);}
  inline ui64 msize() const;

  void mch(size_t new_size);
  void* madd(size_t add_size);
  void msub(size_t sub_size);
  void* maddto(void* to, size_t size);
  void msubfrom(void* from, size_t size);
  void* clone();
  void destroy();

  friend class Variable;
public:
  Object();
  Object(obj object);

  inline bool isEmpty() const {return !length();}
  inline ui64 getMemberCount() const {return *reinterpret_cast<ui64*>(data.bytes + 1);}

  Variable& insert(BufferArray name, Variable var);
  void remove(BufferArray name);

  Variable& getVariable(BufferArray name);
  Variable& operator[](BufferArray name) {return getVariable(name);}


  ObjectIterator begin() const {return reinterpret_cast<ObjectIterator>(data.bytes + 9);}
  ObjectIterator end() const {return reinterpret_cast<ObjectIterator>(data.bytes + msize());}

  Variable& asVar() {return *reinterpret_cast<Variable*>(this);}
};

}

std::ostream& operator<<(std::ostream& os, const binom::Object& object);

#endif
