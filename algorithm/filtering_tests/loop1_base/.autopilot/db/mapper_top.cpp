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
struct __cosim_s512__ { char data[512]; };
extern "C" void top(volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, __cosim_s512__*, __cosim_s512__*);
extern "C" void apatb_top_hw(volatile void * __xlx_apatb_param_inputArray_0, volatile void * __xlx_apatb_param_inputArray_1, volatile void * __xlx_apatb_param_inputArray_2, volatile void * __xlx_apatb_param_inputArray_3, volatile void * __xlx_apatb_param_inputArray_4, volatile void * __xlx_apatb_param_inputArray_5, volatile void * __xlx_apatb_param_inputArray_6, volatile void * __xlx_apatb_param_inputArray_7, volatile void * __xlx_apatb_param_inputArray_8, volatile void * __xlx_apatb_param_inputArray_9, volatile void * __xlx_apatb_param_inputArray_10, volatile void * __xlx_apatb_param_inputArray_11, volatile void * __xlx_apatb_param_inputArray_12, volatile void * __xlx_apatb_param_inputArray_13, volatile void * __xlx_apatb_param_inputArray_14, volatile void * __xlx_apatb_param_inputArray_15, volatile void * __xlx_apatb_param_inputArray_16, volatile void * __xlx_apatb_param_inputArray_17, volatile void * __xlx_apatb_param_inputArray_18, volatile void * __xlx_apatb_param_inputArray_19, volatile void * __xlx_apatb_param_inputArray_20, volatile void * __xlx_apatb_param_inputArray_21, volatile void * __xlx_apatb_param_inputArray_22, volatile void * __xlx_apatb_param_inputArray_23, volatile void * __xlx_apatb_param_inputArray_24, volatile void * __xlx_apatb_param_inputArray_25, volatile void * __xlx_apatb_param_inputArray_26, volatile void * __xlx_apatb_param_inputArray_27, volatile void * __xlx_apatb_param_inputArray_28, volatile void * __xlx_apatb_param_inputArray_29, volatile void * __xlx_apatb_param_inputArray_30, volatile void * __xlx_apatb_param_inputArray_31, volatile void * __xlx_apatb_param_inputArray_32, volatile void * __xlx_apatb_param_inputArray_33, volatile void * __xlx_apatb_param_inputArray_34, volatile void * __xlx_apatb_param_inputArray_35, volatile void * __xlx_apatb_param_inputArray_36, volatile void * __xlx_apatb_param_inputArray_37, volatile void * __xlx_apatb_param_inputArray_38, volatile void * __xlx_apatb_param_inputArray_39, volatile void * __xlx_apatb_param_inputArray_40, volatile void * __xlx_apatb_param_inputArray_41, volatile void * __xlx_apatb_param_inputArray_42, volatile void * __xlx_apatb_param_inputArray_43, volatile void * __xlx_apatb_param_inputArray_44, volatile void * __xlx_apatb_param_inputArray_45, volatile void * __xlx_apatb_param_inputArray_46, volatile void * __xlx_apatb_param_inputArray_47, volatile void * __xlx_apatb_param_inputArray_48, volatile void * __xlx_apatb_param_inputArray_49, volatile void * __xlx_apatb_param_inputArray_50, volatile void * __xlx_apatb_param_inputArray_51, volatile void * __xlx_apatb_param_inputArray_52, volatile void * __xlx_apatb_param_inputArray_53, volatile void * __xlx_apatb_param_inputArray_54, volatile void * __xlx_apatb_param_inputArray_55, volatile void * __xlx_apatb_param_inputArray_56, volatile void * __xlx_apatb_param_inputArray_57, volatile void * __xlx_apatb_param_inputArray_58, volatile void * __xlx_apatb_param_inputArray_59, volatile void * __xlx_apatb_param_inputArray_60, volatile void * __xlx_apatb_param_inputArray_61, volatile void * __xlx_apatb_param_inputArray_62, volatile void * __xlx_apatb_param_inputArray_63, volatile void * __xlx_apatb_param_inputArray_64, volatile void * __xlx_apatb_param_inputArray_65, volatile void * __xlx_apatb_param_inputArray_66, volatile void * __xlx_apatb_param_inputArray_67, volatile void * __xlx_apatb_param_inputArray_68, volatile void * __xlx_apatb_param_inputArray_69, volatile void * __xlx_apatb_param_inputArray_70, volatile void * __xlx_apatb_param_inputArray_71, volatile void * __xlx_apatb_param_inputArray_72, volatile void * __xlx_apatb_param_inputArray_73, volatile void * __xlx_apatb_param_inputArray_74, volatile void * __xlx_apatb_param_inputArray_75, volatile void * __xlx_apatb_param_inputArray_76, volatile void * __xlx_apatb_param_inputArray_77, volatile void * __xlx_apatb_param_inputArray_78, volatile void * __xlx_apatb_param_inputArray_79, volatile void * __xlx_apatb_param_inputArray_80, volatile void * __xlx_apatb_param_inputArray_81, volatile void * __xlx_apatb_param_inputArray_82, volatile void * __xlx_apatb_param_inputArray_83, volatile void * __xlx_apatb_param_inputArray_84, volatile void * __xlx_apatb_param_inputArray_85, volatile void * __xlx_apatb_param_inputArray_86, volatile void * __xlx_apatb_param_inputArray_87, volatile void * __xlx_apatb_param_inputArray_88, volatile void * __xlx_apatb_param_inputArray_89, volatile void * __xlx_apatb_param_inputArray_90, volatile void * __xlx_apatb_param_inputArray_91, volatile void * __xlx_apatb_param_inputArray_92, volatile void * __xlx_apatb_param_inputArray_93, volatile void * __xlx_apatb_param_inputArray_94, volatile void * __xlx_apatb_param_inputArray_95, volatile void * __xlx_apatb_param_inputArray_96, volatile void * __xlx_apatb_param_inputArray_97, volatile void * __xlx_apatb_param_inputArray_98, volatile void * __xlx_apatb_param_inputArray_99, volatile void * __xlx_apatb_param_inputArray_100, volatile void * __xlx_apatb_param_inputArray_101, volatile void * __xlx_apatb_param_inputArray_102, volatile void * __xlx_apatb_param_inputArray_103, volatile void * __xlx_apatb_param_inputArray_104, volatile void * __xlx_apatb_param_inputArray_105, volatile void * __xlx_apatb_param_inputArray_106, volatile void * __xlx_apatb_param_inputArray_107, volatile void * __xlx_apatb_param_inputArray_108, volatile void * __xlx_apatb_param_inputArray_109, volatile void * __xlx_apatb_param_inputArray_110, volatile void * __xlx_apatb_param_inputArray_111, volatile void * __xlx_apatb_param_inputArray_112, volatile void * __xlx_apatb_param_inputArray_113, volatile void * __xlx_apatb_param_inputArray_114, volatile void * __xlx_apatb_param_inputArray_115, volatile void * __xlx_apatb_param_inputArray_116, volatile void * __xlx_apatb_param_inputArray_117, volatile void * __xlx_apatb_param_inputArray_118, volatile void * __xlx_apatb_param_inputArray_119, volatile void * __xlx_apatb_param_inputArray_120, volatile void * __xlx_apatb_param_inputArray_121, volatile void * __xlx_apatb_param_inputArray_122, volatile void * __xlx_apatb_param_inputArray_123, volatile void * __xlx_apatb_param_inputArray_124, volatile void * __xlx_apatb_param_inputArray_125, volatile void * __xlx_apatb_param_inputArray_126, volatile void * __xlx_apatb_param_inputArray_127, volatile void * __xlx_apatb_param_filterArray_0, volatile void * __xlx_apatb_param_filterArray_1, volatile void * __xlx_apatb_param_filterArray_2, volatile void * __xlx_apatb_param_filterArray_3, volatile void * __xlx_apatb_param_filterArray_4, volatile void * __xlx_apatb_param_filterArray_5, volatile void * __xlx_apatb_param_filterArray_6, volatile void * __xlx_apatb_param_filterArray_7, volatile void * __xlx_apatb_param_filterArray_8, volatile void * __xlx_apatb_param_filterArray_9, volatile void * __xlx_apatb_param_filterArray_10, volatile void * __xlx_apatb_param_filterArray_11, volatile void * __xlx_apatb_param_filterArray_12, volatile void * __xlx_apatb_param_filterArray_13, volatile void * __xlx_apatb_param_filterArray_14, volatile void * __xlx_apatb_param_filterArray_15, volatile void * __xlx_apatb_param_filterArray_16, volatile void * __xlx_apatb_param_filterArray_17, volatile void * __xlx_apatb_param_filterArray_18, volatile void * __xlx_apatb_param_filterArray_19, volatile void * __xlx_apatb_param_filterArray_20, volatile void * __xlx_apatb_param_filterArray_21, volatile void * __xlx_apatb_param_filterArray_22, volatile void * __xlx_apatb_param_filterArray_23, volatile void * __xlx_apatb_param_filterArray_24, volatile void * __xlx_apatb_param_filterArray_25, volatile void * __xlx_apatb_param_filterArray_26, volatile void * __xlx_apatb_param_filterArray_27, volatile void * __xlx_apatb_param_filterArray_28, volatile void * __xlx_apatb_param_filterArray_29, volatile void * __xlx_apatb_param_filterArray_30, volatile void * __xlx_apatb_param_filterArray_31, volatile void * __xlx_apatb_param_filterArray_32, volatile void * __xlx_apatb_param_filterArray_33, volatile void * __xlx_apatb_param_filterArray_34, volatile void * __xlx_apatb_param_filterArray_35, volatile void * __xlx_apatb_param_filterArray_36, volatile void * __xlx_apatb_param_filterArray_37, volatile void * __xlx_apatb_param_filterArray_38, volatile void * __xlx_apatb_param_filterArray_39, volatile void * __xlx_apatb_param_filterArray_40, volatile void * __xlx_apatb_param_filterArray_41, volatile void * __xlx_apatb_param_filterArray_42, volatile void * __xlx_apatb_param_filterArray_43, volatile void * __xlx_apatb_param_filterArray_44, volatile void * __xlx_apatb_param_filterArray_45, volatile void * __xlx_apatb_param_filterArray_46, volatile void * __xlx_apatb_param_filterArray_47, volatile void * __xlx_apatb_param_filterArray_48, volatile void * __xlx_apatb_param_filterArray_49, volatile void * __xlx_apatb_param_filterArray_50, volatile void * __xlx_apatb_param_filterArray_51, volatile void * __xlx_apatb_param_filterArray_52, volatile void * __xlx_apatb_param_filterArray_53, volatile void * __xlx_apatb_param_filterArray_54, volatile void * __xlx_apatb_param_filterArray_55, volatile void * __xlx_apatb_param_filterArray_56, volatile void * __xlx_apatb_param_filterArray_57, volatile void * __xlx_apatb_param_filterArray_58, volatile void * __xlx_apatb_param_filterArray_59, volatile void * __xlx_apatb_param_filterArray_60, volatile void * __xlx_apatb_param_filterArray_61, volatile void * __xlx_apatb_param_filterArray_62, volatile void * __xlx_apatb_param_filterArray_63, volatile void * __xlx_apatb_param_filterArray_64, volatile void * __xlx_apatb_param_filterArray_65, volatile void * __xlx_apatb_param_filterArray_66, volatile void * __xlx_apatb_param_filterArray_67, volatile void * __xlx_apatb_param_filterArray_68, volatile void * __xlx_apatb_param_filterArray_69, volatile void * __xlx_apatb_param_filterArray_70, volatile void * __xlx_apatb_param_filterArray_71, volatile void * __xlx_apatb_param_filterArray_72, volatile void * __xlx_apatb_param_filterArray_73, volatile void * __xlx_apatb_param_filterArray_74, volatile void * __xlx_apatb_param_filterArray_75, volatile void * __xlx_apatb_param_filterArray_76, volatile void * __xlx_apatb_param_filterArray_77, volatile void * __xlx_apatb_param_filterArray_78, volatile void * __xlx_apatb_param_filterArray_79, volatile void * __xlx_apatb_param_filterArray_80, volatile void * __xlx_apatb_param_filterArray_81, volatile void * __xlx_apatb_param_filterArray_82, volatile void * __xlx_apatb_param_filterArray_83, volatile void * __xlx_apatb_param_filterArray_84, volatile void * __xlx_apatb_param_filterArray_85, volatile void * __xlx_apatb_param_filterArray_86, volatile void * __xlx_apatb_param_filterArray_87, volatile void * __xlx_apatb_param_filterArray_88, volatile void * __xlx_apatb_param_filterArray_89, volatile void * __xlx_apatb_param_filterArray_90, volatile void * __xlx_apatb_param_filterArray_91, volatile void * __xlx_apatb_param_filterArray_92, volatile void * __xlx_apatb_param_filterArray_93, volatile void * __xlx_apatb_param_filterArray_94, volatile void * __xlx_apatb_param_filterArray_95, volatile void * __xlx_apatb_param_filterArray_96, volatile void * __xlx_apatb_param_filterArray_97, volatile void * __xlx_apatb_param_filterArray_98, volatile void * __xlx_apatb_param_filterArray_99, volatile void * __xlx_apatb_param_filterArray_100, volatile void * __xlx_apatb_param_filterArray_101, volatile void * __xlx_apatb_param_filterArray_102, volatile void * __xlx_apatb_param_filterArray_103, volatile void * __xlx_apatb_param_filterArray_104, volatile void * __xlx_apatb_param_filterArray_105, volatile void * __xlx_apatb_param_filterArray_106, volatile void * __xlx_apatb_param_filterArray_107, volatile void * __xlx_apatb_param_filterArray_108, volatile void * __xlx_apatb_param_filterArray_109, volatile void * __xlx_apatb_param_filterArray_110, volatile void * __xlx_apatb_param_filterArray_111, volatile void * __xlx_apatb_param_filterArray_112, volatile void * __xlx_apatb_param_filterArray_113, volatile void * __xlx_apatb_param_filterArray_114, volatile void * __xlx_apatb_param_filterArray_115, volatile void * __xlx_apatb_param_filterArray_116, volatile void * __xlx_apatb_param_filterArray_117, volatile void * __xlx_apatb_param_filterArray_118, volatile void * __xlx_apatb_param_filterArray_119, volatile void * __xlx_apatb_param_filterArray_120, volatile void * __xlx_apatb_param_filterArray_121, volatile void * __xlx_apatb_param_filterArray_122, volatile void * __xlx_apatb_param_filterArray_123, volatile void * __xlx_apatb_param_filterArray_124, volatile void * __xlx_apatb_param_filterArray_125, volatile void * __xlx_apatb_param_filterArray_126, volatile void * __xlx_apatb_param_filterArray_127, __cosim_s512__* __xlx_apatb_param_bitmask1, __cosim_s512__* __xlx_apatb_param_bitmask2) {
using hls::sim::createStream;
  // DUT call
  top(__xlx_apatb_param_inputArray_0, __xlx_apatb_param_inputArray_1, __xlx_apatb_param_inputArray_2, __xlx_apatb_param_inputArray_3, __xlx_apatb_param_inputArray_4, __xlx_apatb_param_inputArray_5, __xlx_apatb_param_inputArray_6, __xlx_apatb_param_inputArray_7, __xlx_apatb_param_inputArray_8, __xlx_apatb_param_inputArray_9, __xlx_apatb_param_inputArray_10, __xlx_apatb_param_inputArray_11, __xlx_apatb_param_inputArray_12, __xlx_apatb_param_inputArray_13, __xlx_apatb_param_inputArray_14, __xlx_apatb_param_inputArray_15, __xlx_apatb_param_inputArray_16, __xlx_apatb_param_inputArray_17, __xlx_apatb_param_inputArray_18, __xlx_apatb_param_inputArray_19, __xlx_apatb_param_inputArray_20, __xlx_apatb_param_inputArray_21, __xlx_apatb_param_inputArray_22, __xlx_apatb_param_inputArray_23, __xlx_apatb_param_inputArray_24, __xlx_apatb_param_inputArray_25, __xlx_apatb_param_inputArray_26, __xlx_apatb_param_inputArray_27, __xlx_apatb_param_inputArray_28, __xlx_apatb_param_inputArray_29, __xlx_apatb_param_inputArray_30, __xlx_apatb_param_inputArray_31, __xlx_apatb_param_inputArray_32, __xlx_apatb_param_inputArray_33, __xlx_apatb_param_inputArray_34, __xlx_apatb_param_inputArray_35, __xlx_apatb_param_inputArray_36, __xlx_apatb_param_inputArray_37, __xlx_apatb_param_inputArray_38, __xlx_apatb_param_inputArray_39, __xlx_apatb_param_inputArray_40, __xlx_apatb_param_inputArray_41, __xlx_apatb_param_inputArray_42, __xlx_apatb_param_inputArray_43, __xlx_apatb_param_inputArray_44, __xlx_apatb_param_inputArray_45, __xlx_apatb_param_inputArray_46, __xlx_apatb_param_inputArray_47, __xlx_apatb_param_inputArray_48, __xlx_apatb_param_inputArray_49, __xlx_apatb_param_inputArray_50, __xlx_apatb_param_inputArray_51, __xlx_apatb_param_inputArray_52, __xlx_apatb_param_inputArray_53, __xlx_apatb_param_inputArray_54, __xlx_apatb_param_inputArray_55, __xlx_apatb_param_inputArray_56, __xlx_apatb_param_inputArray_57, __xlx_apatb_param_inputArray_58, __xlx_apatb_param_inputArray_59, __xlx_apatb_param_inputArray_60, __xlx_apatb_param_inputArray_61, __xlx_apatb_param_inputArray_62, __xlx_apatb_param_inputArray_63, __xlx_apatb_param_inputArray_64, __xlx_apatb_param_inputArray_65, __xlx_apatb_param_inputArray_66, __xlx_apatb_param_inputArray_67, __xlx_apatb_param_inputArray_68, __xlx_apatb_param_inputArray_69, __xlx_apatb_param_inputArray_70, __xlx_apatb_param_inputArray_71, __xlx_apatb_param_inputArray_72, __xlx_apatb_param_inputArray_73, __xlx_apatb_param_inputArray_74, __xlx_apatb_param_inputArray_75, __xlx_apatb_param_inputArray_76, __xlx_apatb_param_inputArray_77, __xlx_apatb_param_inputArray_78, __xlx_apatb_param_inputArray_79, __xlx_apatb_param_inputArray_80, __xlx_apatb_param_inputArray_81, __xlx_apatb_param_inputArray_82, __xlx_apatb_param_inputArray_83, __xlx_apatb_param_inputArray_84, __xlx_apatb_param_inputArray_85, __xlx_apatb_param_inputArray_86, __xlx_apatb_param_inputArray_87, __xlx_apatb_param_inputArray_88, __xlx_apatb_param_inputArray_89, __xlx_apatb_param_inputArray_90, __xlx_apatb_param_inputArray_91, __xlx_apatb_param_inputArray_92, __xlx_apatb_param_inputArray_93, __xlx_apatb_param_inputArray_94, __xlx_apatb_param_inputArray_95, __xlx_apatb_param_inputArray_96, __xlx_apatb_param_inputArray_97, __xlx_apatb_param_inputArray_98, __xlx_apatb_param_inputArray_99, __xlx_apatb_param_inputArray_100, __xlx_apatb_param_inputArray_101, __xlx_apatb_param_inputArray_102, __xlx_apatb_param_inputArray_103, __xlx_apatb_param_inputArray_104, __xlx_apatb_param_inputArray_105, __xlx_apatb_param_inputArray_106, __xlx_apatb_param_inputArray_107, __xlx_apatb_param_inputArray_108, __xlx_apatb_param_inputArray_109, __xlx_apatb_param_inputArray_110, __xlx_apatb_param_inputArray_111, __xlx_apatb_param_inputArray_112, __xlx_apatb_param_inputArray_113, __xlx_apatb_param_inputArray_114, __xlx_apatb_param_inputArray_115, __xlx_apatb_param_inputArray_116, __xlx_apatb_param_inputArray_117, __xlx_apatb_param_inputArray_118, __xlx_apatb_param_inputArray_119, __xlx_apatb_param_inputArray_120, __xlx_apatb_param_inputArray_121, __xlx_apatb_param_inputArray_122, __xlx_apatb_param_inputArray_123, __xlx_apatb_param_inputArray_124, __xlx_apatb_param_inputArray_125, __xlx_apatb_param_inputArray_126, __xlx_apatb_param_inputArray_127, __xlx_apatb_param_filterArray_0, __xlx_apatb_param_filterArray_1, __xlx_apatb_param_filterArray_2, __xlx_apatb_param_filterArray_3, __xlx_apatb_param_filterArray_4, __xlx_apatb_param_filterArray_5, __xlx_apatb_param_filterArray_6, __xlx_apatb_param_filterArray_7, __xlx_apatb_param_filterArray_8, __xlx_apatb_param_filterArray_9, __xlx_apatb_param_filterArray_10, __xlx_apatb_param_filterArray_11, __xlx_apatb_param_filterArray_12, __xlx_apatb_param_filterArray_13, __xlx_apatb_param_filterArray_14, __xlx_apatb_param_filterArray_15, __xlx_apatb_param_filterArray_16, __xlx_apatb_param_filterArray_17, __xlx_apatb_param_filterArray_18, __xlx_apatb_param_filterArray_19, __xlx_apatb_param_filterArray_20, __xlx_apatb_param_filterArray_21, __xlx_apatb_param_filterArray_22, __xlx_apatb_param_filterArray_23, __xlx_apatb_param_filterArray_24, __xlx_apatb_param_filterArray_25, __xlx_apatb_param_filterArray_26, __xlx_apatb_param_filterArray_27, __xlx_apatb_param_filterArray_28, __xlx_apatb_param_filterArray_29, __xlx_apatb_param_filterArray_30, __xlx_apatb_param_filterArray_31, __xlx_apatb_param_filterArray_32, __xlx_apatb_param_filterArray_33, __xlx_apatb_param_filterArray_34, __xlx_apatb_param_filterArray_35, __xlx_apatb_param_filterArray_36, __xlx_apatb_param_filterArray_37, __xlx_apatb_param_filterArray_38, __xlx_apatb_param_filterArray_39, __xlx_apatb_param_filterArray_40, __xlx_apatb_param_filterArray_41, __xlx_apatb_param_filterArray_42, __xlx_apatb_param_filterArray_43, __xlx_apatb_param_filterArray_44, __xlx_apatb_param_filterArray_45, __xlx_apatb_param_filterArray_46, __xlx_apatb_param_filterArray_47, __xlx_apatb_param_filterArray_48, __xlx_apatb_param_filterArray_49, __xlx_apatb_param_filterArray_50, __xlx_apatb_param_filterArray_51, __xlx_apatb_param_filterArray_52, __xlx_apatb_param_filterArray_53, __xlx_apatb_param_filterArray_54, __xlx_apatb_param_filterArray_55, __xlx_apatb_param_filterArray_56, __xlx_apatb_param_filterArray_57, __xlx_apatb_param_filterArray_58, __xlx_apatb_param_filterArray_59, __xlx_apatb_param_filterArray_60, __xlx_apatb_param_filterArray_61, __xlx_apatb_param_filterArray_62, __xlx_apatb_param_filterArray_63, __xlx_apatb_param_filterArray_64, __xlx_apatb_param_filterArray_65, __xlx_apatb_param_filterArray_66, __xlx_apatb_param_filterArray_67, __xlx_apatb_param_filterArray_68, __xlx_apatb_param_filterArray_69, __xlx_apatb_param_filterArray_70, __xlx_apatb_param_filterArray_71, __xlx_apatb_param_filterArray_72, __xlx_apatb_param_filterArray_73, __xlx_apatb_param_filterArray_74, __xlx_apatb_param_filterArray_75, __xlx_apatb_param_filterArray_76, __xlx_apatb_param_filterArray_77, __xlx_apatb_param_filterArray_78, __xlx_apatb_param_filterArray_79, __xlx_apatb_param_filterArray_80, __xlx_apatb_param_filterArray_81, __xlx_apatb_param_filterArray_82, __xlx_apatb_param_filterArray_83, __xlx_apatb_param_filterArray_84, __xlx_apatb_param_filterArray_85, __xlx_apatb_param_filterArray_86, __xlx_apatb_param_filterArray_87, __xlx_apatb_param_filterArray_88, __xlx_apatb_param_filterArray_89, __xlx_apatb_param_filterArray_90, __xlx_apatb_param_filterArray_91, __xlx_apatb_param_filterArray_92, __xlx_apatb_param_filterArray_93, __xlx_apatb_param_filterArray_94, __xlx_apatb_param_filterArray_95, __xlx_apatb_param_filterArray_96, __xlx_apatb_param_filterArray_97, __xlx_apatb_param_filterArray_98, __xlx_apatb_param_filterArray_99, __xlx_apatb_param_filterArray_100, __xlx_apatb_param_filterArray_101, __xlx_apatb_param_filterArray_102, __xlx_apatb_param_filterArray_103, __xlx_apatb_param_filterArray_104, __xlx_apatb_param_filterArray_105, __xlx_apatb_param_filterArray_106, __xlx_apatb_param_filterArray_107, __xlx_apatb_param_filterArray_108, __xlx_apatb_param_filterArray_109, __xlx_apatb_param_filterArray_110, __xlx_apatb_param_filterArray_111, __xlx_apatb_param_filterArray_112, __xlx_apatb_param_filterArray_113, __xlx_apatb_param_filterArray_114, __xlx_apatb_param_filterArray_115, __xlx_apatb_param_filterArray_116, __xlx_apatb_param_filterArray_117, __xlx_apatb_param_filterArray_118, __xlx_apatb_param_filterArray_119, __xlx_apatb_param_filterArray_120, __xlx_apatb_param_filterArray_121, __xlx_apatb_param_filterArray_122, __xlx_apatb_param_filterArray_123, __xlx_apatb_param_filterArray_124, __xlx_apatb_param_filterArray_125, __xlx_apatb_param_filterArray_126, __xlx_apatb_param_filterArray_127, __xlx_apatb_param_bitmask1, __xlx_apatb_param_bitmask2);
}
