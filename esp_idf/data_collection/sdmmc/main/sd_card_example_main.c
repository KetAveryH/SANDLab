#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "sd_test_io.h"
#include "sd_pwr_ctrl_by_on_chip_ldo.h"

#define BUFFER_SIZE 1024  // Adjust buffer size to a reasonable limit

static const char *TAG = "example";

#define MOUNT_POINT "/sdcard"

#ifdef CONFIG_EXAMPLE_DEBUG_PIN_CONNECTIONS
const char* names[] = {"CLK", "CMD", "D0", "D1", "D2", "D3"};
const int pins[] = {CONFIG_EXAMPLE_PIN_CLK,
                    CONFIG_EXAMPLE_PIN_CMD,
                    CONFIG_EXAMPLE_PIN_D0
                    #ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
                    ,CONFIG_EXAMPLE_PIN_D1,
                    CONFIG_EXAMPLE_PIN_D2,
                    CONFIG_EXAMPLE_PIN_D3
                    #endif
                    };

const int pin_count = sizeof(pins)/sizeof(pins[0]);

#if CONFIG_EXAMPLE_ENABLE_ADC_FEATURE
const int adc_channels[] = {CONFIG_EXAMPLE_ADC_PIN_CLK,
                            CONFIG_EXAMPLE_ADC_PIN_CMD,
                            CONFIG_EXAMPLE_ADC_PIN_D0
                            #ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
                            ,CONFIG_EXAMPLE_ADC_PIN_D1,
                            CONFIG_EXAMPLE_ADC_PIN_D2,
                            CONFIG_EXAMPLE_ADC_PIN_D3
                            #endif
                            };
#endif //CONFIG_EXAMPLE_ENABLE_ADC_FEATURE

pin_configuration_t config = {
    .names = names,
    .pins = pins,
#if CONFIG_EXAMPLE_ENABLE_ADC_FEATURE
    .adc_channels = adc_channels,
#endif
};
#endif //CONFIG_EXAMPLE_DEBUG_PIN_CONNECTIONS

static esp_err_t s_example_read_file(const char *path)
{
    ESP_LOGI(TAG, "Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }

    char buffer[BUFFER_SIZE];
    // char someVariable;
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        // Process the buffer or print to console
        buffer[bytesRead] = '\0';  // Null-terminate the buffer for safe printing
        ESP_LOGI(TAG, "Read from file: '%s'", buffer);
        // printf("Read from file: '%s'\n", buffer);
        // for (size_t i = 0; i < sizeof(buffer); ++i) {
        //     someVariable[i] = buffer[i];
        // }
    }
    
    // while (fgets(buffer, sizeof(buffer), f) != NULL) {
    //     ESP_LOGI(TAG, "Read from file: '%s'", buffer);
    // }


    fclose(f);
    return ESP_OK;
}


// Function to read the file and store its contents in someVariable
// static esp_err_t s_example_read_file(const char *path, char **someVariable)
// {
//     ESP_LOGI(TAG, "Reading file %s", path);
//     FILE *f = fopen(path, "r");
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for reading");
//         return ESP_FAIL;
//     }

//     char buffer[BUFFER_SIZE];
//     size_t bytesRead;
//     size_t totalBytes = 0;
//     size_t capacity = BUFFER_SIZE;

//     // Allocate initial memory for someVariable
//     *someVariable = (char *)malloc(capacity);
//     if (*someVariable == NULL) {
//         ESP_LOGE(TAG, "Failed to allocate memory");
//         fclose(f);
//         return ESP_FAIL;
//     }

//     while ((bytesRead = fread(buffer, 1, sizeof(buffer), f)) > 0) {
//         // Null-terminate the buffer for safe printing
//         buffer[bytesRead] = '\0';
//         ESP_LOGI(TAG, "Read from file: '%s'", buffer);
//         // printf("Read from file: '%s'\n", buffer);

//         // Check if we need to reallocate more memory for someVariable
//         if (totalBytes + bytesRead >= capacity) {
//             capacity *= 2; // Double the capacity
//             *someVariable = (char *)realloc(*someVariable, capacity);
//             if (*someVariable == NULL) {
//                 ESP_LOGE(TAG, "Failed to reallocate memory");
//                 fclose(f);
//                 return ESP_FAIL;
//             }
//         }

//         // Copy the contents of buffer to someVariable
//         memcpy(*someVariable + totalBytes, buffer, bytesRead);
//         totalBytes += bytesRead;
//     }

//     // Null-terminate the final someVariable content
//     (*someVariable)[totalBytes] = '\0';

//     for (size_t i = 0; i < totalBytes; ++i) {
//         printf(someVariable[i]);
//     }

//     fclose(f);
//     return ESP_OK;
// }



void app_main(void)
{
    esp_err_t ret;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.

    ESP_LOGI(TAG, "Using SDMMC peripheral");

    // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
    // For setting a specific frequency, use host.max_freq_khz (range 400kHz - 40MHz for SDMMC)
    // Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    /**
     * On these chips, the SDMMC IO power is supplied externally
     */
#if CONFIG_EXAMPLE_SDMMC_IO_POWER_INTERNAL_LDO
    sd_pwr_ctrl_ldo_config_t ldo_config = {
        .ldo_chan_id = 4, // `LDO_VO4` is used as the SDMMC IO power
    };
    sd_pwr_ctrl_handle_t pwr_ctrl_handle = NULL;

    ret = sd_pwr_ctrl_new_on_chip_ldo(&ldo_config, &pwr_ctrl_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to new an on-chip ldo power control driver");
        return;
    }
    host.pwr_ctrl_handle = pwr_ctrl_handle;
#endif

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // Set bus width to use:
#ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
    slot_config.width = 4;
#else
    slot_config.width = 1;
#endif

    // On chips where the GPIOs used for SD card can be configured, set them in
    // the slot_config structure:
#ifdef CONFIG_SOC_SDMMC_USE_GPIO_MATRIX
    slot_config.clk = CONFIG_EXAMPLE_PIN_CLK;
    slot_config.cmd = CONFIG_EXAMPLE_PIN_CMD;
    slot_config.d0 = CONFIG_EXAMPLE_PIN_D0;
#ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
    slot_config.d1 = CONFIG_EXAMPLE_PIN_D1;
    slot_config.d2 = CONFIG_EXAMPLE_PIN_D2;
    slot_config.d3 = CONFIG_EXAMPLE_PIN_D3;
#endif  // CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
#endif  // CONFIG_SOC_SDMMC_USE_GPIO_MATRIX

    // Enable internal pullups on enabled pins. The internal pullups
    // are insufficient however, please make sure 10k external pullups are
    // connected on the bus. This is for debug / example purpose only.
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
#ifdef CONFIG_EXAMPLE_DEBUG_PIN_CONNECTIONS
            check_sd_card_pins(&config, pin_count);
#endif
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    // Path to the existing file
    const char *file_church = MOUNT_POINT"/church.txt";

    // Read the file
    // char *someVariable = NULL;
    ret = s_example_read_file(file_church);
    // free(someVariable);
    if (ret != ESP_OK) {
        return;
    }

    // All done, unmount partition and disable SDMMC peripheral
    esp_vfs_fat_sdcard_unmount(mount_point, card);

#if SOC_SDMMC_IO_POWER_EXTERNAL
    ret = sd_pwr_ctrl_del_on_chip_ldo(pwr_ctrl_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to delete on-chip ldo power control driver");
        return;
    }
#endif

    ESP_LOGI(TAG, "Card unmounted");
}
