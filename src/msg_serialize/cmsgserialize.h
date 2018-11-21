/*
 * 使用msgpack-c实现序列化和反序列化，使用版本v2.1.5
 * author:lihaiping1603@aliyun.com
 * date:20170927
*/

#ifndef __CMSGSERIALIZE_H_
#define __CMSGSERIALIZE_H_

//#include <msgpack.hpp>

#include <string>
#include <sstream>

using namespace std;


//适用于使用侵入式的方式实现对自定义数据结构的序列化和反序列化操作
//#define CMSGPACK_DEFINE_PUBLIC MSGPACK_DEFINE
//#define CMSGPACK_ADD_ENUM_PUBLIC MSGPACK_ADD_ENUM


//template<typename T>
//inline void CMsgPack_UnPack_Raw(const string msg,T &rawObj)
//{
//    try{
//        msgpack::object_handle oh = msgpack::unpack(msg.data(), msg.size());

//        // deserialized object is valid during the msgpack::object_handle instance is alive.
//        msgpack::object obj = oh.get();

//        //这种方法对于带深拷贝的数据结构不适合使用
////        T tmptt;
////        obj.convert(tmptt);
////        return tmptt;

//        //所以使用传递引用的方式
//        obj.convert(rawObj);
//    }
//    catch(...){
//        //出现异常
//    }
//}

//template<typename T>
//inline string CMsgPack_Pack(T &rawObj)
//{
//    std::stringstream buffer;
//    msgpack::pack(buffer, rawObj);
//    buffer.seekg(0);

//    // deserialize the buffer into msgpack::object instance.
//    return string(buffer.str());
//}



#endif // CMSGSERIALIZE_H
