# HonestDAP说明

## 一.概述

### 1.介绍

> [!TIP]
>
> 这是个CMake的RT-Thread项目，使用Clion打开

**项目简介:** 这个项目是基于STM32F407实现的，支持DAPLink功能和虚拟串口功能。USB技术部分使用的是CherryUSB，DAPLink方面使用的是Arm开源的方案。其本质可以视为我尝试对立创DAPLink技术的拆解，但由于时间问题未能完成完成，暂时先这样。

- **DAPLINK:** 其与电脑的通信使用的是WinUSB,通过WinUSB的端点接收和发送DAP命令，至于DAP命令与待烧录单片机的通信，这里调用的是DAPLINK源码中的`DAP_ExecuteCommand`函数发送的

- **虚拟串口:** 其与电脑的通信使用的是USB的CDC，DAP方面使用Uart与MCU通信，将接收到的数据通过CDC发送给电脑，电脑也同样可以通过CDC给DAP发送数据，再通过DAP的Uart发送给MCU

**技术点:**

- RT-Thread
- CherryUSB
- DAPLink

**端口说明:**

- SWCLK: PE7

- SWDIO:  PE8
- TX: PD8
- RX: PD9

### 2.TODO

**简述:** 起初这个东西只是为了练手，没想到不知不觉研究了快一周，主要是USB那块太难的，而且为了更方便拆机立创的项目，浅浅的把RTThread学了下。总之，花太多时间了，DAPLink的功能目前已实现并测试通过，先放下吧，下面是我未来可能会更新的Flag

- [ ] 增加GUI界面

- [ ] 添加简易电流电压表功能

  这个逻辑已经实现，不过鉴于GUI尚未制作，并未包含在项目内

- [ ] ....

## 二.原理概述

### 1.DAPLINK

#### (1)概述

**概述:** DAPLINK实现的整体难点主要在于USB与PC的通信，其工作函数`chry_dap_handle`会在一个独立的线程中循环调用，在循环中**更新缓冲区**，然后对应的回调函数会因此被触发，处理USB与电脑的通信，进而实现整个DAPLink的逻辑

- 缓冲区更新:
  - 通过循环调用`DAP_ExecuteCommand`函数，实时更新USB_Response 和 USB_Request两个缓冲区
- 回调函数:
  - 分为`dap_in_callback`和`dap_out_callback`两个函数，分别处理USB端点的输入和输出事件回调
  - 当缓冲区发生变化时，它们会因为条件满足，开启与主机间的通信

#### (2)逻辑实现

##### ① chry_dap_handle

**简述:** `chry_dap_handle` 函数的作用是处理 DAP 请求。

**逻辑处理:** 实际的逻辑处理在其两个端点的回调函数中进行

- 只要`USB_RequestCountI`不等于`USB_RequestCountO`，该函数就会进入处理待处理请求的循环
- 将变量`n`设置为`USB_RequestIndexO`
- 它通过检查`USB_Request[n][0]`是否等于`ID_DAP_QueueCommands`来处理排队的命令。如果是，则将其更改为 `ID_DAP_ExecuteCommands`并增加`n`
- 然后调用`DAP_ExecuteCommand`发出通过DAP_config.h内定义的引脚发出DAP命令
- ...(后面的不重要)

