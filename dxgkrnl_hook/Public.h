/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_dxgkrnlhook,
    0x517caa08,0xa850,0x475a,0x9f,0xa6,0x69,0xf5,0x03,0x20,0x72,0x27);
// {517caa08-a850-475a-9fa6-69f503207227}
