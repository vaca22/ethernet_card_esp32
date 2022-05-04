/* Play mp3 file by audio pipeline
   with possibility to start, stop, pause and resume playback
   as well as adjust volume

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_mem.h"
#include "audio_common.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "esp_peripherals.h"
#include "periph_touch.h"
#include "periph_adc_button.h"
#include "periph_button.h"
#include "board.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
static const char *TAG = "PLAY_FLASH_MP3_CONTROL";
#define MOUNT_POINT "/sdcard"
static struct marker {
    int pos;
    const uint8_t *start;
    const uint8_t *end;
} file_marker;


//i2s_config->bck_io_num = GPIO_NUM_33;
//i2s_config->ws_io_num = GPIO_NUM_32;
//i2s_config->data_out_num = GPIO_NUM_17 ;
//i2s_config->data_in_num = GPIO_NUM_35;



FILE *f;
int mp3_music_read_cb(audio_element_handle_t el, char *buf, int len, TickType_t wait_time, void *ctx)
{
    fread(buf,len,1,f);
    return len;
}

void app_main(void)
{



    esp_err_t ret;


    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = true,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024
    };
    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");



    ESP_LOGI(TAG, "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();


    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();


    slot_config.width = 0;

    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");


    sdmmc_card_print_info(stdout, card);


    const char *file = MOUNT_POINT"/fuck.txt";

    f = fopen(file, "wb");
    char buf[1024]={97};
    for(int k=0;k<1024;k++){
        buf[k]=97;
    }
    ESP_LOGI(TAG, "Card mountedxxxxxxxx");
    for(int k=0;k<100000;k++){
        fwrite(buf,1024,1,f);
    }
    fclose(f);
    ESP_LOGI(TAG, "Card unmountedxxxxxxxxxx");
    esp_vfs_fat_sdcard_unmount(mount_point, card);







}
