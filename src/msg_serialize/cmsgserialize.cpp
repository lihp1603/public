/*
 * 使用msgpack-c实现序列化和反序列化，使用版本v2.1.5
 * author:lihaiping1603@aliyun.com
 * date:20170927
*/


#include "cmsgserialize.h"
//#include <msgpack.hpp>
#include <sstream>

//namespace PublicTool {

//    template<typename T>
//    void CMsgPack_UnPack_Raw(const string msg,T &rawObj)
//    {
//        try{
//            msgpack::object_handle oh = msgpack::unpack(msg.data(), msg.size());

//            // deserialized object is valid during the msgpack::object_handle instance is alive.
//            msgpack::object obj = oh.get();

//            //这种方法对于带深拷贝的数据结构不适合使用
//    //        T tmptt;
//    //        obj.convert(tmptt);
//    //        return tmptt;

//            //所以使用传递引用的方式
//            obj.convert(rawObj);
//        }
//        catch(...){
//            //出现异常
//        }
//    }

//    template<typename T>
//    string CMsgPack_Pack(T &rawObj)
//    {
//        std::stringstream buffer;
//        msgpack::pack(buffer, rawObj);
//        buffer.seekg(0);

//        // deserialize the buffer into msgpack::object instance.
//        return string(buffer.str());
//    }

//}
