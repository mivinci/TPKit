#ifndef TPKIT_API_ERRNO_H_
#define TPKIT_API_ERRNO_H_

enum TPKit_Errno {
  TPKit_Errno_Ok = 0,
  TPKit_Errno_Unknown,
  TPKit_Errno_NullPointer,
  TPKit_Errno_InvalidArgument,
  TPKit_Errno_OutOfMemory,
  TPKit_Errno_Timeout,
  TPKit_Errno_NotFound,
  TPKit_Errno_AlreadyExists,
  TPKit_Errno_PermissionDenied,
  TPKit_Errno_OperationFailed,
};

typedef enum TPKit_Errno TPKit_Errno;

#endif // TPKIT_API_ERRNO_H_
