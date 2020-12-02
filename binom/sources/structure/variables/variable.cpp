#include "binom/includes/structure/variables/variable.h"

// Constructor

binom::Variable::Variable(bool value) : data(new VarType((value)? VarType::bool_true : VarType::bool_false)) {}

binom::Variable::Variable(binom::ui8 value) : data(tryMalloc(2)) {
  data.type[0] = VarType::byte;
  data.bytes[1] = value;
}

binom::Variable::Variable(binom::ui16 value) : data(tryMalloc(3)) {
  data.type[0] = VarType::word;
  *reinterpret_cast<ui16*>(data.bytes + 1) = value;
}

binom::Variable::Variable(binom::ui32 value) : data(tryMalloc(5)) {
  data.type[0] = VarType::dword;
  *reinterpret_cast<ui32*>(data.bytes + 1) = value;
}

binom::Variable::Variable(binom::ui64 value) : data(tryMalloc(9)) {
  data.type[0] = VarType::qword;
  *reinterpret_cast<ui64*>(data.bytes + 1) = value;
}




binom::Variable::Variable(binom::i8 value) : data(tryMalloc(2)) {
  data.type[0] = VarType::byte;
  *reinterpret_cast<i8*>(data.bytes + 1) = value;
}

binom::Variable::Variable(binom::i16 value) : data(tryMalloc(3)) {
  data.type[0] = VarType::word;
  *reinterpret_cast<i16*>(data.bytes + 1) = value;
}

binom::Variable::Variable(binom::i32 value) : data(tryMalloc(5)) {
  data.type[0] = VarType::dword;
  *reinterpret_cast<i32*>(data.bytes + 1) = value;
}

binom::Variable::Variable(binom::i64 value) : data(tryMalloc(9)) {
  data.type[0] = VarType::qword;
  *reinterpret_cast<i64*>(data.bytes + 1) = value;
}

binom::Variable::Variable(binom::f32 value) : data(tryMalloc(5)) {
  data.type[0] = VarType::dword;
  *reinterpret_cast<f32*>(data.bytes + 1) = value;
}

binom::Variable::Variable(binom::f64 value) : data(tryMalloc(9)) {
  data.type[0] = VarType::qword;
  *reinterpret_cast<f64*>(data.bytes + 1) = value;
}




binom::Variable::Variable(const char* str) : data(tryMalloc(9 + strlen(str))) {
  data.type[0] = VarType::byte_array;
  ui64 size = strlen(str);
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;
  memcpy(reinterpret_cast<char*>(data.bytes + 9), str, size);
}

binom::Variable::Variable(const std::string str) : data(tryMalloc(9 + str.length())) {
  data.type[0] = VarType::byte_array;
  ui64 size = str.length();
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;
  memcpy(reinterpret_cast<char*>(data.bytes + 9), str.c_str(), size);
}




binom::Variable::Variable(binom::size_t size, binom::ui8 value) : data(tryMalloc(9 + size)) {
  data.type[0] = VarType::byte_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;
  memset(data.bytes + 9, value, size);
}

binom::Variable::Variable(binom::size_t size, binom::ui16 value) : data(tryMalloc(9 + size*2)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  size_t i = 0;
  for(ui16* it = reinterpret_cast<ui16*>(data.bytes + 9); i < size; (++it, ++i) )
    *it = value;
}

binom::Variable::Variable(binom::size_t size, binom::ui32 value) : data(tryMalloc(9 + size*4)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  size_t i = 0;
  for(ui32* it = reinterpret_cast<ui32*>(data.bytes + 9); i < size; (++it, ++i) )
    *it = value;
}

binom::Variable::Variable(binom::size_t size, binom::ui64 value) : data(tryMalloc(9 + size*8)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  size_t i = 0;
  for(ui64* it = reinterpret_cast<ui64*>(data.bytes + 9); i < size; (++it, ++i) )
    *it = value;

}




binom::Variable::Variable(binom::size_t size, binom::i8 value) : data(tryMalloc(9 + size)) {
  data.type[0] = VarType::byte_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;
  memset(data.bytes + 9, value, size);
}

binom::Variable::Variable(binom::size_t size, binom::i16 value) : data(tryMalloc(9 + size*2)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  size_t i = 0;
  for(i16* it = reinterpret_cast<i16*>(data.bytes + 9); i < size; (++it, ++i) )
    *it = value;

}

binom::Variable::Variable(binom::size_t size, binom::i32 value) : data(tryMalloc(9 + size*4)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  size_t i = 0;
  for(i32* it = reinterpret_cast<i32*>(data.bytes + 9); i < size; (++it, ++i) )
    *it = value;

}

binom::Variable::Variable(binom::size_t size, binom::i64 value) : data(tryMalloc(9 + size*8)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  size_t i = 0;
  for(i64* it = reinterpret_cast<i64*>(data.bytes + 9); i < size; (++it, ++i) )
    *it = value;

}




binom::Variable::Variable(binom::ui8* values, binom::size_t size) : data(tryMalloc(9 + size)) {
  data.type[0] = VarType::byte_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;
  memcpy(data.bytes + 9, values, size);
}

binom::Variable::Variable(binom::ui16* values, binom::size_t size) : data(tryMalloc(9 + size*2)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  ui16* v_it = values;
  for(ui64* it = reinterpret_cast<ui64*>(data.bytes + 9); ui64(v_it - values) < size; (++it, ++v_it) )
    *it = *v_it;

}

binom::Variable::Variable(binom::ui32* values, binom::size_t size) : data(tryMalloc(9 + size*4)) {
  data.type[0] = VarType::dword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  ui32* v_it = values;
  for(ui32* it = reinterpret_cast<ui32*>(data.bytes + 9); ui64(v_it - values) < size; (++it, ++v_it) )
    *it = *v_it;

}

