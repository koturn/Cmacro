/*!
 * @brief コンパイラや環境間の互換性のためのヘッダファイル
 *
 * Visual C++用のセキュアな関数と、<br>
 * その他のコンパイラで互換性を持たせるためのヘッダファイル。<br>
 * 完全な互換性はないので、注意すること。
 *
 * @author    koturn 0;
 * @date      2013 05/20
 * @file      compat.h
 * @version   0.1.5.0
 * @attention 安全ではない置き換えがあるので、注意すること
 */
#ifndef COMPAT_H
#define COMPAT_H


// Visual C++ (2005以降) のコンパイラならば
#if _MSC_VER >= 1400 && _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES != 1 && !defined(_CRT_SECURE_NO_WARNINGS)
#define SCAN_S_ARG(arg)   (arg), (_countof(arg))

/* ------------------------------------------------------------
 * 入出力関係
 * ------------------------------------------------------------ */
#define printf(fmt, ...)           printf_s(fmt, ##__VA_ARGS__)
#define gets(dst)                  gets_s(dst, _countof(dst) - 1)
#define FOPEN(fp, filename, mode)  (fopen_s(&(fp), filename, mode), fp)


/* ------------------------------------------------------------
 * 文字列関係
 * dstに当たる部分が静的配列のみであれば、AUTO_SECUREマクロを
 * 予め定義しておくこと
 * ------------------------------------------------------------ */
#ifdef AUTO_SECURE
#  define memcpy(dst, src, count)   (memcpy_s(dst, _countof(dst), src, count), (void *)dst)
#  define memmove(dst, src, count)  (memmove_s(dst, _countof(dst), src, count), (void *)dst)
#  define strcpy(dst, src)          (strcpy_s(dst, _countof(dst), src), dst)
#  define strcat(dst, src)          (strcat_s(dst, _countof(dst), src), dst)
#  define strncat(dst, src, count)  (strncat_s(dst, _countof(dst), src, _TRUNCATE), dst)
#  define strncpy(dst, src, count)  (strncpy_s(dst, _countof(dst), src, _TRUNCATE), dst)
#  define strlwr(str)               (_strlwr_s(str, _countof(str)), str)
#  define strupr(str)               (_strupr_s(str, _countof(str)), str)
#endif




#else
typedef int errno_t;
#define __DUMMY_ERR_NO_OK__   ((errno_t) 0)  //!< ダミーのエラーナンバー(0はエラー無しの意味)
#define __DUMMY_ERR_NO_ERR__  ((errno_t) 1)  //!< ダミーのエラーナンバー(1はエラー有りの意味)


/* ------------------------------------------------------------
 * 入出力関係
 * ------------------------------------------------------------ */
#define printf_s(fp, fmt, ...)                 \
  printf(fp, fmt, ##__VA_ARGS__)

#define fprintf_s(fp, fmt, ...)                \
  fprintf(fp, fmt, ##__VA_ARGS__)

#define fopen_s(fpp, filename, mode)           \
  (*(fpp) = fopen(filename, mode), *(fpp) == NULL ? __DUMMY_ERR_NO_ERR__ : __DUMMY_ERR_NO_OK__)

#define gets_s(dst, dst_size)                  \
  fgets(dst, dst_size, stdin)

#define sprintf_s(dst, dst_size, fmt, ...)     \
  sprintf(dst, fmt, ##__VA_ARGS__)

#define getenv_s(ret, dst, dst_size, varname)  \
  strcpy(dst, getenv(varname))


/* ------------------------------------------------------------
 * scanf関係(可変引数部分でサイズ指定の必要がなければ、問題はない)
 * 可変部分でサイズ指定が無い場合、AUTO_UNSECUREマクロを
 * 予め定義しておくこと
 * ------------------------------------------------------------ */
#define SCAN_S_ARG(arg)   (arg)


#ifdef AUTO_UNSECURE
#  define fscanf_s(fp, fmt, ...)       \
     fscanf(fp, fmt, ##__VA_ARGS__)

#  define scanf_s(fmt, dst, dst_size)  \
     scanf(fmt, dst)

#  define sscanf_s(str, fmt, ...)      \
     sscanf(str, fmt, ##__VA_ARGS__)
#endif


/* ------------------------------------------------------------
 * 文字列関係
 * ------------------------------------------------------------ */
#define memcpy_s(dst, dst_size, src, count)   \
  (memcpy(dst, src, count), __DUMMY_ERR_NO_OK__)

#define memmove_s(dst, dst_size, src, count)  \
  (memmove(dst, src, count), __DUMMY_ERR_NO_OK__)

#define strcat_s(dst, dst_size, src)          \
  (strcat(dst, src), __DUMMY_ERR_NO_OK__)

#define strcpy_s(dst, dst_size, src)          \
  (strcpy(dst, src), __DUMMY_ERR_NO_OK__)

#define strncat_s(dst, dst_size, src, count)  \
  (strncat(dst, src, count), __DUMMY_ERR_NO_OK__)

#define strncpy_s(dst, dst_size, src, count)  \
  (strncpy(dst, src), __DUMMY_ERR_NO_OK__)

#define strtok_s(str, demimiter, next_token)  \
  strtok(str, delimiter)

#define _strlwr_s(str, dst_size)              \
  (strlwr(str), __DUMMY_ERR_NO_OK__)

#define _strupr_s(str, dst_size)              \
  (strupr(str), __DUMMY_ERR_NO_OK__)


/* ------------------------------------------------------------
 * 乱数
 * ------------------------------------------------------------ */
#ifndef _CRT_RAND_S
#  define _CRT_RAND_S
#endif
#define rand_s(rand_val)  \
  (rand(rand_val), *(rand_val))

#endif




// Windowsならば、(Visual C++ / MinGW-gcc, g++)
#if defined(WIN16) || defined(_WIN16) || defined(__WIN16) || defined(__WIN16__)   \
  || defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)  \
  || defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
   // require <Windows.h>
#  define sleep(sec)  Sleep((sec) * 1000)  //!< Windwos向けのsleep()関数の代理
#endif


#if !defined(__cplusplus) && __STDC_VERSION__ < 199901L && !defined(__GNUC__)
#  define inline  // inline指定が使えない処理系では、inline指定を消す
#endif


#endif
