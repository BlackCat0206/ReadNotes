# LLM

*---- Chime Lee*

## 一、概述

**本地环境**：

* CPU：AMD 9700X
* Ramdom：32GB
* GPU：Nvidia GFORCE RTX 5070Ti 16GB
* System：Windowns 11
* IDE：VSCode
* 

## 二、本地部署

### 2.1 下载安装 Ollama（Windows 版）

官网：https://ollama.com/download/windows

一路下一步，**不需要任何配置**。

### 2.2. 打开 CMD / PowerShell，直接跑模型

5070 Ti 16G 显存，推荐这两个：

#### ① 最轻量、秒开

```bash
ollama run deepseek-r1:1.5b
```

显存占用：**≈7GB**

#### ② 效果更好

```plaintext
ollama run deepseek-r1:7b-q4_0
```

显存占用：**≈10GB**

### 3. 验证是否用了显卡

```plaintext
nvidia-smi
```

能看到 `ollama` 占用显存 = **成功用 GPU 加速**。

## 三、Continue

### 3.1 遍历本地模型

* 显示本地下载模型

```bash
ollama list
```

* 示例：

```bash
C:\Users\Admin>ollama list
NAME               ID              SIZE      MODIFIED
deepseek-r1:14b    c333b7232bdb    9.0 GB    5 minutes ago
```

### 3.2 Continue本地配置

```yam
name: Local Config
version: 1.0.0
schema: v1
models:
  - name: deepseek r1
    provider: ollama
    model: deepseek-r1:14b		# 此处名称需要和ollama list显示内容相同！！！
    roles:
      - chat
      - autocomplete
    contextWindow: 8192  # 新增：适配 DeepSeek 上下文，提升代码补全体验
    maxTokens: 2048      # 新增：限制单次生成长度，避免显存占用过高

```



