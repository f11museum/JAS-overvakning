
#include "jasovervakning.h"
#include "datarefs.h"
#include "config.h"

// Downloaded from https://developer.x-plane.com/code-sample/hello-world-sdk-3/

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include <string.h>

//#ifndef XPLM300
//      #error This is made to be compiled against the XPLM300 SDK
//#endif

// An opaque handle to the window we will create

#ifdef XPLM301
static XPLMWindowID g_window;

#endif
static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

XPLMDataRef dr_heartbeat_vat;
XPLMDataRef dr_heartbeat_ess;
XPLMDataRef dr_heartbeat_logic;
XPLMDataRef dr_heartbeat_hud;

XPLMDataRef dr_io_vat_lamp_dator;
XPLMDataRef dr_io_vat_lamp_primdat;

int pv_vat = 0;
int pv_ess = 0;
int pv_logic = 0;
int pv_hud = 0;

int timer_vat = 0;
int timer_ess = 0;
int timer_logic = 0;
int timer_hud = 0;

int findDataRef(const char* name, XPLMDataRef* result) {
    *result = XPLMFindDataRef(name);
    if (*result == NULL) {
        debugLog("Error finding XPL variable ");
        debugLog(name);
        debugLog("\n");
        return -1;
    }
    return 0;
}

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
    strcpy(outName, "jasovervakning");
    strcpy(outSig, "github.condac.jasovervakning");
    strcpy(outDesc, "A plug-in for jasovervakning.");

    // Menu
    XPLMMenuID myMenu;
    int mySubMenuItem;

    /* First we put a new menu item into the plugin menu.
     * This menu item will contain a submenu for us. */
    mySubMenuItem = XPLMAppendMenuItem(XPLMFindPluginsMenu(), /* Put in plugins menu */
                                       "jasovervakning",      /* Item Title */
                                       0,                     /* Item Ref */
                                       1);                    /* Force English */

    /* Now create a submenu attached to our menu item. */
    myMenu = XPLMCreateMenu("jasovervakning",
                            XPLMFindPluginsMenu(),
                            mySubMenuItem,         /* Menu Item to attach to. */
                            MyMenuHandlerCallback, /* The handler */
                            0);                    /* Handler Ref */
    XPLMAppendMenuItem(myMenu, "Loaded 1", (void*)1, 1);
    XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, /* Callback */
                                   1.0,                  /* Interval */
                                   NULL);                /* refcon not used. */

    findDataRef("JAS/system/vat/heartbeat", &dr_heartbeat_vat);
    findDataRef("JAS/system/ess/heartbeat", &dr_heartbeat_ess);
    findDataRef("JAS/system/logic/heartbeat", &dr_heartbeat_logic);
    findDataRef("JAS/system/hud/heartbeat", &dr_heartbeat_hud);
    findDataRef("JAS/io/vat/lamp/dator", &dr_io_vat_lamp_dator);
    findDataRef("JAS/io/vat/lamp/primdat", &dr_io_vat_lamp_primdat);

    return 1; //g_window != NULL;
}

PLUGIN_API void XPluginStop(void) {
    // Since we created the window, we'll be good citizens and clean it up
#ifdef XPLM301
    XPLMDestroyWindow(g_window);
    g_window = NULL;
#endif
}

PLUGIN_API void XPluginDisable(void) {
}
PLUGIN_API int XPluginEnable(void) {
    return 1;
}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {
}

void MyMenuHandlerCallback(void* inMenuRef, void* inItemRef) {
    //display("%d",(int)inItemRef);
    int incommand = (uintptr_t)inItemRef;
    if (incommand == 1) {
    }
}

int checkHeartBeat(int heartbeat, int* heartbeat_prev, int* heartbeat_timer) {
    if (heartbeat != *heartbeat_prev) {
        *heartbeat_prev = heartbeat;
        *heartbeat_timer = 0;
    } else {

        *heartbeat_timer = *heartbeat_timer + 1;
        if (*heartbeat_timer > 20) {
            return 1;
        }
    }
    return 0;
}

float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon) {
    //float elapsed = XPLMGetElapsedTime();
    /* The actual callback.  First we read the sim's time and the data. */
    //XPLMDebugString("jasovervakning: flightloop\n");
    int res;
    res = checkHeartBeat(XPLMGetDatai(dr_heartbeat_vat), &pv_vat, &timer_vat);
    res += checkHeartBeat(XPLMGetDatai(dr_heartbeat_ess), &pv_ess, &timer_ess);
    res += checkHeartBeat(XPLMGetDatai(dr_heartbeat_logic), &pv_logic, &timer_logic);
    res += checkHeartBeat(XPLMGetDatai(dr_heartbeat_hud), &pv_hud, &timer_hud);

    if (res > 0) {
        XPLMSetDatai(dr_io_vat_lamp_dator, 1);
    } else {
        XPLMSetDatai(dr_io_vat_lamp_dator, 0);
    }

    XPLMSetDatai(dr_io_vat_lamp_primdat, 1);
    /* Return 1.0 to indicate that we want to be called again in 1 second. */
    return 0.1;
}
