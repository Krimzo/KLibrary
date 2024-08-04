#include "examples.h"


/* Source of dll_test_1.dll
#include <iostream>
#include <windows.h>

#define DLL_EXPORT(return_type) extern "C" return_type __declspec(dllexport) __stdcall

DLL_EXPORT(void) script_start()
{
	printf("It started!\n");
}

DLL_EXPORT(void) script_update(int index)
{
	printf("Index is: %d\n", index);
}
*/

int examples::dynamic_linking_main(const int argc, const char** argv)
{
	kl::DLL loaded_dll = kl::DLL("dlls/dll_test_1.dll");
	if (!verify(loaded_dll, "Failed to load dll")) {
		return 1;
	}

	kl::DLL::Function<void> script_start = loaded_dll.read_function<void>("script_start");
	kl::DLL::Function<void, int> script_update = loaded_dll.read_function<void, int>("script_update");
	if (!script_start || !script_update) {
		return 2;
	}

	script_start();
	for (int i = 0; i < 5; i++) {
		script_update(i);
	}
	return 0;
}
