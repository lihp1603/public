/* 说明：
 * 用于进程重命名，主进程、子进程使用不同的命令，便于命令ps -ef查看。
 * from:httpsqs
 */
#ifndef __PRENAME_H_
#define __PRENAME_H_

#ifdef __cplusplus
extern "C" {
#endif

/*extern*/ void prename_setproctitle_init(int argc, char **argv, char **envp);
/*extern*/ void prename_setproctitle(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
