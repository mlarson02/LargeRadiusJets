#include "hls_signal_handler.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_directio.h"
#include "hls_stream.h"
using namespace std;

namespace hls::sim
{
  template<size_t n>
  struct Byte {
    unsigned char a[n];

    Byte()
    {
      for (size_t i = 0; i < n; ++i) {
        a[i] = 0;
      }
    }

    template<typename T>
    Byte<n>& operator= (const T &val)
    {
      std::memcpy(a, &val, n);
      return *this;
    }
  };

  struct SimException : public std::exception {
    const std::string msg;
    const size_t line;
    SimException(const std::string &msg, const size_t line)
      : msg(msg), line(line)
    {
    }
  };

  void errExit(const size_t line, const std::string &msg)
  {
    std::string s;
    s += "ERROR";
//  s += '(';
//  s += __FILE__;
//  s += ":";
//  s += std::to_string(line);
//  s += ')';
    s += ": ";
    s += msg;
    s += "\n";
    fputs(s.c_str(), stderr);
    exit(1);
  }
}


namespace hls::sim
{
  struct Buffer {
    char *first;
    Buffer(char *addr) : first(addr)
    {
    }
  };

  struct DBuffer : public Buffer {
    static const size_t total = 1<<10;
    size_t ufree;

    DBuffer(size_t usize) : Buffer(nullptr), ufree(total)
    {
      first = new char[usize*ufree];
    }

    ~DBuffer()
    {
      delete[] first;
    }
  };

  struct CStream {
    char *front;
    char *back;
    size_t num;
    size_t usize;
    std::list<Buffer*> bufs;
    bool dynamic;

    CStream() : front(nullptr), back(nullptr),
                num(0), usize(0), dynamic(true)
    {
    }

    ~CStream()
    {
      for (Buffer *p : bufs) {
        delete p;
      }
    }

    template<typename T>
    T* data()
    {
      return (T*)front;
    }

    template<typename T>
    void transfer(hls::stream<T> *param)
    {
      while (!empty()) {
        param->write(*(T*)nextRead());
      }
    }

    bool empty();
    char* nextRead();
    char* nextWrite();
  };

  bool CStream::empty()
  {
    return num == 0;
  }

  char* CStream::nextRead()
  {
    assert(num > 0);
    char *res = front;
    front += usize;
    if (dynamic) {
      if (++static_cast<DBuffer*>(bufs.front())->ufree == DBuffer::total) {
        if (bufs.size() > 1) {
          bufs.pop_front();
          front = bufs.front()->first;
        } else {
          front = back = bufs.front()->first;
        }
      }
    }
    --num;
    return res;
  }

  char* CStream::nextWrite()
  {
    if (dynamic) {
      if (static_cast<DBuffer*>(bufs.back())->ufree == 0) {
        bufs.push_back(new DBuffer(usize));
        back = bufs.back()->first;
      }
      --static_cast<DBuffer*>(bufs.back())->ufree;
    }
    char *res = back;
    back += usize;
    ++num;
    return res;
  }

  std::list<CStream> streams;
  std::map<char*, CStream*> prebuilt;

  CStream* createStream(size_t usize)
  {
    streams.emplace_front();
    CStream &s = streams.front();
    {
      s.dynamic = true;
      s.bufs.push_back(new DBuffer(usize));
      s.front = s.bufs.back()->first;
      s.back = s.front;
      s.num = 0;
      s.usize = usize;
    }
    return &s;
  }

  template<typename T>
  CStream* createStream(hls::stream<T> *param)
  {
    CStream *s = createStream(sizeof(T));
    {
      s->dynamic = true;
      while (!param->empty()) {
        T data = param->read();
        memcpy(s->nextWrite(), (char*)&data, sizeof(T));
      }
      prebuilt[s->front] = s;
    }
    return s;
  }

  template<typename T>
  CStream* createStream(T *param, size_t usize)
  {
    streams.emplace_front();
    CStream &s = streams.front();
    {
      s.dynamic = false;
      s.bufs.push_back(new Buffer((char*)param));
      s.front = s.back = s.bufs.back()->first;
      s.usize = usize;
      s.num = ~0UL;
    }
    prebuilt[s.front] = &s;
    return &s;
  }

