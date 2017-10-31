#ifndef NAMETOHASH_H
#define NAMETOHASH_H

#define LEGACY_SEED -1

#ifdef __cplusplus
extern "C" {
#endif

int convertNameToHash(const char* name, char* hash, int encodeSeed);

#ifdef __cplusplus
}
#endif

#endif
