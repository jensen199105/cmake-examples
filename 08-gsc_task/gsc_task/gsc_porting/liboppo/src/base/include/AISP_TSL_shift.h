#ifndef __AISP_TSL_SHIFT_H__
#define __AISP_TSL_SHIFT_H__

#ifndef __AISP_TSL_BASE_H__
#error "forbid to include this header directly"
#endif

#define SHL(a, shift)                   ((a) << (shift))
#define SHR(a, shift)                   ((a) >> (shift))
#define PSHR_POSITIVE(a, shift)         SHR((a)+SHL(1,(shift)-1), shift)
#define PSHR_NEGTIVE(a, shift)          SHR((a)+SHL(1,(shift)-1)-1, shift)
#define PSHR(a, shift)                  ((a) >=0 ? PSHR_POSITIVE(a, shift) : PSHR_NEGTIVE(a, shift))

#endif