  CStream* findStream(char *buf)
  {
    return prebuilt.at(buf);
  }
}
class AESL_RUNTIME_BC {
  public:
    AESL_RUNTIME_BC(const char* name) {
      file_token.open( name);
      if (!file_token.good()) {
        cout << "Failed to open tv file " << name << endl;
        exit (1);
      }
      file_token >> mName;//[[[runtime]]]
    }
    ~AESL_RUNTIME_BC() {
      file_token.close();
    }
    int read_size () {
      int size = 0;
      file_token >> mName;//[[transaction]]
      file_token >> mName;//transaction number
      file_token >> mName;//pop_size
      size = atoi(mName.c_str());
      file_token >> mName;//[[/transaction]]
      return size;
    }
  public:
    fstream file_token;
    string mName;
};
using hls::sim::Byte;
extern "C" void top(volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, Byte<4>*, volatile void *, volatile void *);
extern "C" void apatb_top_hw(volatile void * __xlx_apatb_param_seedValues_0, volatile void * __xlx_apatb_param_seedValues_1, volatile void * __xlx_apatb_param_seedValues_2, volatile void * __xlx_apatb_param_seedValues_3, volatile void * __xlx_apatb_param_seedValues_4, volatile void * __xlx_apatb_param_seedValues_5, volatile void * __xlx_apatb_param_seedValues_6, volatile void * __xlx_apatb_param_seedValues_7, volatile void * __xlx_apatb_param_seedValues_8, volatile void * __xlx_apatb_param_seedValues_9, volatile void * __xlx_apatb_param_seedValues_10, volatile void * __xlx_apatb_param_seedValues_11, volatile void * __xlx_apatb_param_inputObjectValues_0_0, volatile void * __xlx_apatb_param_inputObjectValues_0_1, volatile void * __xlx_apatb_param_inputObjectValues_0_2, volatile void * __xlx_apatb_param_inputObjectValues_0_3, volatile void * __xlx_apatb_param_inputObjectValues_1_0, volatile void * __xlx_apatb_param_inputObjectValues_1_1, volatile void * __xlx_apatb_param_inputObjectValues_1_2, volatile void * __xlx_apatb_param_inputObjectValues_1_3, volatile void * __xlx_apatb_param_inputObjectValues_2_0, volatile void * __xlx_apatb_param_inputObjectValues_2_1, volatile void * __xlx_apatb_param_inputObjectValues_2_2, volatile void * __xlx_apatb_param_inputObjectValues_2_3, volatile void * __xlx_apatb_param_inputObjectValues_3_0, volatile void * __xlx_apatb_param_inputObjectValues_3_1, volatile void * __xlx_apatb_param_inputObjectValues_3_2, volatile void * __xlx_apatb_param_inputObjectValues_3_3, volatile void * __xlx_apatb_param_inputObjectValues_4_0, volatile void * __xlx_apatb_param_inputObjectValues_4_1, volatile void * __xlx_apatb_param_inputObjectValues_4_2, volatile void * __xlx_apatb_param_inputObjectValues_4_3, volatile void * __xlx_apatb_param_inputObjectValues_5_0, volatile void * __xlx_apatb_param_inputObjectValues_5_1, volatile void * __xlx_apatb_param_inputObjectValues_5_2, volatile void * __xlx_apatb_param_inputObjectValues_5_3, volatile void * __xlx_apatb_param_inputObjectValues_6_0, volatile void * __xlx_apatb_param_inputObjectValues_6_1, volatile void * __xlx_apatb_param_inputObjectValues_6_2, volatile void * __xlx_apatb_param_inputObjectValues_6_3, volatile void * __xlx_apatb_param_inputObjectValues_7_0, volatile void * __xlx_apatb_param_inputObjectValues_7_1, volatile void * __xlx_apatb_param_inputObjectValues_7_2, volatile void * __xlx_apatb_param_inputObjectValues_7_3, volatile void * __xlx_apatb_param_inputObjectValues_8_0, volatile void * __xlx_apatb_param_inputObjectValues_8_1, volatile void * __xlx_apatb_param_inputObjectValues_8_2, volatile void * __xlx_apatb_param_inputObjectValues_8_3, volatile void * __xlx_apatb_param_inputObjectValues_9_0, volatile void * __xlx_apatb_param_inputObjectValues_9_1, volatile void * __xlx_apatb_param_inputObjectValues_9_2, volatile void * __xlx_apatb_param_inputObjectValues_9_3, volatile void * __xlx_apatb_param_inputObjectValues_10_0, volatile void * __xlx_apatb_param_inputObjectValues_10_1, volatile void * __xlx_apatb_param_inputObjectValues_10_2, volatile void * __xlx_apatb_param_inputObjectValues_10_3, volatile void * __xlx_apatb_param_inputObjectValues_11_0, volatile void * __xlx_apatb_param_inputObjectValues_11_1, volatile void * __xlx_apatb_param_inputObjectValues_11_2, volatile void * __xlx_apatb_param_inputObjectValues_11_3, volatile void * __xlx_apatb_param_outputJetValues_0, volatile void * __xlx_apatb_param_outputJetValues_1) {
using hls::sim::createStream;
  // Collect __xlx_inputObjectValues_0_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_0_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_0_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_0)[i]);
}
  int __xlx_size_param_inputObjectValues_0_0 = 32;
  int __xlx_offset_param_inputObjectValues_0_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_0_0 = 0*4;
  // Collect __xlx_inputObjectValues_0_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_0_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_0_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_1)[i]);
}
  int __xlx_size_param_inputObjectValues_0_1 = 32;
  int __xlx_offset_param_inputObjectValues_0_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_0_1 = 0*4;
  // Collect __xlx_inputObjectValues_0_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_0_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_0_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_2)[i]);
}
  int __xlx_size_param_inputObjectValues_0_2 = 32;
  int __xlx_offset_param_inputObjectValues_0_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_0_2 = 0*4;
  // Collect __xlx_inputObjectValues_0_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_0_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_0_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_3)[i]);
}
  int __xlx_size_param_inputObjectValues_0_3 = 32;
  int __xlx_offset_param_inputObjectValues_0_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_0_3 = 0*4;
  // Collect __xlx_inputObjectValues_1_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_1_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_1_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_0)[i]);
}
  int __xlx_size_param_inputObjectValues_1_0 = 32;
  int __xlx_offset_param_inputObjectValues_1_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_1_0 = 0*4;
  // Collect __xlx_inputObjectValues_1_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_1_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_1_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_1)[i]);
}
  int __xlx_size_param_inputObjectValues_1_1 = 32;
  int __xlx_offset_param_inputObjectValues_1_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_1_1 = 0*4;
  // Collect __xlx_inputObjectValues_1_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_1_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_1_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_2)[i]);
}
  int __xlx_size_param_inputObjectValues_1_2 = 32;
  int __xlx_offset_param_inputObjectValues_1_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_1_2 = 0*4;
  // Collect __xlx_inputObjectValues_1_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_1_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_1_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_3)[i]);
}
  int __xlx_size_param_inputObjectValues_1_3 = 32;
  int __xlx_offset_param_inputObjectValues_1_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_1_3 = 0*4;
  // Collect __xlx_inputObjectValues_2_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_2_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_2_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_0)[i]);
}
  int __xlx_size_param_inputObjectValues_2_0 = 32;
  int __xlx_offset_param_inputObjectValues_2_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_2_0 = 0*4;
  // Collect __xlx_inputObjectValues_2_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_2_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_2_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_1)[i]);
}
  int __xlx_size_param_inputObjectValues_2_1 = 32;
  int __xlx_offset_param_inputObjectValues_2_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_2_1 = 0*4;
  // Collect __xlx_inputObjectValues_2_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_2_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_2_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_2)[i]);
}
  int __xlx_size_param_inputObjectValues_2_2 = 32;
  int __xlx_offset_param_inputObjectValues_2_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_2_2 = 0*4;
  // Collect __xlx_inputObjectValues_2_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_2_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_2_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_3)[i]);
}
  int __xlx_size_param_inputObjectValues_2_3 = 32;
  int __xlx_offset_param_inputObjectValues_2_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_2_3 = 0*4;
  // Collect __xlx_inputObjectValues_3_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_3_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_3_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_0)[i]);
}
  int __xlx_size_param_inputObjectValues_3_0 = 32;
  int __xlx_offset_param_inputObjectValues_3_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_3_0 = 0*4;
  // Collect __xlx_inputObjectValues_3_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_3_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_3_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_1)[i]);
}
  int __xlx_size_param_inputObjectValues_3_1 = 32;
  int __xlx_offset_param_inputObjectValues_3_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_3_1 = 0*4;
  // Collect __xlx_inputObjectValues_3_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_3_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_3_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_2)[i]);
}
  int __xlx_size_param_inputObjectValues_3_2 = 32;
  int __xlx_offset_param_inputObjectValues_3_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_3_2 = 0*4;
  // Collect __xlx_inputObjectValues_3_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_3_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_3_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_3)[i]);
}
  int __xlx_size_param_inputObjectValues_3_3 = 32;
  int __xlx_offset_param_inputObjectValues_3_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_3_3 = 0*4;
  // Collect __xlx_inputObjectValues_4_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_4_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_4_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_0)[i]);
}
  int __xlx_size_param_inputObjectValues_4_0 = 32;
  int __xlx_offset_param_inputObjectValues_4_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_4_0 = 0*4;
  // Collect __xlx_inputObjectValues_4_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_4_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_4_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_1)[i]);
}
  int __xlx_size_param_inputObjectValues_4_1 = 32;
  int __xlx_offset_param_inputObjectValues_4_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_4_1 = 0*4;
  // Collect __xlx_inputObjectValues_4_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_4_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_4_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_2)[i]);
}
  int __xlx_size_param_inputObjectValues_4_2 = 32;
  int __xlx_offset_param_inputObjectValues_4_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_4_2 = 0*4;
  // Collect __xlx_inputObjectValues_4_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_4_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_4_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_3)[i]);
}
  int __xlx_size_param_inputObjectValues_4_3 = 32;
  int __xlx_offset_param_inputObjectValues_4_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_4_3 = 0*4;
  // Collect __xlx_inputObjectValues_5_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_5_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_5_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_0)[i]);
}
  int __xlx_size_param_inputObjectValues_5_0 = 32;
  int __xlx_offset_param_inputObjectValues_5_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_5_0 = 0*4;
  // Collect __xlx_inputObjectValues_5_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_5_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_5_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_1)[i]);
}
  int __xlx_size_param_inputObjectValues_5_1 = 32;
  int __xlx_offset_param_inputObjectValues_5_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_5_1 = 0*4;
  // Collect __xlx_inputObjectValues_5_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_5_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_5_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_2)[i]);
}
  int __xlx_size_param_inputObjectValues_5_2 = 32;
  int __xlx_offset_param_inputObjectValues_5_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_5_2 = 0*4;
  // Collect __xlx_inputObjectValues_5_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_5_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_5_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_3)[i]);
}
  int __xlx_size_param_inputObjectValues_5_3 = 32;
  int __xlx_offset_param_inputObjectValues_5_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_5_3 = 0*4;
  // Collect __xlx_inputObjectValues_6_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_6_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_6_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_0)[i]);
}
  int __xlx_size_param_inputObjectValues_6_0 = 32;
  int __xlx_offset_param_inputObjectValues_6_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_6_0 = 0*4;
  // Collect __xlx_inputObjectValues_6_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_6_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_6_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_1)[i]);
}
  int __xlx_size_param_inputObjectValues_6_1 = 32;
  int __xlx_offset_param_inputObjectValues_6_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_6_1 = 0*4;
  // Collect __xlx_inputObjectValues_6_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_6_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_6_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_2)[i]);
}
  int __xlx_size_param_inputObjectValues_6_2 = 32;
  int __xlx_offset_param_inputObjectValues_6_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_6_2 = 0*4;
  // Collect __xlx_inputObjectValues_6_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_6_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_6_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_3)[i]);
}
  int __xlx_size_param_inputObjectValues_6_3 = 32;
  int __xlx_offset_param_inputObjectValues_6_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_6_3 = 0*4;
  // Collect __xlx_inputObjectValues_7_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_7_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_7_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_0)[i]);
}
  int __xlx_size_param_inputObjectValues_7_0 = 32;
  int __xlx_offset_param_inputObjectValues_7_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_7_0 = 0*4;
  // Collect __xlx_inputObjectValues_7_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_7_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_7_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_1)[i]);
}
  int __xlx_size_param_inputObjectValues_7_1 = 32;
  int __xlx_offset_param_inputObjectValues_7_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_7_1 = 0*4;
  // Collect __xlx_inputObjectValues_7_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_7_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_7_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_2)[i]);
}
  int __xlx_size_param_inputObjectValues_7_2 = 32;
  int __xlx_offset_param_inputObjectValues_7_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_7_2 = 0*4;
  // Collect __xlx_inputObjectValues_7_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_7_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_7_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_3)[i]);
}
  int __xlx_size_param_inputObjectValues_7_3 = 32;
  int __xlx_offset_param_inputObjectValues_7_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_7_3 = 0*4;
  // Collect __xlx_inputObjectValues_8_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_8_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_8_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_0)[i]);
}
  int __xlx_size_param_inputObjectValues_8_0 = 32;
  int __xlx_offset_param_inputObjectValues_8_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_8_0 = 0*4;
  // Collect __xlx_inputObjectValues_8_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_8_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_8_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_1)[i]);
}
  int __xlx_size_param_inputObjectValues_8_1 = 32;
  int __xlx_offset_param_inputObjectValues_8_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_8_1 = 0*4;
  // Collect __xlx_inputObjectValues_8_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_8_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_8_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_2)[i]);
}
  int __xlx_size_param_inputObjectValues_8_2 = 32;
  int __xlx_offset_param_inputObjectValues_8_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_8_2 = 0*4;
  // Collect __xlx_inputObjectValues_8_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_8_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_8_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_3)[i]);
}
  int __xlx_size_param_inputObjectValues_8_3 = 32;
  int __xlx_offset_param_inputObjectValues_8_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_8_3 = 0*4;
  // Collect __xlx_inputObjectValues_9_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_9_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_9_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_0)[i]);
}
  int __xlx_size_param_inputObjectValues_9_0 = 32;
  int __xlx_offset_param_inputObjectValues_9_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_9_0 = 0*4;
  // Collect __xlx_inputObjectValues_9_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_9_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_9_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_1)[i]);
}
  int __xlx_size_param_inputObjectValues_9_1 = 32;
  int __xlx_offset_param_inputObjectValues_9_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_9_1 = 0*4;
  // Collect __xlx_inputObjectValues_9_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_9_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_9_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_2)[i]);
}
  int __xlx_size_param_inputObjectValues_9_2 = 32;
  int __xlx_offset_param_inputObjectValues_9_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_9_2 = 0*4;
  // Collect __xlx_inputObjectValues_9_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_9_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_9_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_3)[i]);
}
  int __xlx_size_param_inputObjectValues_9_3 = 32;
  int __xlx_offset_param_inputObjectValues_9_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_9_3 = 0*4;
  // Collect __xlx_inputObjectValues_10_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_10_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_10_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_0)[i]);
}
  int __xlx_size_param_inputObjectValues_10_0 = 32;
  int __xlx_offset_param_inputObjectValues_10_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_10_0 = 0*4;
  // Collect __xlx_inputObjectValues_10_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_10_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_10_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_1)[i]);
}
  int __xlx_size_param_inputObjectValues_10_1 = 32;
  int __xlx_offset_param_inputObjectValues_10_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_10_1 = 0*4;
  // Collect __xlx_inputObjectValues_10_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_10_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_10_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_2)[i]);
}
  int __xlx_size_param_inputObjectValues_10_2 = 32;
  int __xlx_offset_param_inputObjectValues_10_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_10_2 = 0*4;
  // Collect __xlx_inputObjectValues_10_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_10_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_10_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_3)[i]);
}
  int __xlx_size_param_inputObjectValues_10_3 = 32;
  int __xlx_offset_param_inputObjectValues_10_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_10_3 = 0*4;
  // Collect __xlx_inputObjectValues_11_0__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_11_0__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_11_0__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_0)[i]);
}
  int __xlx_size_param_inputObjectValues_11_0 = 32;
  int __xlx_offset_param_inputObjectValues_11_0 = 0;
  int __xlx_offset_byte_param_inputObjectValues_11_0 = 0*4;
  // Collect __xlx_inputObjectValues_11_1__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_11_1__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_11_1__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_1)[i]);
}
  int __xlx_size_param_inputObjectValues_11_1 = 32;
  int __xlx_offset_param_inputObjectValues_11_1 = 0;
  int __xlx_offset_byte_param_inputObjectValues_11_1 = 0*4;
  // Collect __xlx_inputObjectValues_11_2__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_11_2__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_11_2__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_2)[i]);
}
  int __xlx_size_param_inputObjectValues_11_2 = 32;
  int __xlx_offset_param_inputObjectValues_11_2 = 0;
  int __xlx_offset_byte_param_inputObjectValues_11_2 = 0*4;
  // Collect __xlx_inputObjectValues_11_3__tmp_vec
