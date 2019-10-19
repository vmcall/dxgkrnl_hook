#include "dxgkrnl_hook.hpp"

bool dxgkrnl_hook::is_hooked = false;

dxgkrnl_hook::dxgk_submit_command_t dxgkrnl_hook::original_submit_command = nullptr;
dxgkrnl_hook::dxgk_submit_command_t* dxgkrnl_hook::original_entry = nullptr;

bool dxgkrnl_hook::hook_submit_command()
{
	// SAVE ORIGINAL ENTRY
	dxgkrnl_hook::original_entry = dxgkrnl_hook::find_submit_command_entry();

	if (dxgkrnl_hook::original_entry == nullptr)
	{
		DbgPrint("Failed to find NtGdiDdDDISubmitCommand\n");
		return false;
	}

	// SAVE ORIGINAL FUNCTION POINTER
	dxgkrnl_hook::original_submit_command = *dxgkrnl_hook::original_entry;

	if (dxgkrnl_hook::original_submit_command == nullptr)
	{
		DbgPrint("Failed to find DxgkSubmitCommand\n");
		return false;
	}

	// HOOK
	*dxgkrnl_hook::original_entry = dxgkrnl_hook::submit_command_hook;

	// SAVE STATE
	dxgkrnl_hook::is_hooked = true;

	DbgPrint("Hooked DxgkSubmitCommand!\n");

	return true;
}

bool dxgkrnl_hook::unhook_submit_command()
{
	if (!dxgkrnl_hook::is_hooked)
		return false;

	// UNHOOK
	*dxgkrnl_hook::original_entry = dxgkrnl_hook::original_submit_command;

	DbgPrint("Unhooked DxgkSubmitCommand!\n");

	return true;
}

dxgkrnl_hook::dxgk_submit_command_t* dxgkrnl_hook::find_submit_command_entry()
{
	uint8_t* submit_command_address = reinterpret_cast<uint8_t*>(NtGdiDdDDISubmitCommand);

	// FIND MOV INSTRUCTION
	auto instruction = submit_command_address;
	for (;
		instruction[0] != 0x48 ||
		instruction[1] != 0x8B ||
		instruction[2] != 0x05;
		instruction++)
	{
		//:)
	}

	// mov rax,QWORD PTR [rip+0x????????]
	// 48 8B 05 ?? ?? ?? ??
	auto delta = *reinterpret_cast<int32_t*>(instruction + 3);
	auto result = reinterpret_cast<dxgkrnl_hook::dxgk_submit_command_t*>(instruction + delta + 7);

	DbgPrint("DxgkSubmitCommand: %p\n", result);

	return result;
}

int64_t __fastcall dxgkrnl_hook::submit_command_hook(D3DKMT_SUBMITCOMMAND * data)
{
	const auto current_process = IoGetCurrentProcess();
	const auto process_name = PsGetProcessImageFileName(current_process);

	if (memeq(process_name, dxgkrnl_hook::target_name))
	{
		// GET CONTEXT
		const auto ctx = NtUserGetDc(0x00);

		// DRAW TO GAME WINDOW BUFFER
		NtGdiPatBlt(ctx, 15, 15, 5, 5, PATCOPY);
	}

	return dxgkrnl_hook::original_submit_command(data);
}
