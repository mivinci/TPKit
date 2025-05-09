#ifndef TPKIT_H_
#define TPKIT_H_

#include "API/Transport.h"

struct TPKit_Version {
  int major;
  int minor;
  int patch;
  int build_no;
  const char branch[64];
};

typedef struct TPKit_Version TPKit_Version;

void TPKit_GetVersion(TPKit_Version *);

#endif  // TPKIT_H_