std::vector<Byte<4>> __xlx_inputObjectValues_11_3__tmp_vec;
for (size_t i = 0; i < 32; ++i){
__xlx_inputObjectValues_11_3__tmp_vec.push_back(((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_3)[i]);
}
  int __xlx_size_param_inputObjectValues_11_3 = 32;
  int __xlx_offset_param_inputObjectValues_11_3 = 0;
  int __xlx_offset_byte_param_inputObjectValues_11_3 = 0*4;
  // DUT call
  top(__xlx_apatb_param_seedValues_0, __xlx_apatb_param_seedValues_1, __xlx_apatb_param_seedValues_2, __xlx_apatb_param_seedValues_3, __xlx_apatb_param_seedValues_4, __xlx_apatb_param_seedValues_5, __xlx_apatb_param_seedValues_6, __xlx_apatb_param_seedValues_7, __xlx_apatb_param_seedValues_8, __xlx_apatb_param_seedValues_9, __xlx_apatb_param_seedValues_10, __xlx_apatb_param_seedValues_11, __xlx_inputObjectValues_0_0__tmp_vec.data(), __xlx_inputObjectValues_0_1__tmp_vec.data(), __xlx_inputObjectValues_0_2__tmp_vec.data(), __xlx_inputObjectValues_0_3__tmp_vec.data(), __xlx_inputObjectValues_1_0__tmp_vec.data(), __xlx_inputObjectValues_1_1__tmp_vec.data(), __xlx_inputObjectValues_1_2__tmp_vec.data(), __xlx_inputObjectValues_1_3__tmp_vec.data(), __xlx_inputObjectValues_2_0__tmp_vec.data(), __xlx_inputObjectValues_2_1__tmp_vec.data(), __xlx_inputObjectValues_2_2__tmp_vec.data(), __xlx_inputObjectValues_2_3__tmp_vec.data(), __xlx_inputObjectValues_3_0__tmp_vec.data(), __xlx_inputObjectValues_3_1__tmp_vec.data(), __xlx_inputObjectValues_3_2__tmp_vec.data(), __xlx_inputObjectValues_3_3__tmp_vec.data(), __xlx_inputObjectValues_4_0__tmp_vec.data(), __xlx_inputObjectValues_4_1__tmp_vec.data(), __xlx_inputObjectValues_4_2__tmp_vec.data(), __xlx_inputObjectValues_4_3__tmp_vec.data(), __xlx_inputObjectValues_5_0__tmp_vec.data(), __xlx_inputObjectValues_5_1__tmp_vec.data(), __xlx_inputObjectValues_5_2__tmp_vec.data(), __xlx_inputObjectValues_5_3__tmp_vec.data(), __xlx_inputObjectValues_6_0__tmp_vec.data(), __xlx_inputObjectValues_6_1__tmp_vec.data(), __xlx_inputObjectValues_6_2__tmp_vec.data(), __xlx_inputObjectValues_6_3__tmp_vec.data(), __xlx_inputObjectValues_7_0__tmp_vec.data(), __xlx_inputObjectValues_7_1__tmp_vec.data(), __xlx_inputObjectValues_7_2__tmp_vec.data(), __xlx_inputObjectValues_7_3__tmp_vec.data(), __xlx_inputObjectValues_8_0__tmp_vec.data(), __xlx_inputObjectValues_8_1__tmp_vec.data(), __xlx_inputObjectValues_8_2__tmp_vec.data(), __xlx_inputObjectValues_8_3__tmp_vec.data(), __xlx_inputObjectValues_9_0__tmp_vec.data(), __xlx_inputObjectValues_9_1__tmp_vec.data(), __xlx_inputObjectValues_9_2__tmp_vec.data(), __xlx_inputObjectValues_9_3__tmp_vec.data(), __xlx_inputObjectValues_10_0__tmp_vec.data(), __xlx_inputObjectValues_10_1__tmp_vec.data(), __xlx_inputObjectValues_10_2__tmp_vec.data(), __xlx_inputObjectValues_10_3__tmp_vec.data(), __xlx_inputObjectValues_11_0__tmp_vec.data(), __xlx_inputObjectValues_11_1__tmp_vec.data(), __xlx_inputObjectValues_11_2__tmp_vec.data(), __xlx_inputObjectValues_11_3__tmp_vec.data(), __xlx_apatb_param_outputJetValues_0, __xlx_apatb_param_outputJetValues_1);
// print __xlx_apatb_param_inputObjectValues_0_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_0_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_0)[i] = __xlx_inputObjectValues_0_0__tmp_vec[__xlx_offset_param_inputObjectValues_0_0+i];
}
// print __xlx_apatb_param_inputObjectValues_0_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_0_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_1)[i] = __xlx_inputObjectValues_0_1__tmp_vec[__xlx_offset_param_inputObjectValues_0_1+i];
}
// print __xlx_apatb_param_inputObjectValues_0_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_0_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_2)[i] = __xlx_inputObjectValues_0_2__tmp_vec[__xlx_offset_param_inputObjectValues_0_2+i];
}
// print __xlx_apatb_param_inputObjectValues_0_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_0_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_0_3)[i] = __xlx_inputObjectValues_0_3__tmp_vec[__xlx_offset_param_inputObjectValues_0_3+i];
}
// print __xlx_apatb_param_inputObjectValues_1_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_1_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_0)[i] = __xlx_inputObjectValues_1_0__tmp_vec[__xlx_offset_param_inputObjectValues_1_0+i];
}
// print __xlx_apatb_param_inputObjectValues_1_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_1_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_1)[i] = __xlx_inputObjectValues_1_1__tmp_vec[__xlx_offset_param_inputObjectValues_1_1+i];
}
// print __xlx_apatb_param_inputObjectValues_1_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_1_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_2)[i] = __xlx_inputObjectValues_1_2__tmp_vec[__xlx_offset_param_inputObjectValues_1_2+i];
}
// print __xlx_apatb_param_inputObjectValues_1_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_1_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_1_3)[i] = __xlx_inputObjectValues_1_3__tmp_vec[__xlx_offset_param_inputObjectValues_1_3+i];
}
// print __xlx_apatb_param_inputObjectValues_2_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_2_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_0)[i] = __xlx_inputObjectValues_2_0__tmp_vec[__xlx_offset_param_inputObjectValues_2_0+i];
}
// print __xlx_apatb_param_inputObjectValues_2_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_2_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_1)[i] = __xlx_inputObjectValues_2_1__tmp_vec[__xlx_offset_param_inputObjectValues_2_1+i];
}
// print __xlx_apatb_param_inputObjectValues_2_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_2_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_2)[i] = __xlx_inputObjectValues_2_2__tmp_vec[__xlx_offset_param_inputObjectValues_2_2+i];
}
// print __xlx_apatb_param_inputObjectValues_2_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_2_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_2_3)[i] = __xlx_inputObjectValues_2_3__tmp_vec[__xlx_offset_param_inputObjectValues_2_3+i];
}
// print __xlx_apatb_param_inputObjectValues_3_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_3_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_0)[i] = __xlx_inputObjectValues_3_0__tmp_vec[__xlx_offset_param_inputObjectValues_3_0+i];
}
// print __xlx_apatb_param_inputObjectValues_3_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_3_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_1)[i] = __xlx_inputObjectValues_3_1__tmp_vec[__xlx_offset_param_inputObjectValues_3_1+i];
}
// print __xlx_apatb_param_inputObjectValues_3_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_3_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_2)[i] = __xlx_inputObjectValues_3_2__tmp_vec[__xlx_offset_param_inputObjectValues_3_2+i];
}
// print __xlx_apatb_param_inputObjectValues_3_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_3_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_3_3)[i] = __xlx_inputObjectValues_3_3__tmp_vec[__xlx_offset_param_inputObjectValues_3_3+i];
}
// print __xlx_apatb_param_inputObjectValues_4_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_4_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_0)[i] = __xlx_inputObjectValues_4_0__tmp_vec[__xlx_offset_param_inputObjectValues_4_0+i];
}
// print __xlx_apatb_param_inputObjectValues_4_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_4_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_1)[i] = __xlx_inputObjectValues_4_1__tmp_vec[__xlx_offset_param_inputObjectValues_4_1+i];
}
// print __xlx_apatb_param_inputObjectValues_4_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_4_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_2)[i] = __xlx_inputObjectValues_4_2__tmp_vec[__xlx_offset_param_inputObjectValues_4_2+i];
}
// print __xlx_apatb_param_inputObjectValues_4_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_4_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_4_3)[i] = __xlx_inputObjectValues_4_3__tmp_vec[__xlx_offset_param_inputObjectValues_4_3+i];
}
// print __xlx_apatb_param_inputObjectValues_5_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_5_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_0)[i] = __xlx_inputObjectValues_5_0__tmp_vec[__xlx_offset_param_inputObjectValues_5_0+i];
}
// print __xlx_apatb_param_inputObjectValues_5_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_5_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_1)[i] = __xlx_inputObjectValues_5_1__tmp_vec[__xlx_offset_param_inputObjectValues_5_1+i];
}
// print __xlx_apatb_param_inputObjectValues_5_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_5_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_2)[i] = __xlx_inputObjectValues_5_2__tmp_vec[__xlx_offset_param_inputObjectValues_5_2+i];
}
// print __xlx_apatb_param_inputObjectValues_5_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_5_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_5_3)[i] = __xlx_inputObjectValues_5_3__tmp_vec[__xlx_offset_param_inputObjectValues_5_3+i];
}
// print __xlx_apatb_param_inputObjectValues_6_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_6_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_0)[i] = __xlx_inputObjectValues_6_0__tmp_vec[__xlx_offset_param_inputObjectValues_6_0+i];
}
// print __xlx_apatb_param_inputObjectValues_6_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_6_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_1)[i] = __xlx_inputObjectValues_6_1__tmp_vec[__xlx_offset_param_inputObjectValues_6_1+i];
}
// print __xlx_apatb_param_inputObjectValues_6_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_6_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_2)[i] = __xlx_inputObjectValues_6_2__tmp_vec[__xlx_offset_param_inputObjectValues_6_2+i];
}
// print __xlx_apatb_param_inputObjectValues_6_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_6_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_6_3)[i] = __xlx_inputObjectValues_6_3__tmp_vec[__xlx_offset_param_inputObjectValues_6_3+i];
}
// print __xlx_apatb_param_inputObjectValues_7_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_7_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_0)[i] = __xlx_inputObjectValues_7_0__tmp_vec[__xlx_offset_param_inputObjectValues_7_0+i];
}
// print __xlx_apatb_param_inputObjectValues_7_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_7_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_1)[i] = __xlx_inputObjectValues_7_1__tmp_vec[__xlx_offset_param_inputObjectValues_7_1+i];
}
// print __xlx_apatb_param_inputObjectValues_7_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_7_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_2)[i] = __xlx_inputObjectValues_7_2__tmp_vec[__xlx_offset_param_inputObjectValues_7_2+i];
}
// print __xlx_apatb_param_inputObjectValues_7_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_7_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_7_3)[i] = __xlx_inputObjectValues_7_3__tmp_vec[__xlx_offset_param_inputObjectValues_7_3+i];
}
// print __xlx_apatb_param_inputObjectValues_8_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_8_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_0)[i] = __xlx_inputObjectValues_8_0__tmp_vec[__xlx_offset_param_inputObjectValues_8_0+i];
}
// print __xlx_apatb_param_inputObjectValues_8_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_8_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_1)[i] = __xlx_inputObjectValues_8_1__tmp_vec[__xlx_offset_param_inputObjectValues_8_1+i];
}
// print __xlx_apatb_param_inputObjectValues_8_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_8_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_2)[i] = __xlx_inputObjectValues_8_2__tmp_vec[__xlx_offset_param_inputObjectValues_8_2+i];
}
// print __xlx_apatb_param_inputObjectValues_8_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_8_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_8_3)[i] = __xlx_inputObjectValues_8_3__tmp_vec[__xlx_offset_param_inputObjectValues_8_3+i];
}
// print __xlx_apatb_param_inputObjectValues_9_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_9_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_0)[i] = __xlx_inputObjectValues_9_0__tmp_vec[__xlx_offset_param_inputObjectValues_9_0+i];
}
// print __xlx_apatb_param_inputObjectValues_9_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_9_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_1)[i] = __xlx_inputObjectValues_9_1__tmp_vec[__xlx_offset_param_inputObjectValues_9_1+i];
}
// print __xlx_apatb_param_inputObjectValues_9_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_9_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_2)[i] = __xlx_inputObjectValues_9_2__tmp_vec[__xlx_offset_param_inputObjectValues_9_2+i];
}
// print __xlx_apatb_param_inputObjectValues_9_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_9_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_9_3)[i] = __xlx_inputObjectValues_9_3__tmp_vec[__xlx_offset_param_inputObjectValues_9_3+i];
}
// print __xlx_apatb_param_inputObjectValues_10_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_10_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_0)[i] = __xlx_inputObjectValues_10_0__tmp_vec[__xlx_offset_param_inputObjectValues_10_0+i];
}
// print __xlx_apatb_param_inputObjectValues_10_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_10_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_1)[i] = __xlx_inputObjectValues_10_1__tmp_vec[__xlx_offset_param_inputObjectValues_10_1+i];
}
// print __xlx_apatb_param_inputObjectValues_10_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_10_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_2)[i] = __xlx_inputObjectValues_10_2__tmp_vec[__xlx_offset_param_inputObjectValues_10_2+i];
}
// print __xlx_apatb_param_inputObjectValues_10_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_10_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_10_3)[i] = __xlx_inputObjectValues_10_3__tmp_vec[__xlx_offset_param_inputObjectValues_10_3+i];
}
// print __xlx_apatb_param_inputObjectValues_11_0
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_11_0; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_0)[i] = __xlx_inputObjectValues_11_0__tmp_vec[__xlx_offset_param_inputObjectValues_11_0+i];
}
// print __xlx_apatb_param_inputObjectValues_11_1
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_11_1; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_1)[i] = __xlx_inputObjectValues_11_1__tmp_vec[__xlx_offset_param_inputObjectValues_11_1+i];
}
// print __xlx_apatb_param_inputObjectValues_11_2
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_11_2; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_2)[i] = __xlx_inputObjectValues_11_2__tmp_vec[__xlx_offset_param_inputObjectValues_11_2+i];
}
// print __xlx_apatb_param_inputObjectValues_11_3
for (size_t i = 0; i < __xlx_size_param_inputObjectValues_11_3; ++i) {
((Byte<4>*)__xlx_apatb_param_inputObjectValues_11_3)[i] = __xlx_inputObjectValues_11_3__tmp_vec[__xlx_offset_param_inputObjectValues_11_3+i];
}
}
