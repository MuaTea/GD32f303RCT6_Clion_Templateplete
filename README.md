# README

首先项目模块引用的是 STM32F103ZET6 标准库模板，为小白的我省了好多时间，下面是大佬的模板地址  
[https://github.com/Peakors/STM32F103ZET6_StdPeriph_Lib_Template](https://github.com/Peakors/STM32F103ZET6_StdPeriph_Lib_Template)  
使用 CLion 和 Keil 同时开发

STM32F103ZET6 标准库，标准库版本：**V3.6.0 / 20-September-2021**

Clion 和 Keil 中已重定向自己到 printf。

Keil 重定向参考文档：[https://blog.csdn.net/qq_34810707/article/details/112255306](https://blog.csdn.net/qq_34810707/article/details/112255306)

最重要的重定向找到了 FLASH.ld 引用自 CSDN 的一位作者，感谢！下面是他的博客  
[https://blog.csdn.net/weixin_43932857/article/details/130542569](https://blog.csdn.net/weixin_43932857/article/details/130542569)

### 这里我自己的修改的一些修改：

为了利用 stm32f1xx.cfg 的文件，标准板修改了 openocd 的配置文件，在 `/usr/local/share/openocd/scripts/target` 目录下，将 `stm32f1xx.cfg` 文件复制一份并改名为 `gd32f303rct6.cfg`（原目录未动），找到下配置。（修改好的文件我放在项目的 `OpenOCD/gd32f303rct6.cfg`）

---

```bash
set _FLASHNAME $_CHIPNAME.flash flash bank $_FLASHNAME stm32f1x 0x08000000 $_FLASH_SIZE 0 0 $_TARGETNAME
手动指定 Flash 大小为 256KB (0x00040000)

set _FLASHNAME $_CHIPNAME.flash flash bank $_FLASHNAME stm32f1x 0x08000000 0x00040000 0 0 $_TARGETNAME

并找到下面代码段 
#jtag scan chain if { [info exists CPUTAPID] } { set _CPUTAPID $CPUTAPID } else { if { [using_jtag] } {

See STM Document RM0008 Section 26.6.3

set _CPUTAPID 0x3ba00477
this is the SW-DP tap id not the jtag tap id

set _CPUTAPID 0x1ba01477

并修改 0x1ba01477 为 0x2ba01477