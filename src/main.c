#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>

#include <psp2/appmgr.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <taihen.h>


#define HOOKS_NUM       10

static SceUID g_hooks[HOOKS_NUM];
static tai_hook_ref_t ref[HOOKS_NUM];
static uint8_t cur_hook = 0;

static char titleid[16];
static char ctid[16];

void hookFunction(uint32_t nid, const void* func){
	g_hooks[cur_hook] = taiHookFunctionImport(&ref[cur_hook], TAI_MAIN_MODULE, TAI_ANY_LIBRARY, nid, func);
	cur_hook++;
}




SceUID return1(void) {

  return 1;

}





void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {


	sceAppMgrAppParamGetString(0, 12, titleid , 256);
	snprintf(ctid, 9, "%s", titleid);


	if((strlen(ctid) == 9) && ((strncmp(ctid, "NPXS10031", 9) == 0) || (strncmp(ctid, "SKGPKG2CH", 9) == 0))){

		//vshSysconIsIduMode
		hookFunction(0xE493EFF4,return1);

	}


	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {


	int i;
	for (i = 0; i < HOOKS_NUM; i++){
		taiHookRelease(g_hooks[i], ref[i]);
	}


	return SCE_KERNEL_STOP_SUCCESS;
}