```c
void chry_dap_handle(void)
{
    uint32_t n;

    // Process pending requests 处理待处理的请求
    while (USB_RequestCountI != USB_RequestCountO) {
        // Handle Queue Commands 处理队列命令
        n = USB_RequestIndexO;
        while (USB_Request[n][0] == ID_DAP_QueueCommands) {//检查索引n处的USB_Request数组的第一个字节是否等于命令标识符
            //将索引 n 处的 USB_Request 数组的第一个字节设置为命令标识符 ID_DAP_ExecuteCommands。
						//此操作将 USB 请求的命令类型更改为 ExecuteCommands，这是 CMSIS-DAP 协议的一部分，指示现在应执行排队的命令
					  USB_Request[n][0] = ID_DAP_ExecuteCommands;
            n++;
            if (n == DAP_PACKET_COUNT) {
                n = 0U;
            }
            if (n == USB_RequestIndexI) {
                // flags = osThreadFlagsWait(0x81U, osFlagsWaitAny, osWaitForever);
                // if (flags & 0x80U) {
                //     break;
                // }
            }
        }

        // Execute DAP Command (process request and prepare response)
				//将DAP_ExecuteCommand函数的结果分配给索引USB_RespnseIndexI处的USB_RespSize数组
        USB_RespSize[USB_ResponseIndexI] =
            (uint16_t)DAP_ExecuteCommand(USB_Request[USB_RequestIndexO], USB_Response[USB_ResponseIndexI]);

        // Update Request Index and Count
        USB_RequestIndexO++;
        if (USB_RequestIndexO == DAP_PACKET_COUNT) {
            USB_RequestIndexO = 0U;
        }
        USB_RequestCountO++;

        if (USB_RequestIdle) {//不处于请求忙状态
            if ((uint16_t)(USB_RequestCountI - USB_RequestCountO) != DAP_PACKET_COUNT) {
                USB_RequestIdle = 0U;//设置忙状态
                usbd_ep_start_read(0, DAP_OUT_EP, USB_Request[USB_RequestIndexI], DAP_PACKET_SIZE);
            }
        }

        // Update Response Index and Count
				// 更新响应索引和计数
        USB_ResponseIndexI++;
        if (USB_ResponseIndexI == DAP_PACKET_COUNT) {//如果 传入USB请求的数量 等于 DAP数据包COUNT
            USB_ResponseIndexI = 0U;//清除请求数
        }
        USB_ResponseCountI++;

        if (USB_ResponseIdle) {//不处于响应忙状态时
            if (USB_ResponseCountI != USB_ResponseCountO) {
                // Load data from response buffer to be sent back
                n = USB_ResponseIndexO++;
                if (USB_ResponseIndexO == DAP_PACKET_COUNT) {
                    USB_ResponseIndexO = 0U;
                }
                USB_ResponseCountO++;
                USB_ResponseIdle = 0U;
                usbd_ep_start_write(0, DAP_IN_EP, USB_Response[n], USB_RespSize[n]);
            }
        }
    }
}
```

##### ② 端点设置

**DAP_OUT端点:** 用于处理 USB DAP OUT 端点的数据接收事件。其功能如下(从主机到设备的数据传输)

1. 检查接收到的请求是否为 `ID_DAP_TransferAbort`，如果是，则设置 `DAP_TransferAbort` 标志。
2. 如果不是 `ID_DAP_TransferAbort` 请求，则增加 `USB_RequestIndexI` 索引，并更新 `USB_RequestCountI` 计数
3. 检查接收缓冲区是否有空间接收更多的数据包，如果有，则启动下一次数据接收操作
4. 如果缓冲区已满，则设置 `USB_RequestIdle` 标志

```c
void dap_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    (void)busid;
    if (USB_Request[USB_RequestIndexI][0] == ID_DAP_TransferAbort) {//USB的请求缓冲区
        DAP_TransferAbort = 1U;
    } else {
        //接收索引++
        USB_RequestIndexI++;
        if (USB_RequestIndexI == DAP_PACKET_COUNT) {//索引的大小等于缓冲区的大小(防止溢出的)
            //将接收索引清零
            USB_RequestIndexI = 0U;
        }
        //更新接收到的数据量
        USB_RequestCountI++;
    }

    // 开始接收下一个请求数据包
    if ((uint16_t)(USB_RequestCountI - USB_RequestCountO) != DAP_PACKET_COUNT) {//请求IN和请求OUT的差不等于缓冲区大小
        //端点接收，接收DAP_OUT_EP端点的数据
        usbd_ep_start_read(0, DAP_OUT_EP, USB_Request[USB_RequestIndexI], DAP_PACKET_SIZE);
    } else {
        //将USB设置为忙状态
        USB_RequestIdle = 1U;
    }
}
```

**DAP_IN端点:** 用于处理 USB DAP IN 端点的数据发送事件(从设备到主机的数据传输)

1. 检查是否有待发送的响应数据包
2. 如果有，将数据从响应缓冲区加载到 USB 端点进行发送
3. 更新响应缓冲区索引和计数
4. 如果没有待发送的数据包，则设置 `USB_ResponseIdle` 标志

```c
void dap_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    (void)busid;
    //已写入响应缓冲区的响应包数量不等于已从响应缓冲区中读取并发送出去的响应包数量
    //即还有待发送的响应数据包时执行if代码块
    if (USB_ResponseCountI != USB_ResponseCountO) {
        //从响应缓冲区加载要发送回的数据
        //向DAP_IN_EP端点写出
        usbd_ep_start_write(0, DAP_IN_EP, USB_Response[USB_ResponseIndexO], USB_RespSize[USB_ResponseIndexO]);
        //响应OUT索引++
        USB_ResponseIndexO++;
        //如果响应OUT索引 == 缓冲区，则将其清零
        if (USB_ResponseIndexO == DAP_PACKET_COUNT) {
            USB_ResponseIndexO = 0U;
        }
        //响应O++
        USB_ResponseCountO++;
    } else {
        //设置为忙状态
        USB_ResponseIdle = 1U;
    }
}
```

### 2.USB2Uart实现

#### (1)概述

