#ifndef __MD5_H
#define __MD5_H

#ifdef _EXTERN_INC_
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
#endif

void Md5Maker(const void *, int nbytes, char *md5_data);
/* pur @ 生成md5字符串
 * para @ olddata 原始字符串以\0结尾
 * para @ md5_data 返回生成的md5字符串
 * return @
 */
void Md5Maker(const char *olddata, char *md5_data);

#ifdef _EXTERN_INC_
#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
#endif

#endif // __MD5_H
