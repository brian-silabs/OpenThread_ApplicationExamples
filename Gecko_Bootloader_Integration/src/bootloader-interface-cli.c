
#include "bootloader-interface-cli.h"
#include "application_properties.h"
#include "btl_interface.h"

#include"openthread/cli.h"

static void bootloader(void * aContext, uint8_t aArgsLength, char *  aArgs[]);

static otCliCommand gUserCommands[]={
    {.mName = "bootloader", .mCommand = bootloader},
};


void bootloaderInterfaceCliInit(void)
{
    otCliSetUserCommands(gUserCommands, (sizeof(gUserCommands) / sizeof(otCliCommand)), NULL);
}


static void bootloader(void * aContext, uint8_t aArgsLength, char *  aArgs[])
{

  char ch;              // UART input character
  int32_t btl_status;  // store the status of various bootloader API call's result
  uint8_t image[16];   // read the image from the slot to this buffer

  (void) aContext;  //Unused Param

  if((aArgsLength - 1) != 1)
  {
    ch = (aArgs[0])[0];//first char of first arg
    if((ch & 0xff) == 'u') {
      if(bootloader_verifyImage(0, NULL) == BOOTLOADER_OK) {
          otCliOutputFormat("BTL: updating image...\n");
        bootloader_rebootAndInstall();
      } else {
          otCliOutputFormat("BTL: no valid image, skipping update\n");
      }
    }
    else if((ch & 0xff) == 'v') {
      if(bootloader_verifyImage(0, NULL) == BOOTLOADER_OK) {
          otCliOutputFormat("BTL: image valid\n");
      } else {
          otCliOutputFormat("BTL: image invalid\n");
      }
    }
    else if((ch & 0xff) == 'e') {
        otCliOutputFormat("BTL: erasing image...\n");
      if((btl_status = bootloader_eraseStorageSlot(0)) != BOOTLOADER_OK) {
          otCliOutputFormat("BTL: erase error: 0x%08lx\n", btl_status);
      } else {
          otCliOutputFormat("BTL: erase OK\n");
      }
    }
    else if((ch & 0xff) == 'r') {
      if((btl_status = bootloader_readStorage(0, 0, image, 16)) == BOOTLOADER_OK) {
          otCliOutputFormat("BTL: raw data (slot0 / address 0x0 [ ");
        for(int cnt = 0; cnt < 16; cnt++) {
            otCliOutputFormat("0x%02x ");
        }
        otCliOutputFormat("]\n");
      } else {
          otCliOutputFormat("BTL: cannot read image\n");
      }
    }
    else {
      otCliOutputFormat("bootloader: Invalid Argument\r\n");
    }
  } else {
    otCliOutputFormat("bootloader: Invalid Argument Count\r\n");
  }
}
