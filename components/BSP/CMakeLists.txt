set(src_dirs
            LED
            KEY
            ESPTIM
            GPTIM
            WDT
            PWM
            IIC)

set(include_dirs
            LED
            KEY
            ESPTIM
            GPTIM
            WDT
            PWM
            IIC)

set(requires
            driver
            esp_timer
            esp_lcd)

idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})

component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)