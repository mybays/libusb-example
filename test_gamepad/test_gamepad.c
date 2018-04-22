#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define VERSION "0.1.0"
#define VENDOR_ID 0x1cbe
#define PRODUCT_ID 0x000f

const static int PACKET_INT_LEN=4;
const static int INTERFACE=0;
const static int ENDPOINT_INT_IN=0x81; /* endpoint 0x81 address for IN */
const static int ENDPOINT_INT_OUT=0x81; /* endpoint 1 address for OUT */
const static int TIMEOUT=5000; /* timeout in ms */

void bad(const char *why) {
    fprintf(stderr,"Fatal error> %s\n",why);
    exit(17);
}

static struct libusb_device_handle *devh = NULL;

static int find_lvr_hidusb(void)
{
    devh = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
    return devh ? 0 : -1;
}

static int test_interrupt_transfer(void)
{
    int r,i;
    int transferred;
    char answer[PACKET_INT_LEN];
    char question[PACKET_INT_LEN];
    int j;
    for(j=0;j<100;j++)
    {
        r = libusb_interrupt_transfer(devh, ENDPOINT_INT_OUT, question, PACKET_INT_LEN,
                                      &transferred,TIMEOUT);
        if (r < 0) {
            fprintf(stderr, "Interrupt write error %d:%s\n", r,libusb_error_name(r));
            return r;
        }

        r = libusb_interrupt_transfer(devh, ENDPOINT_INT_IN, answer,PACKET_INT_LEN,
                                      &transferred, TIMEOUT);
        if (r < 0) {
            fprintf(stderr, "Interrupt read error %d:%s\n", r,libusb_error_name(r));
            return r;
        }
        if (transferred < PACKET_INT_LEN) {
            fprintf(stderr, "Interrupt transfer short read %d (%d):%s\n",transferred, r,libusb_error_name(r));
            return -1;
        }

        for(i = 0;i < PACKET_INT_LEN; i++) {
            if(i%8 == 0)
                printf("\n");
            printf("%02x ",answer[i]);
        }
        printf("\n");
    }
    return 0;
}

int main(void)
{
    int r = 1;

    r = libusb_init(NULL);
    if (r < 0) {
        fprintf(stderr, "Failed to initialise libusb\n");
        exit(1);
    }

    r = find_lvr_hidusb();
    if (r < 0) {
        fprintf(stderr, "Could not find/open LVR Generic HID device\n");
        goto out;
    }
    printf("Successfully find the LVR Generic HID device\n");

//#ifdef LINUX
    libusb_detach_kernel_driver(devh, 0);
//#endif

    r = libusb_set_configuration(devh, 1);
    if (r < 0) {
        fprintf(stderr, "libusb_set_configuration error %d\n", r);
        goto out;
    }
    printf("Successfully set usb configuration 1\n");
    r = libusb_claim_interface(devh, 0);
    if (r < 0) {
        fprintf(stderr, "libusb_claim_interface error %d:%s\n", r,libusb_error_name(r));
        //goto out;
    }
    printf("Successfully claimed interface\n");

    test_interrupt_transfer();
    libusb_release_interface(devh, 0);
    out:
    //  libusb_reset_device(devh);
    libusb_close(devh);
    libusb_exit(NULL);
    return r >= 0 ? r : -r;
}