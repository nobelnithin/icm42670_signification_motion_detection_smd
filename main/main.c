#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <icm42670.h>

static const char *TAG = "icm42670";

#define PORT 0
#if defined(CONFIG_EXAMPLE_I2C_ADDRESS_GND)
#define I2C_ADDR ICM42670_I2C_ADDR_GND
#endif
#if defined(CONFIG_EXAMPLE_I2C_ADDRESS_VCC)
#define I2C_ADDR ICM42670_I2C_ADDR_VCC
#endif

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

/* Find gpio definitions in sdkconfig */



void icm42670_pedo_test(void *pvParameters)
{
    // const gpio_config_t io_conf = {
    //     .intr_type = GPIO_INTR_DISABLE,
    //     .mode = GPIO_MODE_INPUT,
    //     .pin_bit_mask = BIT(CONFIG_EXAMPLE_INT_INPUT_PIN),
    //     .pull_down_en = 0,
    //     .pull_up_en = 0,
    // };
    // gpio_config(&io_conf);

    icm42670_t dev = { };
    ESP_ERROR_CHECK(icm42670_init_desc(&dev, I2C_ADDR, PORT, CONFIG_EXAMPLE_I2C_MASTER_SDA, CONFIG_EXAMPLE_I2C_MASTER_SCL));
    ESP_ERROR_CHECK(icm42670_init(&dev));

    // const uint8_t int_pin = 2;
    // const icm42670_int_config_t int_config = {
    //     .mode = ICM42670_INT_MODE_LATCHED,
    //     .drive = ICM42670_INT_DRIVE_PUSH_PULL,
    //     .polarity = ICM42670_INT_POLARITY_ACTIVE_HIGH,
    // };
    // ESP_ERROR_CHECK(icm42670_config_int_pin(&dev, int_pin, int_config));

    // icm42670_int_source_t sources = {false};
    // sources.smd = true;
    // // sources.wom_z = true;
    // // sources.wom_y = true;
    // // sources.wom_z = true;
    // ESP_ERROR_CHECK(icm42670_set_int_sources(&dev, int_pin, sources));

    ESP_ERROR_CHECK(icm42670_set_accel_odr(&dev, ICM42670_ACCEL_ODR_50HZ));
    ESP_ERROR_CHECK(accel_config1(&dev, true));
    ESP_ERROR_CHECK(icm42670_set_accel_pwr_mode(&dev, ICM42670_ACCEL_ENABLE_LP_MODE));
    //ESP_ERROR_CHECK(icm42670_set_accel_pwr_mode(&dev, ICM42670_LP_CLK_WUO));
    //  ESP_ERROR_CHECK(icm42670_power_up(&dev, true));
    ESP_ERROR_CHECK(icm42670_dmp_odr(&dev, true));
    // ESP_ERROR_CHECK(icm42670_apex_config0_1(&dev, true)); 
    vTaskDelay(pdMS_TO_TICKS(1));
    ESP_ERROR_CHECK(icm42670_apex_config2(&dev, true));
    ESP_ERROR_CHECK(icm42670_apex_config3(&dev, true));
    ESP_ERROR_CHECK(icm42670_apex_config4(&dev, true));
    ESP_ERROR_CHECK(icm42670_apex_config9(&dev, true));
    ESP_ERROR_CHECK(icm42670_apex_config0(&dev, true));
    vTaskDelay(pdMS_TO_TICKS(1));
    ESP_ERROR_CHECK(icm42670_apex_config0_1(&dev, true));
    vTaskDelay(pdMS_TO_TICKS(50));
    ESP_ERROR_CHECK(icm42670_intr_source6(&dev, true));
    ESP_ERROR_CHECK(icm42670_intr_source7(&dev, true));
    ESP_ERROR_CHECK(icm42670_ped_enable(&dev, true));
    ESP_ERROR_CHECK(icm42670_smd_sen_sel(&dev, true));   
    ESP_ERROR_CHECK(icm42670_apex_config0_2(&dev, true));   
    vTaskDelay(pdMS_TO_TICKS(1));
    ESP_ERROR_CHECK(icm42670_apex_config0_1(&dev, true));
    vTaskDelay(pdMS_TO_TICKS(50));
    ESP_ERROR_CHECK(icm42670_intr_source1(&dev, true));
    ESP_ERROR_CHECK(icm42670_intr_source2(&dev, true));
    ESP_ERROR_CHECK(icm42670_ped_enable(&dev, true));
    ESP_ERROR_CHECK(icm42670_smd_enable(&dev, true));
    uint8_t reg;
    printf("---------------------------------\n");
    while(1)
    {
       
        reg=icm42670_activity_class(&dev, 0);
        if(reg==8)
        {
           printf("Significant motion detected\n");   
        }
    
        // printf("apex_config 0:%u\n",data); 
    
        // printf("Steps:%u\n",reg);
   
    // ESP_ERROR_CHECK(icm42670_apex_config9(&dev, true));
        // ESP_ERROR_CHECK(read_all_m_reg(&dev, true));
    //     ESP_ERROR_CHECK(read_all_reg(&dev, APEX_CONFIG1));
    //     ESP_ERROR_CHECK(read_all_reg(&dev, APEX_CONFIG2));
    //     ESP_ERROR_CHECK(read_all_reg(&dev, APEX_CONFIG3));
    //    ESP_ERROR_CHECK(read_all_reg(&dev, APEX_CONFIG4));
    //    ESP_ERROR_CHECK(read_all_reg(&dev, APEX_CONFIG9));
    //    ESP_ERROR_CHECK(read_all_reg(&dev, ACCEL_CONFIG0));
    //    ESP_ERROR_CHECK(read_all_reg(&dev, PWR_MGMT0));
    //     ESP_ERROR_CHECK(read_all_reg(&dev, intr_source6));
    //    ESP_ERROR_CHECK(read_all_reg(&dev, intr_source7));
    //    printf("--------------------------------------------------------\n");
        // ESP_LOGI(TAG, "Pedometer detected %s", gpio_get_level(CONFIG_EXAMPLE_INT_INPUT_PIN) ? "true" : "false");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}

void app_main()
{
    ESP_ERROR_CHECK(i2cdev_init());                                                                                     
    
    // xTaskCreatePinnedToCore(icm42670_wom_test, "icm42670_wom_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreatePinnedToCore(icm42670_pedo_test, "icm42670_pedometer_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}

