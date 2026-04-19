/*******************************************************************************

SPDX-License-Identifier: BSD-2-Clause-Patent

******************************************************************************/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/FileHandleLib.h>
#include <Protocol/NorFlashProtocol.h>
#include <Protocol/FileExplorer.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

EFI_STATUS
EFIAPI
Main (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                  Status;
  UNI_NOR_FLASH_PROTOCOL       *SpiFlash;
  EFI_FILE_EXPLORER_PROTOCOL  *FileExplorer;
  EFI_DEVICE_PATH_PROTOCOL    *SelectedFile;
  EFI_FILE_PROTOCOL          *File;
  UINT8                      *Buffer;
  UINTN                      FileSize;
  UINTN                      ReadSize;
  EFI_FILE_INFO              *FileInfo;
  UINTN                      BufferSize;

  SelectedFile = NULL;
  Buffer = NULL;
  File = NULL;
  FileInfo = NULL;
  FileSize = 0;

  Print (L"Firmware Update Application\n");
  Print (L"==============================\n\n");

  Status = gBS->LocateProtocol (
            &gUniNorFlashProtocolGuid,
            NULL,
            (VOID **)&SpiFlash
            );
  if (EFI_ERROR (Status)) {
    Print (L"Error: Cannot locate SpiFlash protocol\n");
    return Status;
  }

  Status = gBS->LocateProtocol (
            &gEfiFileExplorerProtocolGuid,
            NULL,
            (VOID **)&FileExplorer
            );
  if (EFI_ERROR (Status)) {
    Print (L"Error: Cannot locate File Explorer protocol\n");
    return Status;
  }

  Print (L"Select firmware image file (.img)...\n\n");

  Status = FileExplorer->ChooseFile (
            NULL,
            L".img",
            NULL,
            &SelectedFile
            );
  if (EFI_ERROR (Status)) {
    Print (L"Error selecting file - %r\n", Status);
    return Status;
  }

  if (SelectedFile == NULL) {
    Print (L"No file selected.\n");
    return EFI_ABORTED;
  }

  Print (L"File selected. Reading file...\n");

  Status = EfiOpenFileByDevicePath (
            &SelectedFile,
            &File,
            EFI_FILE_MODE_READ,
            0
            );
  if (EFI_ERROR (Status)) {
    Print (L"Error: Cannot open file - %r\n", Status);
    goto Exit;
  }

  BufferSize = SIZE_OF_EFI_FILE_INFO + 32;
  FileInfo = AllocatePool (BufferSize);
  if (FileInfo == NULL) {
    Print (L"Error: Cannot allocate memory\n");
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = File->GetInfo (
              File,
              &gEfiFileInfoGuid,
              &BufferSize,
              FileInfo
              );
  if (EFI_ERROR (Status)) {
    Print (L"Error: Cannot get file info - %r\n", Status);
    goto Exit;
  }

  FileSize = FileInfo->FileSize;
  FreePool (FileInfo);
  FileInfo = NULL;

  Print (L"File size: %d bytes\n", FileSize);

  Buffer = AllocatePool (FileSize);
  if (Buffer == NULL) {
    Print (L"Error: Cannot allocate memory\n");
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  ReadSize = FileSize;
  Status = File->Read (File, &ReadSize, Buffer);
  if (EFI_ERROR (Status) || (ReadSize != FileSize)) {
    Print (L"Error: Cannot read file - %r\n", Status);
    goto Exit;
  }

  File->Close (File);
  File = NULL;

  Print (L"Flashing firmware to SPI...\n");

  Status = SpiFlash->Update (SpiFlash, 0, Buffer, FileSize);
  if (EFI_ERROR (Status)) {
    Print (L"Error: Cannot update flash - %r\n", Status);
    goto Exit;
  }

  Print (L"Firmware updated successfully.\n");
  Print (L"Rebooting...\n");

  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

Exit:
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }

  if (File != NULL) {
    File->Close (File);
  }

  FreePool (SelectedFile);

  return Status;
}