binom::Variable::Variable(binom::ui64* values, binom::size_t size) : data(tryMalloc(9 + size*8)) {
  data.type[0] = VarType::qword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  ui64* v_it = values;
  for(ui64* it = reinterpret_cast<ui64*>(data.bytes + 9); ui64(v_it - values) < size; (++it, ++v_it) )
    *it = *v_it;

}




binom::Variable::Variable(binom::i8* values, binom::size_t size) : data(tryMalloc(9 + size)) {
  data.type[0] = VarType::byte_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;
  memcpy(data.bytes + 9, values, size);
}

binom::Variable::Variable(binom::i16* values, binom::size_t size) : data(tryMalloc(9 + size*2)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  i16* v_it = values;
  for(i64* it = reinterpret_cast<i64*>(data.bytes + 9); ui64(v_it - values) < size; (++it, ++v_it) )
    *it = *v_it;

}

binom::Variable::Variable(binom::i32* values, binom::size_t size) : data(tryMalloc(9 + size*4)) {
  data.type[0] = VarType::dword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  i32* v_it = values;
  for(i32* it = reinterpret_cast<i32*>(data.bytes + 9); ui64(v_it - values) < size; (++it, ++v_it) )
    *it = *v_it;

}

binom::Variable::Variable(binom::i64* values, binom::size_t size) : data(tryMalloc(9 + size*8)) {
  data.type[0] = VarType::qword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = size;

  i64* v_it = values;
  for(i64* it = reinterpret_cast<i64*>(data.bytes + 9); ui64(v_it - values) < size; (++it, ++v_it) )
    *it = *v_it;

}





binom::Variable::Variable(binom::ui8arr array) : data(tryMalloc(9 + array.size())) {
  data.type[0] = VarType::byte_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  ui8* it = data.bytes + 9;
  for(ui8 value : array)
    *it = value;
}

binom::Variable::Variable(binom::ui16arr array) : data(tryMalloc(9 + array.size()*2)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  ui16* it = reinterpret_cast<ui16*>(data.bytes + 9);
  for(ui16 value : array)
    *it = value;
}

binom::Variable::Variable(binom::ui32arr array) : data(tryMalloc(9 + array.size()*4)) {
  data.type[0] = VarType::dword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  ui32* it = reinterpret_cast<ui32*>(data.bytes + 9);
  for(ui32 value : array)
    *it = value;
}

binom::Variable::Variable(binom::ui64arr array) : data(tryMalloc(9 + array.size()*8)) {
  data.type[0] = VarType::qword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  ui64* it = reinterpret_cast<ui64*>(data.bytes + 9);
  for(ui64 value : array)
    *it = value;
}





binom::Variable::Variable(binom::i8arr array) : data(tryMalloc(9 + array.size())) {
  data.type[0] = VarType::byte_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  i16* it = reinterpret_cast<i16*>(data.bytes + 9);
  for(i8 value : array)
    *it = value;
}

binom::Variable::Variable(binom::i16arr array) : data(tryMalloc(9 + array.size()*2)) {
  data.type[0] = VarType::word_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  i16* it = reinterpret_cast<i16*>(data.bytes + 9);
  for(i16 value : array)
    *it = value;
}

binom::Variable::Variable(binom::i32arr array) : data(tryMalloc(9 + array.size()*4)) {
  data.type[0] = VarType::dword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  i32* it = reinterpret_cast<i32*>(data.bytes + 9);
  for(i32 value : array)
    *it = value;
}

binom::Variable::Variable(binom::i64arr array) : data(tryMalloc(9 + array.size()*8)) {
  data.type[0] = VarType::qword_array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  i64* it = reinterpret_cast<i64*>(data.bytes + 9);
  for(i64 value : array)
    *it = value;
}




binom::Variable::Variable(binom::varr array) : data(tryMalloc(9 + array.size()*sizeof(Variable))) {
  data.type[0] = VarType::array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = array.size();
  void** it = reinterpret_cast<void**>(data.bytes + 9);
  for(const Variable& value : array) {
    *it = value.getDataPointer();
    const_cast<Variable&>(value).data.ptr = nullptr;
  }
}

binom::Variable::Variable(binom::obj object) : data(tryMalloc(9 + object.size()*sizeof(NamedVariable))) {
  data.type[0] = VarType::array;
  *reinterpret_cast<ui64*>(data.bytes + 1) = object.size();
  NamedVariable* it = reinterpret_cast<NamedVariable*>(data.bytes + 9);
  for(const NamedVariable& value : object) {
    it->name = value.name;
    it->variable.data.ptr = value.variable.data.ptr;
    const_cast<NamedVariable&>(value).name.ptr = nullptr;
    const_cast<NamedVariable&>(value).variable.data.ptr = nullptr;
  }
}

binom::Variable::Variable(binom::mtrx matrix) : data(tryMalloc(9 + matrix.getNeededMemory())) {
  data.type[0] = VarType::matrix;
  *reinterpret_cast<ui64*>(data.bytes + 1) = matrix.getRowCount();
  {
    VarType* it = data.type + 9;
    for(VarType type : matrix.type_list) {
      *it = type;
      ++it;
    }
    *it = VarType::end;
  }

  {
    void** it = reinterpret_cast<void**>(data.bytes + 10 + matrix.type_list.size());
    for(const Variable& var : matrix.var_list) {
      *it = var.getDataPointer();
      const_cast<Variable&>(var).data.ptr = nullptr;
      ++it;
    }
  }

}

binom::Variable::Variable(binom::Variable&& other) : data(other.data.ptr) {other.data.ptr = nullptr;}
