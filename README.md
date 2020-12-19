
## Notes ##


1. 

Used `gcc-arm-none-eabi-9-2019-q4-major`  toolchain by copying to

 `'STM32CubeIDE/st/stm32cubeide_1.2.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-arm-embedded.7-2018-q2-
 update.linux64_1.4.0.202007081208/tools'` directory.




2. 

Added 
`// Fixes bug - https://github.com/STMicroelectronics/STM32CubeF4/issues/8`
`HAL_I2SEx_TxRxCpltCallback(hi2s);`

to stm32f405xx_hal_i2s_ex.c:939  


3. CMSIS DSP dependency

Clone the https://github.com/ARM-software/CMSIS repo to the same directory as this repo.

i. Add "../../../Lib/CMSIS_4/CMSIS/Lib/GCC" to C/C++ General > Paths and Symbols > Library Paths
ii. Add "../../../Lib/CMSIS_4/CMSIS/Include" to C/C++ General > Paths and Symbols > Includes