**整体逻辑:** chry_dap_usb2uart_handle在项目的while循环中循环调用，在其内**usbrx to uart tx**读取从USB中获取的数据，调用`chry_dap_usb2uart_uart_send_bydma`使用uart的DMA发送给PC机，然后**uartrx to usb tx**的逻辑大致是读取Uart的数据，然后使用端点写出(官方在chry_dap_usb2uart_handle和usbd_cdc_acm_bulk_in均设有写逻辑，我搞不太懂)

#### (2)逻辑实现

##### ① 回调设置

###### Ⅰ.chry_dap_usb2uart_handle

**逻辑:** 这个函数会在while循环中循环调用

- 通过检查config_uart(默认非0)判断是否已经配置
  - 没有配置
    - 设置`config_uart = 0`标记已经配置
    - 调用弱定义函数`chry_dap_usb2uart_uart_config_callback`(用户外部实现)初始化Uart
    - 设置空闲标记位和`config_uart_transfer`(Uart传输已配置)为`1`
  - 已配置
    - 进一步检查Uart传输是否已经配置，已配置则直接返回
    - 后继只要USB传输通道都是空闲，则执行**uartrx to usb tx**和**usbrx to uart tx**逻辑
      - 具体见下面的剖析

```c
void chry_dap_usb2uart_handle(void)
{
    uint32_t size;
    uint8_t *buffer;
	
    if (config_uart) {
        /* disable irq here */
        config_uart = 0;
        /* config uart here */
        chry_dap_usb2uart_uart_config_callback((struct cdc_line_coding *)&g_cdc_lincoding);
        usbtx_idle_flag = 1;
        uarttx_idle_flag = 1;
        config_uart_transfer = 1;
        //chry_ringbuffer_reset_read(&g_uartrx);
        /* enable irq here */
    }

    if (config_uart_transfer == 0) {
        return;
    }

    /* why we use chry_ringbuffer_linear_read_setup?
     * becase we use dma and we do not want to use temp buffer to memcpy from ringbuffer
     * 
    */

    /* uartrx to usb tx */
    if (usbtx_idle_flag) {
        if (chry_ringbuffer_get_used(&g_uartrx)) {
            usbtx_idle_flag = 0;
            /* start first transfer */
            buffer = chry_ringbuffer_linear_read_setup(&g_uartrx, &size);
			/* for byte alignment */
			memcpy(_usbtx_buffer, buffer, size);
			 
            usbd_ep_start_write(CDC_IN_EP, _usbtx_buffer, size);
        }
    }

    /* usbrx to uart tx */
    if (uarttx_idle_flag) {
        if (chry_ringbuffer_get_used(&g_usbrx)) {
            uarttx_idle_flag = 0;
            /* start first transfer */
            buffer = chry_ringbuffer_linear_read_setup(&g_usbrx, &size);
            chry_dap_usb2uart_uart_send_bydma(buffer, size);
        }
    }

    /* check whether usb rx ringbuffer have space to store */
    if (usbrx_idle_flag) {
        if (chry_ringbuffer_get_free(&g_usbrx) >= DAP_PACKET_SIZE) {
            usbrx_idle_flag = 0;
            usbd_ep_start_read(CDC_OUT_EP, usb_tmpbuffer, DAP_PACKET_SIZE);
        }
    }
}
```

###### Ⅱ.uartrx to usb tx

> [!WARNING]
>
> 注意不要把`CDC_IN_EP`写成`CDC_INT_EP`这两长得好像很容易搞错，如果发现IN回调不会触发去检查下

**逻辑:**

- 检查`usbtx_idle_flag`是否为1（表明USB TX处于空闲状态）
- 如果`g_uartrx`缓冲区中有数据（即`chry_ringbuffer_get_used(&g_uartrx)`返回非零），则执行以下逻辑：
  - 将`usbtx_idle_flag`设置为0，表示USB TX现在处于忙碌状态
  - 使用`chry_ringbuffer_linear_read_setup`函数获取从 g_uartrx 缓冲区读取数据的起始地址和最大可读取长度
  - 调用 usbd_ep_start_write 函数，将缓冲区中的数据通过USB发送出去

```c
if (usbtx_idle_flag) {
    if (chry_ringbuffer_get_used(&g_uartrx)) {// 获取缓冲区使用大小，只有他不为空时执行下面的逻辑
        usbtx_idle_flag = 0;//设置USB TX的忙状态
        /* start first transfer */
        //启动DMA，获取读取起始内存地址和最大线性可读取长度
        buffer = chry_ringbuffer_linear_read_setup(&g_uartrx, &size);
        //写端点
        usbd_ep_start_write(0, CDC_IN_EP, buffer, size);
    }
}
```

###### Ⅲ.usbrx to uart tx

> 将接收到数据通过Uart3发送出去

