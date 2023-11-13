#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#define CAMERA_BINDING              0
#define MODEL_BINDING               1
#define DRAW_ID_2_MODEL_IDX_BINDING 2
#define TEXTURE_BINDING             3
#define INDIRECT_BINDING            4
#define ATOMIC_COUNTER_BINDING      5

#define WORK_GROUP_LOCAL_SIZE_X 16
#define WORK_GROUP_LOCAL_SIZE_Y 2
#define WORK_GROUP_LOCAL_SIZE_Z 1

#define MAX_MODELS (1 << 24)

#endif  // COMMON_DEFS_H