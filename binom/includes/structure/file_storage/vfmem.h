#ifndef VFMEM_H
#define VFMEM_H

#include "file_io.h"
#include "file_structs.h"

namespace binom {

struct MemoryBlock { ui64 index; ui64 size; };

//! Pointer on bit in BitMap. Can be used as iterator.
struct BitPointer {
  BitMap* bitmap = nullptr;
  ui8 bit = 0;
public:
  BitPointer(BitMap* map) : bitmap(map), bit(0) {}
  BitPointer(BitMap* map, ui8 bit) : bitmap(map), bit(bit) {}
  BitPointer(const BitPointer& other) : bitmap(other.bitmap), bit(other.bit) {}
  BitPointer(const BitPointer&& other) : bitmap(other.bitmap), bit(other.bit) {}

  BitPointer& operator=(const BitPointer& other) {bitmap = other.bitmap; bit = other.bit; return *this;}
  BitPointer& operator=(const BitPointer&& other) {bitmap = other.bitmap; bit = other.bit; return *this;}

  bool operator*() {return (*bitmap).get(bit);}

  bool get() {return (bit == 64)? false : (*bitmap).get(bit);}
  bool set(bool value) {return (bit == 64)? false : (*bitmap).set(bit, value);}

  bool isEnd() {return bit == 64;}
  bool isBegin() {return bit == 0;}

  BitPointer& operator++() {++bit;return *this;}
  BitPointer operator++(int) {BitPointer last(*this);++bit;return last;}
  BitPointer& operator--() {--bit;return *this;}
  BitPointer operator--(int) {BitPointer last(*this);--bit;return last;}
};





//! Virtual File Memory Controller
class VFMemoryController {
public:



private:

  static constexpr ui64 node_segement_size = sizeof(NodeSegmentDescriptor) + 64*sizeof(NodeDescriptor);
  static constexpr ui64 primitive_segment_size = sizeof(PrimitiveSegmentDescriptor) + 64;




  template<typename Descriptor>
  class SegmentList {
  public:

    struct SegmentNode {
      SegmentNode* next = nullptr;
      Descriptor descriptor;
      MemoryBlock block;
    };

    class SegmentIterator {
      SegmentNode* current = nullptr;
    public:
      SegmentIterator(SegmentNode* node) : current(node) {}

      SegmentIterator& operator++() {current = current->next; return *this;}
      SegmentNode& operator*() {return *current;}
      bool operator==(SegmentIterator& other) const {return current == other.current;}
      bool operator!=(SegmentIterator& other) const {return current != other.current;}
    };

    SegmentNode& last() {return *last_segment;}
    SegmentNode& first() {return first_segment;}

    SegmentNode& operator[](ui64 index) {
      for(SegmentNode& node : *this) {
        if(index == 0) return node;
        --index;
      }
      throw SException(ErrCode::binomdb_segment_isnt_exist);
    }

    SegmentNode* insertSegement(MemoryBlock memory_block, Descriptor descriptor) {
      return last_segment = last_segment->next = new SegmentNode{nullptr, descriptor, memory_block};
    }

    SegmentIterator begin() {return &first_segment;}
    SegmentIterator end() {return nullptr;}

  private:
    SegmentNode first_segment;
    SegmentNode* last_segment = &first_segment;
  };

  typedef SegmentList<NodeSegmentDescriptor> NodeSegmentList;
  typedef SegmentList<PrimitiveSegmentDescriptor> PrimitiveSegmentList;
  typedef SegmentList<DataSegmentDescriptor> DataSegmentList;








  class DataMemoryBlocks {
    struct DataBlock {
      bool is_used = false;
      MemoryBlock block{0, 0};
      DataBlock* next = nullptr;
      DataBlock* prev = nullptr;
    };


    MemoryBlock alloc(DataBlock& data_block, ui64 size) {
      if(data_block.block.size == size) {
        data_block.is_used = true;
        return data_block.block;
      }

      DataBlock* new_data_block = new DataBlock{
                                  false,
                                  {data_block.block.index + size,
                                  data_block.block.size - size},
                                  data_block.next, &data_block};
      data_block.block.size = size;
      if(data_block.next) data_block.next->prev = new_data_block;
      else last_block = new_data_block;
      data_block.next = new_data_block;
      data_block.is_used = true;
      return data_block.block;
    }

