# README

Firstly, the project module uses the STM32F103ZET6 Standard Peripheral Library template, which has saved me a lot of time as a beginner. Below is the link to the template from a skilled developer:
https://github.com/Peakors/STM32F103ZET6_StdPeriph_Lib_Template
The project is developed using both CLion and Keil.

STM32F103ZET6 Standard Peripheral Library version: V3.6.0 / 20-September-2021

printf has been redirected in both CLion and Keil.

For Keil, the redirection was based on the following documentation:
https://blog.csdn.net/qq_34810707/article/details/112255306

The most important redirection was found in the FLASH.ld file, referenced from a CSDN author. Huge thanks! Here's a link to their blog:
https://blog.csdn.net/weixin_43932857/article/details/130542569

### Below are some of my own modifications:

To make use of the stm32f1xx.cfg file, I modified OpenOCD's configuration. In the /usr/local/share/openocd/scripts/target directory, I copied the stm32f1xx.cfg file and renamed it to gd32f303rct6.cfg (leaving the original untouched). You can find this modified file in the project under OpenOCD/gd32f303rct6.cfg.
---

```bash
set _FLASHNAME $_CHIPNAME.flash flash bank $_FLASHNAME stm32f1x 0x08000000 $_FLASH_SIZE 0 0 $_TARGETNAME
```
Manually specify the Flash size as 256KB (0x00040000):
```bash
set _FLASHNAME $_CHIPNAME.flash flash bank $_FLASHNAME stm32f1x 0x08000000 0x00040000 0 0 $_TARGETNAME
```
Then, find the following code snippet:
```bash
#jtag scan chain if { [info exists CPUTAPID] } { set _CPUTAPID $CPUTAPID } else { if { [using_jtag] } {

See STM Document RM0008 Section 26.6.3

set _CPUTAPID 0x3ba00477
this is the SW-DP tap id not the jtag tap id

set _CPUTAPID 0x1ba01477
```
Change 0x1ba01477 to 0x2ba01477.

---
---

首先项目模块引用的是 STM32F103ZET6 标准库模板，为小白的我省了好多时间，下面是大佬的模板地址  
[https://github.com/Peakors/STM32F103ZET6_StdPeriph_Lib_Template](https://github.com/Peakors/STM32F103ZET6_StdPeriph_Lib_Template)  
使用 CLion 和 Keil 同时开发

STM32F103ZET6 标准库，标准库版本：**V3.6.0 / 20-September-2021**

Clion 和 Keil 中已重定向自己到 printf。

Keil 重定向参考文档：[https://blog.csdn.net/qq_34810707/article/details/112255306](https://blog.csdn.net/qq_34810707/article/details/112255306)

最重要的重定向找到了 FLASH.ld 引用自 CSDN 的一位作者，感谢！下面是他的博客  
[https://blog.csdn.net/weixin_43932857/article/details/130542569](https://blog.csdn.net/weixin_43932857/article/details/130542569)

### 以下我自己的修改的一些修改：

为了利用 stm32f1xx.cfg 的文件，标准板修改了 openocd 的配置文件，在 `/usr/local/share/openocd/scripts/target` 目录下，将 `stm32f1xx.cfg` 文件复制一份并改名为 `gd32f303rct6.cfg`（原目录未动），找到下配置。（修改好的文件我放在项目的 `OpenOCD/gd32f303rct6.cfg`）

---

```bash
set _FLASHNAME $_CHIPNAME.flash flash bank $_FLASHNAME stm32f1x 0x08000000 $_FLASH_SIZE 0 0 $_TARGETNAME
```
手动指定 Flash 大小为 256KB (0x00040000)
```bash
set _FLASHNAME $_CHIPNAME.flash flash bank $_FLASHNAME stm32f1x 0x08000000 0x00040000 0 0 $_TARGETNAME
```
并找到下面代码段
```bash
#jtag scan chain if { [info exists CPUTAPID] } { set _CPUTAPID $CPUTAPID } else { if { [using_jtag] } {

See STM Document RM0008 Section 26.6.3

set _CPUTAPID 0x3ba00477
this is the SW-DP tap id not the jtag tap id

set _CPUTAPID 0x1ba01477
```
并修改 0x1ba01477 为 0x2ba01477