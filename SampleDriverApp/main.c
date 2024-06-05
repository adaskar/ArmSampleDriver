#include <windows.h>

int __cdecl
wmain(
    _In_ int argc,
    _In_reads_(argc) wchar_t* argv[]
    )
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    goto exit;

   /* hDevice = CreateFile(L"G_DevicePath",
                         GENERIC_READ|GENERIC_WRITE,
                         FILE_SHARE_READ | FILE_SHARE_WRITE,
                         NULL,
                         OPEN_EXISTING,
                         0,
                         NULL );

    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Failed to open device. Error %d\n",GetLastError());
        goto exit;
    }

    printf("Opened device successfully\n");*/

exit:
    if (hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(hDevice);
    }

    return 0;

}
