#include <stdint.h>
#include "dxgkrnl_hook.hpp"

extern "C" int64_t DriverEntry(void* ctx)
{
	dxgkrnl_hook::hook_submit_command();

	return 1;
}