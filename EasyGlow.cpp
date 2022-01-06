#include <Windows.h>

struct offsets {
	uintptr_t dwGlowObjectManager;
	uintptr_t m_iGlowIndex;
	uintptr_t dwEntityList;
	uintptr_t dwLocalPlayer;
	uintptr_t m_bDormant;
	uintptr_t m_iTeamNum;
} offsets;


DWORD WINAPI fMain(HMODULE hMod)
{
	//Getting module handle
	uintptr_t client = (uintptr_t)GetModuleHandle(TEXT("client.dll"));
	uintptr_t engine = (uintptr_t)GetModuleHandle(TEXT("engine.dll"));

	offsets.dwGlowObjectManager = 0x52E81B0;
	offsets.m_iGlowIndex = 0xA438;
	offsets.dwEntityList = 0x4D9FBD4;
	offsets.dwLocalPlayer = 0xD882BC;
	offsets.m_bDormant = 0xED;
	offsets.m_iTeamNum = 0xF4;

	uintptr_t pLocalPlayer = client + offsets.dwLocalPlayer;
	uintptr_t pGlowObjectManager = client + offsets.dwGlowObjectManager;
	uintptr_t pEntityList = client + offsets.dwEntityList;

	while (true)
	{
		uintptr_t LocalPlayer = *(uintptr_t*)pLocalPlayer;
		uintptr_t EntityList = *(uintptr_t*)pEntityList;

		if (LocalPlayer != NULL && EntityList != NULL)
		{
			int playerTeamNum = *(int*)(LocalPlayer + offsets.m_iTeamNum);

			//Check entity
			for (short i = 0; i < 64; i++)
			{
				uintptr_t GlowObjectManager = *(uintptr_t*)pGlowObjectManager;

				uintptr_t entity = *(uintptr_t*)(pEntityList + i * 0x10);

				if (entity == NULL) continue;
				if (*(bool*)(entity + offsets.m_bDormant)) continue;

				int glowIndex = *(int*)(entity + offsets.m_iGlowIndex);
				int entityTeamNum = *(int*)(entity + offsets.m_iTeamNum);

				if (entityTeamNum == playerTeamNum)
				{
					/*
					//teammate
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0x4)) = 0.2f; //r
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0x8)) = 0.5f; //g
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0xC)) = 0.9f; //b
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0x10)) = 0.5f; //a
					*/
				}
				else
				{
					//enemy
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0x4)) = 0.6f; //r
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0x8)) = 0.2f; //g
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0xC)) = 0.0f; //b
					*(float*)((GlowObjectManager)+((glowIndex * 0x38) + 0x10)) = 1.0f; //a
				}
				*(bool*)((GlowObjectManager)+((glowIndex * 0x38) + 0x24)) = true;
				*(bool*)((GlowObjectManager)+((glowIndex * 0x38) + 0x25)) = false;
			}
		}
		Sleep(10);
	}
	FreeLibraryAndExitThread(hMod, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hMod, DWORD fdwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hMod);
	if (fdwReason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)fMain, hMod, 0, nullptr);

	return TRUE;
}