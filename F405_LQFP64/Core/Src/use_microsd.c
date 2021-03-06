/*
 * read_microsd_data.c
 *
 *  Created on: Nov 8, 2020
 *      Author: chris
 */

#include <use_microsd.h>
#include "fatfs.h"
#include <string.h>
#include "sdio.h"

// WARNING WILL DELETE DATA ON BOOT!
//#define FORMAT_MICROSD

// f_mkfs
//BYTE work[FF_MAX_SS];

// f_write
BYTE readBuf[30];

void read_sdio_data()
{

#ifdef FORMAT_MICROSD
  fres = f_mkfs("", 0, work, sizeof(work));
  if (fres != FR_OK) {
    Error_Handler();
  }
#endif
	 // try to mount Micro-SD card

	  FRESULT fres = f_mount(&SDFatFS, SDPath, 0);
	  if(fres != FR_OK)
	  {
		  // FR_DISK_ERR (and HAL_SD_ERROR_RX_OVERRUN)
		  // Fix by setting SDIOCLK clock divide factor to 3
		  error_sdio("mount");
	  }

	  DWORD free_clusters, free_sectors, total_sectors;

	  FATFS* getFreeFs;

	  fres = f_getfree("", &free_clusters, &getFreeFs);
	  if (fres != FR_OK) {
		  error_sdio("getfree");
	  }
	  else
	  {
		  total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
		  free_sectors = free_clusters * getFreeFs->csize;

		  printf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

	  }

	  char line[100];
	   /* Open a text file */
	  fres = f_open(&SDFile, "TEST.TXT", FA_READ);
	    if (fres) error_sdio("open file");

	    /* Read every line and display it */
	    while (f_gets(line, sizeof line, &SDFile)) {
	        printf("%s\r\n", line);
	    }

	  //write_sdio_data("test.txt", "Hello, from STM32F405");

	  //Close file, don't forget this!
	  f_close(&SDFile);

	  //De-mount drive
	  f_mount(NULL, "", 0);


}

// writes empty string for some reason :/
void write_sdio_data(char* filename, char* writebuf)
{
	  FRESULT fres = f_open(&SDFile, filename, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	  if(fres == FR_OK) {
	    printf("I was able to open file for writing\r\n");
	  } else {
		  error_sdio("open file");
	  }

	  strncpy((char*)readBuf, "some text", 19);
	  UINT bytesWrote;
	  fres = f_write(&SDFile, readBuf, 19, &bytesWrote);
	  if(fres == FR_OK) {
	    printf("Wrote %i bytes to %s!\r\n", bytesWrote, filename);
	  } else {
		  // FR_DISK_ERR (and HAL_SD_ERROR_TX_UNDERRUN)
		  // Fix by setting SDIOCLK clock divide factor to 4
		  error_sdio("write file");
	  }
}

void error_sdio(char* opcode)
{
	printf("Error when performing SDIO %s (%lu)\r\n", opcode, hsd.ErrorCode);
}
