#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "libusb.h"


const static int reqIntLen=8;
const static int reqBulkLen=64;
const static int endpoint_Int_in=0x81; /* endpoint 0x81 address for IN */
const static int endpoint_Int_out=0x01; /* endpoint 1 address for OUT */
const static int endpoint_Bulk_in=0x82; /* endpoint 0x81 address for IN */
const static int endpoint_Bulk_out=0x02; /* endpoint 1 address for OUT */
const static int timeout=5000; /* timeout in ms */

static struct libusb_device_handle *devh = NULL;


static void perr(char const *format, ...)
{
    va_list args;

    va_start (args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void test_bulk_transfer(libusb_device_handle *dev)
 
  {
 
    int r,i;
    char answer[reqBulkLen];
    char question[reqBulkLen];
    int size;
    for (i=0;i<reqBulkLen; i++) question[i]=i;
    r = libusb_bulk_transfer(dev, endpoint_Bulk_out, (unsigned char*)&question,reqBulkLen,&size, timeout);
    if( r != LIBUSB_SUCCESS )
    {
        perr("   send_mass_storage_command: %s\n", libusb_strerror((enum libusb_error)r));
    }
    r = libusb_interrupt_transfer(dev, endpoint_Bulk_in, (unsigned char*)&answer, reqBulkLen,&size, timeout);
    if( r != reqBulkLen )
    {
      perr("   USB bulk read: %s\n", libusb_strerror((enum libusb_error)r));
    }
    for (i=0;i<reqBulkLen;i++) printf("%i, %i, \n",question[i],answer[i]);
 //   usb_set_altinterface(dev, 0);
 
    libusb_release_interface(dev, 0);
 
  }


int main( int argc, char **argv)

{
    int rc = libusb_init(NULL);
    if (rc < 0) {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(rc));
        exit(1);
    }

    devh = libusb_open_device_with_vid_pid(NULL, 0x16c0, 0x0763);
    if (!devh) {
        fprintf(stderr, "Error finding USB device\n");
        goto out;
    }

    rc = libusb_claim_interface(devh, 2);
    if (rc < 0) {
        fprintf(stderr, "Error claiming interface: %s\n", libusb_error_name(rc));
        goto out;
    }





    /* Measurement has already been done by the signal handler. */

    libusb_release_interface(devh, 2);
out:
    if (devh)
        libusb_close(devh);
    libusb_exit(NULL);
    return rc;
 }