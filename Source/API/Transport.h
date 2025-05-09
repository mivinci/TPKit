#ifndef TPKIT_API_TRANSPORT_H_
#define TPKIT_API_TRANSPORT_H_

#include <stddef.h>
#include <stdint.h>

#include "API/Export.h"

#ifndef __cplusplus
extern "C" {
#endif  // __cplusplus

enum TPKit_Transport_TaskKind {
  TPKit_Transport_TaskKind_Auto = 0,
};

enum TPKit_Transport_TaskEventKind {
  TPKit_Transport_TaskEventKind_Error = 0,
  TPKit_Transport_TaskEventKind_Done,
  TPKit_Transport_TaskEventKind_Progress,
  TPKit_Transport_TaskEventKind_Suspend,
  TPKit_Transport_TaskEventKind_Resume,
  TPKit_Transport_TaskEventKind_Start,
  TPKit_Transport_TaskEventKind_Stop,
};

typedef enum TPKit_Transport_TaskKind TPKit_Transport_TaskKind;
typedef enum TPKit_Transport_TaskEventKind TPKit_Transport_TaskEventKind;
typedef struct TPKit_Transport_TaskListener TPKit_Transport_TaskListener;
typedef struct TPKit_Transport_TaskEvent TPKit_Transport_TaskEvent;

struct TPKit_Transport_TaskEvent {
  int32_t task_id;
  int32_t clip_no;
  TPKit_Transport_TaskEventKind kind;
};

struct TPKit_Transport_TaskListener {
  void (*OnTaskEvent)(TPKit_Transport_TaskEvent);
};

TPKIT_API int TPKit_Transport_Init();
TPKIT_API int TPKit_Transport_Drop();
TPKIT_API int32_t TPKit_Transport_CreateTask(TPKit_Transport_TaskKind,
                                           TPKit_Transport_TaskListener *);
TPKIT_API int TPKit_Transport_StopTask(int32_t);
TPKIT_API int TPKit_Transport_SuspendTask(int32_t);
TPKIT_API int TPKit_Transport_ResumeTask(int32_t);
TPKIT_API int TPKit_Transport_GetProxyURL(int32_t, const char *);

#ifndef __cplusplus
}
#endif  // __cplusplus

#endif  // TPKIT_API_TRANSPORT_H_
