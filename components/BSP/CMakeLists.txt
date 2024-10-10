set(src_dirs
            LED
            KEY
            ESPTIM
            GPTIM
            WDT
            PWM
            IIC
            WIFI)

set(include_dirs
            LED
            KEY
            ESPTIM
            GPTIM
            WDT
            PWM
            IIC
            WIFI)

set(requires
            driver
            esp_timer
            esp_lcd
            nvs_flash
            esp_wifi)

idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})

component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)