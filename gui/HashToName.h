#ifndef HASHTONAME_H
#define HASHTONAME_H

#ifdef __cplusplus
extern "C" {
#endif

int getAssociatedDataFileNumber(const char *hash);
int convertHashToName(const char *hash, char *name);

#ifdef __cplusplus
}
#endif

#endif
