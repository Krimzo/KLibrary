#include "klib.h"


/* Source of dll_test_1.dll
#include <iostream>
#include <windows.h>

#define DLL_EXPORT(return_type) extern "C" return_type __declspec(dllexport) __stdcall

DLL_EXPORT(void) script_start() {
	printf("It started!\n");
}

DLL_EXPORT(void) script_update(int index) {
	printf("Index is: %d\n", index);
}
*/

int main()
{
	kl::dll loaded_dll = kl::dll("examples/dlls/dll_test_1.dll");
	if (kl::warning_check(!loaded_dll, "Failed to load dll")) {
		return 1;
	}

	kl::dll::function<void> script_start = loaded_dll.get_function<void>("script_start");
	kl::dll::function<void, int> script_update = loaded_dll.get_function<void, int>("script_update");

	if (script_start) {
		script_start();
	}

	if (script_update) {
		for (int i = 0; i < 5; i++) {
			script_update(i);
		}
	}

	kl::get();
}
