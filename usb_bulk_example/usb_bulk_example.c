/*
* Test suite program based of libusb-0.1-compat testlibusb
* Copyright (c) 2013 Nathan Hjelm <hjelmn@mac.ccom>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libusb.h"


#define USB_DEVICE_VID	(0x1cbe)
#define USB_DEVICE_PID	(0x0003)

#define TEST_OUT_ENDPOINT	(0x01)
#define TEST_IN_ENDPOINT		(0x81)

int verbose = 0;



static struct libusb_device_handle *devh = NULL;

static int find_test_device(void)
{
	devh = libusb_open_device_with_vid_pid(NULL, USB_DEVICE_VID, USB_DEVICE_PID);
	return devh ? 0 : -1;
}

unsigned char send[256]="abcdefghijklmnopl";
unsigned char response[256];

int main(int argc, char *argv[])
{
	libusb_device **devs;
	ssize_t cnt;
	int r, i;

	if (argc > 1 && !strcmp(argv[1], "-v"))
		verbose = 1;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	if(0!=find_test_device())
	{
		printf("open device failed.\r\n");
		return -1;
	}

	r = libusb_claim_interface(devh, 0);
	if (r < 0) {
		fprintf(stderr, "usb_claim_interface error %d %s\n", r, strerror(-r));
		goto out;
	}
	int actsize;
	while(1)
	{
		
		//发送
		r=libusb_bulk_transfer(devh,TEST_OUT_ENDPOINT,send,64,&actsize,1000);
		if(0 != r)
		{
			fprintf(stderr, "recv error %d %s %s\n", r, libusb_error_name(r),libusb_strerror(r));
			goto out;
		}
		
		//接收
		r=libusb_bulk_transfer(devh,TEST_IN_ENDPOINT,response,64,&actsize,1000);
		if(0 != r)
		{
			fprintf(stderr, "recv error %d %s %s\n", r, libusb_error_name(r),libusb_strerror(r));
			goto out;
		}
		int i;
		for(i=0;i<64;i++)
		{
			printf("%c ",response[i]);
		}
		printf("\r\n");
		usleep(1*1000*1000);
	}

out:
	libusb_close(devh);

	libusb_exit(NULL);
	return 0;
}
