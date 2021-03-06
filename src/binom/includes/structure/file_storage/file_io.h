﻿#ifndef FILE_IO_H
#define FILE_IO_H

#include "../types.h"
#include <cstdio>
#include <string>
#include <mutex>

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#else
  #error "Unsupported filesystem"
#endif

#ifdef _WIN32
  #define fseeko64(stream, offset, origin) _fseeki64(stream, offset, origin);
#endif

namespace binom {

class FileIO {
  fs::path file_path;
  std::FILE* file = nullptr;
  std::mutex mtx;

  ui64 pos();
  bool seek(ui64 pos);
  bool rseek(ui64 rpos);

public:
  FileIO() = default;
  FileIO(std::string filename);
  FileIO(const FileIO& other);
  FileIO(FileIO&& other);
  ~FileIO();

  void close();

  bool open(std::string filename);

  ui64 size();
  static bool isExist(std::string file_path);
  bool isExist();
  bool isOpen();
  bool isEmpty();
  fs::path path() const;

  ui64 resize(ui64 new_size);
  ui64 addSize(ui64 add);
  ui64 subSize(ui64 sub);


  bool write(ui64 index, const void* buffer, ui64 size);
  template<typename Type>
  bool write(ui64 index, const Type& data) {return write (index, &data, sizeof(data));}
  bool write(ui64 index, ByteArray& byte_array);

  ByteArray::iterator write(ui64 index, ByteArray& byte_array, ui64 size);
  ByteArray::iterator write(ui64 index, ByteArray::iterator it, ui64 size);

  bool read(ui64 index, void* buffer, ui64 size);
  template<typename Type>
  bool read(ui64 index, Type& data) {return read(index, &data, sizeof(data));}
  bool read(ui64 index, ByteArray& byte_array);

  ByteArray::iterator read(ui64 index, ByteArray& byte_array, ui64 size);
  ByteArray::iterator read(ui64 index, ByteArray::iterator it, ui64 size);

};

}

#endif