**关键:** chry_dap_usb2uart_uart_send_bydma函数，将数据交给这个弱定义的函数，这个函数最后在外部实现，然后调用Uart的DMA发送函数将数据发送给PC

**逻辑:**

- 检查`usbtx_idle_flag`是否为1，表示USB TX空闲
- 检查`g_uartrx` 缓冲区中是否有数据
  - 如果有数据，则将 `usbtx_idle_flag` 设置为 0（表示 USB TX 繁忙）
  - 然后启动 DMA 传输
  - 将数据从 `g_uartrx` 缓冲区通过 USB 发送出去

```c
if (uarttx_idle_flag) {
    if (chry_ringbuffer_get_used(&g_usbrx)) {// 获取缓冲区使用大小，只有他不为空时执行下面的逻辑
        uarttx_idle_flag = 0;//设置为忙状态
        /* start first transfer */
        buffer = chry_ringbuffer_linear_read_setup(&g_usbrx, &size);
        chry_dap_usb2uart_uart_send_bydma(buffer, size);//TODO 只调没实现
    }
}
```

**chry_dap_usb2uart_uart_send_bydma函数:** 这里只是给个示例

```c
// CDC UART通过DMA发送数据函数
void chry_dap_usb2uart_uart_send_bydma(uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit_DMA(&huart3,data,len);
    // 记录要发送的数据长度
    g_uart_tx_transfer_length = len;
}
```

##### ② 端点回调

**DAP_CDC_IN端点:** 处理CDC ACM接口批量IN传输的完成,最终数据的处理在USB2Uart回调函数中通过USB发送出去

- 标记从 UART 接收环形缓冲区读取数据的完成
- 如果读取的字节数（`nbytes`）是 DAP 数据包大小的倍数且非零，它会发送一个零长度数据包 (ZLP) 以确保正确终止传输
- 如果 UART 接收环形缓冲区中有更多数据，它会设置另一次传输
- 如果没有更多可用数据，它会设置 `usbtx_idle_flag` 以指示 USB 传输处于空闲状态并准备好接收新数据

```c
void usbd_cdc_acm_bulk_in(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    (void)busid;
    uint32_t size;
    uint8_t *buffer;

	//用于DMA完成，增加读指针
    chry_ringbuffer_linear_read_done(&g_uartrx, nbytes);
		
    if ((nbytes % DAP_PACKET_SIZE) == 0 && nbytes) {//检查发送长度是否是最大包长的整数
    	/* send zlp */
		//是，发送 zlp 包表示结束
        usbd_ep_start_write(0, CDC_IN_EP, NULL, 0);
    } else {
        if (chry_ringbuffer_get_used(&g_uartrx)) {//获取ringbuffer使用大小
			//用于启动DMA，获取读取起始内存地址和最大线性可读取长度
            buffer = chry_ringbuffer_linear_read_setup(&g_uartrx, &size);
            usbd_ep_start_write(0, CDC_IN_EP, buffer, size);
        } else {
            usbtx_idle_flag = 1;
        }
    }
}
```

**DAP_CDC_OUT端点:** 处理 USB CDC ACM 接口的 OUT 传输完成事件

**作用:** 读取端点数据写入到g_usbrx

1. 将 `usb_tmpbuffer` 中的数据写入到环形缓冲区 `g_usbrx` 内。
2. 检查环形缓冲区 `g_usbrx` 中的可用空间是否大于等于 `DAP_PACKET_SIZE`：

- 如果是，则继续从 `CDC_OUT_EP` 端点读取数据到 `usb_tmpbuffer`。
- 否则，设置 `usbrx_idle_flag` 为 1，表示当前传输处于忙状态，无法接受新的数据。

```c
void usbd_cdc_acm_bulk_out(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    (void)busid;
	//将usb_tmpbuffer中的数据写入到环形缓冲区g_usbrx内
    chry_ringbuffer_write(&g_usbrx, usb_tmpbuffer, nbytes);
    if (chry_ringbuffer_get_free(&g_usbrx) >= DAP_PACKET_SIZE) {//如果g_usbrx的大小大于等于缓冲区的大小
		//继续读CDC_OUT_EP端点
        usbd_ep_start_read(0, CDC_OUT_EP, usb_tmpbuffer, DAP_PACKET_SIZE);
    } else {
		//否则设置忙状态
        usbrx_idle_flag = 1;
    }
}
```

## 三.注意事项

- 如无法使用CLion打开，可以直接从网盘或从Releases下载项目打开

  [百度网盘](https://pan.baidu.com/s/1lDzyoXoS1KrU3V6syQQMeQ?pwd=j9j9) 

## 四.联系信息

邮箱：[mail@honestliu.com](mailto:mail@honestliu.com)