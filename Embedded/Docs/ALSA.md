# ALSA

---- Chime Lee

## 一、概述

**SAI (Synchronous Audio Interface)**：同步音频接口；

**信号**：

* SAIx_MCLK(Audio Master Clock)：主时钟；
* SAIx_RX_BCLK(Receive Bit Clokc)：接收位时钟，串行数据时钟；
* SAIx_TX_BCLK(Transmit Bit Clokc)：发送位时钟，串行数据时钟；
* SAI1_RX_SYNC(Receive Frame Sync)：接收同步线，区分左右声道；
* SAI1_TX_SYNC(Transmit Frame Sync)：发送同步线，区分左右声道；
* SAI1_RX_DATA（Receive Data.）：接收数据线
* SAI1_TX_DATA（Transmit Data.）：发送数据线

配置芯片：WM8960，地址：0x1a；

用I2C配置

**设备树配置**：

```dts
/ {
    // 1. ALSA SND_SOC声卡节点：绑定CPU-DAI(I2S0)和CODEC-DAI(WM8960)
    sound {
        compatible = "rockchip,rk3588-audio-wm8960", "simple-audio-card";
        simple-audio-card,name = "UAV-WM8960";  // 声卡名称，自定义
        simple-audio-card,format = "i2s";        // 音频接口：I2S
        simple-audio-card,bitclock-master = <&sound_codec>;
        simple-audio-card,frame-master = <&sound_codec>;
        // 音频控件与链路路由：绑定WM8960输出到扬声器
        simple-audio-card,widgets =
            "Speaker", "Speaker Out";
        simple-audio-card,routing =
            "Speaker Out", "HP_OUTL",
            "Speaker Out", "HP_OUTR";
        // CPU-DAI：RK3588 I2S0控制器
        sound_cpu: simple-audio-card,cpu {
            sound-dai = <&i2s0>;
            dma-buf-size = <0x10000>; // DMA缓冲区，避免音频卡顿
        };
        // CODEC-DAI：WM8960编解码芯片
        sound_codec: simple-audio-card,codec {
            sound-dai = <&wm8960>;
            clocks = <&cru SCLK_I2S0>; // 绑定I2S0时钟，保证时序同步
        };
    };

    // 2. WM8960节点：I2C地址+GPIO控制+设备树匹配
    wm8960: wm8960@1a { // WM8960默认I2C从地址：0x1a，不可修改
        compatible = "wlf,wm8960"; // 与内核WM8960驱动的compatible一致
        reg = <0x1a>;
        pinctrl-names = "default";
        pinctrl-0 = <&pinctrl_wm8960>; // 绑定复位/休眠GPIO
        reset-gpios = <&gpio4 RK_PB1 GPIO_ACTIVE_LOW>; // 复位GPIO，低电平有效
        shutdown-gpios = <&gpio4 RK_PB2 GPIO_ACTIVE_LOW>; // 休眠GPIO
        wlf,shared-lrclk; // 共享LRCLK，适配RK3588 I2S时序
        status = "okay";
    };
};

// 3. 启用RK3588原生I2S0控制器，配置语音标准参数
&i2s0 {
    status = "okay";
    rockchip,i2s-rx-sample-rate = <16000>; // 16kHz，语音播放标准
    rockchip,i2s-tx-sample-rate = <16000>;
    rockchip,i2s-bit-width = <16>;         // 16bit位深
    rockchip,i2s-channel = <2>;            // 立体声，适配WM8960
    pinctrl-names = "default";
    pinctrl-0 = <&i2s0m1_pins>; // 绑定I2S0引脚
};

// 4. 启用RK3588原生I2C2控制器，配置400KHz高速模式
&i2c2 {
    status = "okay";
    clock-frequency = <400000>; // I2C高速模式，快速配置WM8960寄存器
    pinctrl-names = "default";
    pinctrl-0 = <&i2c2m1_pins>; // 绑定I2C2引脚
};	

// 5. 配置所有相关引脚的电气特性（抗干扰，贴合无人机场景）
&pinctrl {
    // WM8960复位/休眠GPIO配置
    pinctrl_wm8960: wm8960grp {
        rockchip,pins = <
            RK3588_GPIO4_PB1 GPIO_OUT &pcfg_pull_up // 上拉+输出，稳定电平
            RK3588_GPIO4_PB2 GPIO_OUT &pcfg_pull_up
        >;
    };
    // I2C2引脚配置（复用为I2C，上拉+高速率）
    i2c2m1_pins: i2c2m1grp {
        rockchip,pins = <
            RK3588_GPIO1_A6 I2C2_SDA &pcfg_pull_up &pcfg_high_speed
            RK3588_GPIO1_A7 I2C2_SCL &pcfg_pull_up &pcfg_high_speed
        >;
    };
    // I2S0引脚配置（复用为I2S，上拉+高速率）
    i2s0m1_pins: i2s0m1grp {
        rockchip,pins = <
            RK3588_GPIO2_A0 I2S0_BCLK &pcfg_pull_up &pcfg_high_speed
            RK3588_GPIO2_A1 I2S0_LRCLK &pcfg_pull_up &pcfg_high_speed
            RK3588_GPIO2_A2 I2S0_DO &pcfg_pull_up &pcfg_high_speed
        >;
    };
    // 通用电气特性配置（复用，无需重复定义）
    pcfg_pull_up: pcfg-pull-up {
        bias-pull-up;
        drive-strength = <8>;
    };
    pcfg_high_speed: pcfg-high-speed {
        slew-rate = <1>;
    };
};
```

