#ifndef _ERROR_H
#define _ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

void error_init(void);
void error_trace(const char *sModule, const char *sFunction, int modErr);

#ifdef __cplusplus
}
#endif

#endif
