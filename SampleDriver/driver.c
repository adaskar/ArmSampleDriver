#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#endif

#include <ntddk.h>

#define BAIL_NT(status) \
	do { \
		if (!NT_SUCCESS(status)) { \
			KdPrint(("Failed with status: (0x%08X). File: %s. Line: %d\n", status, __FILE__, __LINE__)); \
			goto exit; \
		} \
	} while (0)

#define BAIL_MY(s, st) \
	do { \
		if (!(s)) { \
			KdPrint(("Failed with status: (0x%08X). File: %s. Line: %d\n", st, __FILE__, __LINE__)); \
			status = st; \
			goto exit; \
		} \
	} while (0)

#define CompleteIRP(status, info) \
	do { \
		Irp->IoStatus.Status = status; \
		Irp->IoStatus.Information = info; \
		IoCompleteRequest(Irp, IO_NO_INCREMENT); \
		return status; \
	} while (0)

UNICODE_STRING g_DeviceName = RTL_CONSTANT_STRING(L"\\Device\\ArmSampleDriver");
UNICODE_STRING g_DeviceLink = RTL_CONSTANT_STRING(L"\\??\\ArmSampleDriver");

VOID
DriverUnload(
    IN PDRIVER_OBJECT DriverObject
)
{
    KdPrint(("ArmSampleDriver DriverUnload\n"));

    IoDeleteSymbolicLink(&g_DeviceLink);
    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{
	UNREFERENCED_PARAMETER(RegistryPath);
	KdPrint(("ArmSampleDriver DriverEntry\n"));

	NTSTATUS status;
	PDEVICE_OBJECT deviceObject = NULL;

	status = IoCreateDevice(
		DriverObject,
		0,
		&g_DeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&deviceObject);
	BAIL_NT(status);

	deviceObject->Flags |= DO_DIRECT_IO;

	status = IoCreateSymbolicLink(&g_DeviceLink, &g_DeviceName);
	BAIL_NT(status);

	DriverObject->DriverUnload = DriverUnload;

	//KeBugCheck(0xCCCCCCCC);

	//DriverObject->MajorFunction[IRP_MJ_CREATE] = ;
	//DriverObject->MajorFunction[IRP_MJ_CLOSE] = ;
	//DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ;

exit:
	if (!NT_SUCCESS(status)) {
		if (deviceObject) {
			IoDeleteDevice(deviceObject);
		}
	}

	return status;
}
