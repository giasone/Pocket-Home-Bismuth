#define LIB_NM_THREAD_IMPLEMENTATION
#include "LibNM/ThreadHandler.h"
#include "LibNM/ThreadResource.h"

/*
 * Creates the shared LibNM::ThreadResource if it doesn't already exist.
 */
LibNM::ThreadHandler::ThreadHandler() :
GLib::ThreadHandler(ThreadResource::resourceKey, 
        []()->GLib::ThreadResource* { return new ThreadResource(); } ) { }

/*
 * Gets the shared NetworkManager client object if called within the LibNM event
 * loop.
 */
LibNM::Client LibNM::ThreadHandler::getClient() const
{
    SharedResource::LockedPtr<ThreadResource> nmThread
            = getWriteLockedResource<ThreadResource>();
    return nmThread->getClient();
}

/*
 * Gets the shared DeviceWifi object used to control the LibNM-managed Wifi 
 * device if called within the LibNM event loop.
 */
LibNM::DeviceWifi LibNM::ThreadHandler::getWifiDevice() const
{
    SharedResource::LockedPtr<ThreadResource> nmThread
            = getWriteLockedResource<ThreadResource>();
    return nmThread->getWifiDevice();
}
