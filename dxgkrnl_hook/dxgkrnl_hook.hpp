#pragma once

namespace dxgkrnl_hook
{
	extern bool is_hooked;

	constexpr auto target_name = "RainbowSix.exe";

	// HOOKERS
	bool hook_submit_command();
	bool unhook_submit_command();

	// HOOK INFO
	using dxgk_submit_command_t = int64_t(__fastcall*)(D3DKMT_SUBMITCOMMAND* data);
	extern dxgk_submit_command_t original_submit_command;
	extern dxgk_submit_command_t* original_entry;

	// NATIVE HELPERS
	dxgkrnl_hook::dxgk_submit_command_t* find_submit_command_entry();

	// HOOK HANDLER
	int64_t __fastcall submit_command_hook(D3DKMT_SUBMITCOMMAND* data);
}