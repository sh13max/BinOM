#ifndef VARIABLE_H
#define VARIABLE_H

#include "../types.h"

#include "boolean.h"
#include "primitive.h"
#include "buffer_array.h"
#include "array.h"
#include "object.h"
#include "matrix.h"
#include "table.h"

namespace binom {

class Variable {

  union types {
    void* ptr;
    VarType* type;
    byte* bytes;

    Boolean boolean;
    Primitive primitive;
    BufferArray buffer_array;
    Array array;
    Object object;
    Matrix matrix;
    Table table;

    types(void* ptr) : ptr(ptr) {}
  } data;

public:

  Variable() : data(nullptr) {}

  // Bool init
  Variable(bool value);

  // Primitive init

  Variable(ui8 value);
  Variable(ui16 value);
  Variable(ui32 value);
  Variable(ui64 value);

  Variable(i8 value);
  Variable(i16 value);
  Variable(i32 value);
  Variable(i64 value);
  Variable(f32 value);
  Variable(f64 value);

  // Buffer_array init
  Variable(const char* str);
  Variable(const std::string str);

  Variable(size_t size, ui8 value);
  Variable(size_t size, ui16 value);
  Variable(size_t size, ui32 value);
  Variable(size_t size, ui64 value);
  Variable(size_t size, i8 value);
  Variable(size_t size, i16 value);
  Variable(size_t size, i32 value);
  Variable(size_t size, i64 value);

  Variable(ui8* values, size_t size);
  Variable(ui16* values, size_t size);
  Variable(ui32* values, size_t size);
  Variable(ui64* values, size_t size);

  Variable(i8* values, size_t size);
  Variable(i16* values, size_t size);
  Variable(i32* values, size_t size);
  Variable(i64* values, size_t size);

  Variable(ui8arr array);
  Variable(ui16arr array);
  Variable(ui32arr array);
  Variable(ui64arr array);

  Variable(i8arr array);
  Variable(i16arr array);
  Variable(i32arr array);
  Variable(i64arr array);

  // Array
  Variable(varr array);

  // Object
  Variable(obj object);

  // Matrix
  Variable(mtrx matrix);

  // Table
  Variable(tbl table);

  Variable(Variable&& other);
  Variable(Variable& other);

  ~Variable() {if(data.ptr) free(data.ptr);}

  inline void* getDataPointer() const {return data.ptr;}

  inline VarType type() const noexcept             {return (data.type == nullptr)? VarType::invlid_type :*data.type;}
  inline VarTypeClass typeClass() const noexcept   {return (data.type == nullptr)? VarTypeClass::invalid_type :toTypeClass(*data.type);}
  inline bool isNull() const noexcept              {return data.type == nullptr;}

  inline bool isBoolean() const noexcept           {return typeClass() == VarTypeClass::boolean;}
  inline bool isPrimitive() const noexcept         {return typeClass() == VarTypeClass::primitive;}
  inline bool isBufferArray() const noexcept       {return typeClass() == VarTypeClass::buffer_array;}
  inline bool isArray() const noexcept             {return typeClass() == VarTypeClass::array;}
  inline bool isObject() const noexcept            {return typeClass() == VarTypeClass::object;}
  inline bool isMatrix() const noexcept            {return typeClass() == VarTypeClass::matrix;}
  inline bool isTable() const noexcept             {return typeClass() == VarTypeClass::table;}

  inline Boolean& toBoolean() noexcept           {return data.boolean;}
  inline Primitive& toPrimitive() noexcept       {return data.primitive;}
  inline BufferArray& toBufferArray() noexcept   {return data.buffer_array;}
  inline Array& toArray() noexcept               {return data.array;}
  inline Object& toObject() noexcept             {return data.object;}
  inline Matrix& toMatrix() noexcept             {return data.matrix;}
  inline Table& toTable() noexcept               {return data.table;}

  inline ui64 length() {
    return (isBufferArray() || isArray() || isObject() || isMatrix() || isTable())?
          *reinterpret_cast<ui64*>(data.bytes + 1)
        : 1;
  }
};


// Initers or other

struct NamedVariable {
  BufferArray name;
  Variable variable;
};

struct mtrx {
  std::initializer_list<VarType> type_list;
  std::initializer_list<Variable> var_list;

  mtrx(std::initializer_list<VarType> types, std::initializer_list<Variable> vars)
    : type_list(types), var_list(vars)
  {if(!isValid())throw SException(ErrCode::binom_invalid_initer, "Invalid matrix initer");}

  ui64 getNeededMemory() {return type_list.size() + 1 + var_list.size() * PTR_SZ;}
  ui64 getRowCount() {return type_list.size() / var_list.size();}

  private:
  bool isValid() {
    std::initializer_list<VarType>::iterator type(type_list.begin());
    std::initializer_list<Variable>::iterator var(var_list.begin());
    for(;var == var_list.end();(++type, ++var)) {
      if(type == type_list.end()) type = type_list.begin();
      if(var->type() != *type) return false;
    }
    return type == type_list.end();
  }


};

}

#endif