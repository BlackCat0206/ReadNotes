# LLM

*---- Chime Lee*

## 一、概述

**本地环境**：

* **CPU**：AMD 9700X
* **Ramdom**：32GB
* **GPU**：Nvidia GeForce RTX 5070Ti 16GB
* **System**：Windows 11
* **IDE**：VSCode

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

### 2.3. 验证是否用了显卡

```plaintext
nvidia-smi
```

能看到 `ollama` 占用显存 = **成功用 GPU 加速**。

### 2.4. Ollama 命令集

```bash
C:\Users\Admin>ollama help
Large language model runner

Usage:
  ollama [flags]
  ollama [command]

Available Commands:
  serve       Start Ollama
  create      Create a model
  show        Show information for a model
  run         Run a model
  stop        Stop a running model
  pull        Pull a model from a registry
  push        Push a model to a registry
  signin      Sign in to ollama.com
  signout     Sign out from ollama.com
  list        List models
  ps          List running models
  cp          Copy a model
  rm          Remove a model
  launch      Launch the Ollama menu or an integration
  help        Help about any command

Flags:
  -h, --help         help for ollama
      --nowordwrap   Don't wrap words to the next line automatically
      --verbose      Show timings for response
  -v, --version      Show version information

Use "ollama [command] --help" for more information about a command.
```

## 三、Continue

### 3.1 遍历本地模型

* 显示本地下载模型

```bash
ollama list
```

* 示例：

```bash
C:\Users\Admin>ollama list
NAME                     ID              SIZE      MODIFIED
deepseek-coder-v2:16b    63fb193b3a9b    8.9 GB    17 seconds ago
qwen2.5-coder:14b        9ec8897f747e    9.0 GB    18 minutes ago
deepseek-r1:14b          c333b7232bdb    9.0 GB    14 hours ago
```

### 3.2 Continue本地配置

```yaml
name: Local Config
version: 1.0.0
schema: v1
models:
  - name: deepseek r1             #对话模型
    provider: ollama
    model: deepseek-r1:14b
    roles:
      - chat
    contextWindow: 8192
    maxTokens: 2048
    apiBase: http://localhost:11434
  - name: deepseek-coder-v2       # 补全模型
    provider: ollama
    model: deepseek-coder-v2:16b  # 优先用量化版，速度更快
    roles:
      - autocomplete
      - chat  # 新增chat角色，支持主动触发注释生成代码
    contextWindow: 128000  # 发挥超长上下文优势
    maxTokens: 20  # 适配你“单行补全”的需求
    # 核心新增：自定义补全提示词，强制单行补全
    promptTemplate: |
      你是一个专业的代码补全助手，只输出一行代码，不输出任何解释、注释、换行符。
      根据上下文的代码和注释，补全当前行的剩余内容，确保语法正确、贴合需求。
      上下文：{{context}}
      需要补全的行：{{prefix}}
      补全结果：
    # 极致低随机性，避免模型“摆烂不补全”
    temperature: 0.0  # 从0.1降到0，完全确定性输出
    topP: 0.1
    stopSequences: ["\n"]  # 强制换行就停
    apiBase: http://localhost:11434
# 核心新增：强制指定补全模型+扩大上下文范围
defaultModel: deepseek r1
tabAutocompleteModel: deepseek-coder-v2  # 确保Tab补全用deepseek-coder-v2
completionOptions:
  contextLines: 50  # 保留完整上下文，保证补全准确性
  triggerOnNewLine: true
  triggerOnTab: true
  debounceTime: 50  # 进一步缩短补全延迟（从100→50ms）
  # 新增：强制补全为单行、无换行
  stopSequences: ["\n"]  # 遇到换行符就停止生成，确保只输出一行
  temperature: 0.1  # 降低随机性，单行补全更精准
  minimumPrefixLength: 1  # 只要输入1个字符就触发补全
  disableAutoCompletions: false
  topP: 0.9
```