    void free(DataBlock& data_block) {
      while(!data_block.prev->is_used) {
        data_block.prev->is_used = true;
        DataBlock* prev_block = data_block.prev;
        data_block.prev = data_block.prev->prev;
        if(data_block.prev)data_block.prev->next = &data_block;
        data_block.block = {prev_block->block.index,
                            data_block.block.size + prev_block->block.size};
        delete prev_block;
        if(!data_block.prev) break;
      }

      while(!data_block.next->is_used) {
        data_block.next->is_used = true;
        DataBlock* next_block = data_block.next;
        data_block.next = data_block.next->next;
        if(data_block.next) data_block.next->prev = &data_block;
        data_block.block.size += next_block->block.size;
        delete next_block;
        if(!data_block.next) {
          last_block = &data_block;
          break;
        }
      }

      data_block.is_used = false;
    }


    DataBlock start_block;
    DataBlock* last_block = &start_block;

  public:

    class iterator {
      DataBlock* ptr;
    public:
      iterator(DataBlock* ptr) : ptr(ptr) {}
      iterator(iterator& other) : ptr(other.ptr) {}
      iterator(iterator&& other) : ptr(other.ptr) {}
      iterator& operator=(DataBlock* ptr) {this->ptr = ptr; return *this;}
      iterator& operator++() {ptr = ptr->next; return *this;}
      iterator operator++(int) {iterator other(*this); ptr = ptr->next; return other;}
      iterator& operator--() {ptr = ptr->prev; return *this;}
      iterator operator--(int) {iterator other(*this); ptr = ptr->prev; return other;}
      bool operator==(iterator& other) {return ptr == other.ptr;}
      bool operator!=(iterator& other) {return ptr != other.ptr;}
      DataBlock* operator->() {return ptr;}
      DataBlock& operator*() {return *ptr;}
    };

    void addMemory(ui64 size) {
      if(!last_block->is_used) {
        last_block->block.size += size;
      } else {
        last_block = last_block->next = new DataBlock{false,
                                                      {last_block->block.index + last_block->block.size, size},
                                                      nullptr,
                                                      last_block};
      }
    }

    MemoryBlock findDataBlock(ui64 size) {
      for(DataBlock& data : *this) {
        if(data.is_used || data.block.size < size) continue;
        return alloc(data, size);
      }
      return {0,0};
    }

    void freeBlock(ui64 index) {
      for(DataBlock& data : *this)
        if(data.block.index == index) return free(data);
    }

    iterator begin() {return &start_block;}
    iterator end() {return nullptr;}

  };










  void init();

  NodeSegmentList::SegmentNode& createNodeSegment();
  void loadNodeSegments();
  PrimitiveSegmentList::SegmentNode& createPrimitiveSegment();
  void loadPrimitiveSegments();
  DataSegmentList::SegmentNode& createDataSegment(ui64 size = 4096);
  void loadDataSegments();
  void loadFreeMemoryList();





  public:
    VFMemoryController(std::string filename) : file(std::move(filename)) {init();}
    VFMemoryController(const char* filename) : file(filename) {init();}


    // File info
    ui64 getFileSize() {return file.size();}

    ui64 getDataSegmentsSize();
    ui64 getNodeSegmentsSize();
    ui64 getPrimitiveSegmentsSize();

    ui64 getDataSegmentsCount();
    ui64 getNodeSegmentsCount();
    ui64 getPrimitiveSegmentsCount();


    // NodeDescriptor management
    NodeDescriptor getNodeDescriptor(ui64 index);
    ui64 setNodeDescriptor(ui64 index, NodeDescriptor descriptor);
    ui64 setNodeDescriptor(NodeDescriptor descriptor);
    void freeNodeDescriptor(ui64 index);
    ui64 findFreeNodeDescriptor();


    // Primitive memory mangement
    template<typename Type> Type getPrimitive(ui64 index);
    template<typename Type> ui64 setPrimitive(ui64 index, Type value);
    template<typename Type> ui64 setPrimitive(Type value);
    template<typename Type> void freePrimitive(ui64 index);
    template<typename Type> ui64 findFreePrimitive();


    // Data memory magement




  private:
    FileIO file;

    DBHeader header;
    NodeSegmentList node_segment_list;
    PrimitiveSegmentList primitive_segment_list;
    DataSegmentList data_segment_list;
    DataMemoryBlocks data_memory_map;

  };

  }

  #endif
