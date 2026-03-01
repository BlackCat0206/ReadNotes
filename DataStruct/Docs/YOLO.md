# YOLO

----Chime Lee

## 一、概述

**YOLO（You Only Look Once）检测原理**：**单阶段检测算法**，核心是【端到端的回归思想】，区别于R-CNN系列的”先提候选框再分类“，优势是快、适合实时场景。

**核心步骤：特征提取->特征融合->预测输出**

* **特征提取（Backbone）**：用CSPDarknet53对图像层层下采样（卷积 + 池化），提取多尺度特征（80 x 80、40 x 40、20 x 20）;
* **特征融合（Neck）**：用FPN（自顶向下）+ PAN（自底向上）融合多尺度特征——解决”**小目标（远处人像）检测差“的问题**，用SPPF模块替代池化，减少计算量同时保留关键特征。即：你检测的人像有不同大小（比如近大远小），特征融合能让模型同时识别不同尺度的人像
* **预测输出（Head）**：不再分”先提候选框再分类“，直接**回归三类信息（一次性输出）**：；
  * 边界框坐标；
  * 目标置信度；
  * 类别是吧率；

**核心优势**：

* 速度快：单阶段检测，无需生成候选框；
* 端到端：训练和推理一体化；
* 通用性强：对于检测类，换场景只需微调数数据集；

## 二、环境准备（纯本地，无 ROS/leaper 依赖）

### 步骤 1：创建独立训练目录（所有文件都放这里，不混其他路径）

```bash
# 创建主目录，后续所有操作都在这个目录下
mkdir -p ~/yolov5_train && cd ~/yolov5_train
```

### 步骤 2：安装 YOLOv5 核心依赖（仅装必要的，无冗余）

```bash
# 安装基础依赖（适配 Ubuntu 22.04 + Python3.10）
pip3 install torch==1.13.1 torchvision==0.14.1 ultralytics==8.0.20 opencv-python==4.8.1.78 numpy==1.26.4
```

### 步骤 3：下载 YOLOv5 源码（纯本地版，无需 leaper）

```bash
# 下载 YOLOv5 v7.0 源码（最稳定的版本）
wget https://github.com/ultralytics/yolov5/archive/refs/tags/v7.0.zip && unzip v7.0.zip && mv yolov5-7.0 yolov5
# 进入 YOLOv5 目录
cd yolov5
# 安装 YOLOv5 自带依赖
pip3 install -r requirements.txt
```

## 三、准备本地数据集

### 步骤 1：创建 YOLO 标准数据集目录结构

```bash
# 在 yolov5_train 目录下创建数据集目录（回到主目录）
cd ~/yolov5_train
mkdir -p person_dataset/{images/train,images/val,labels/train,labels/val}
```

**目录结构（简单清晰，无冗余）：**

```plaintext
yolov5_train/
├── yolov5/                # YOLOv5 源码
└── person_dataset/        # 纯本地人像数据集
    ├── images/
    │   ├── train/         # 训练集图片（.jpg）
    │   └── val/           # 验证集图片（.jpg）
    └── labels/
        ├── train/         # 训练集标签（.txt，YOLO格式）
        └── val/           # 验证集标签（.txt，YOLO格式）
```

### 步骤 2：下载预处理好的人像数据（无需标注，直接用）

```bash
# 下载 COCO128 数据集（仅筛选 person 类）
wget https://github.com/ultralytics/yolov5/releases/download/v1.0/coco128.zip && unzip coco128.zip

# 筛选仅 person 类（COCO 中 person 的 cls_id=0），放入我们的数据集目录
python3 -c "
import os, shutil
# 源路径（下载的 coco128）
src_img = './coco128/images/train2017'
src_label = './coco128/labels/train2017'
# 目标路径（纯本地数据集）
dst_img_train = './person_dataset/images/train'
dst_label_train = './person_dataset/labels/train'
dst_img_val = './person_dataset/images/val'
dst_label_val = './person_dataset/labels/val'

# 先创建目标目录（避免路径不存在报错）
os.makedirs(dst_img_train, exist_ok=True)
os.makedirs(dst_label_train, exist_ok=True)
os.makedirs(dst_img_val, exist_ok=True)
os.makedirs(dst_label_val, exist_ok=True)

# 遍历所有标签文件，仅保留 person 类
all_txt = [f for f in os.listdir(src_label) if f.endswith('.txt')]
train_size = int(len(all_txt)*0.8)  # 8:2 划分训练/验证集
for i, txt_file in enumerate(all_txt):
    # 修正核心：先读取文件内容到 lines，再筛选 person 类
    with open(os.path.join(src_label, txt_file), 'r') as f:
        lines = f.readlines()  # 先读取所有行，定义 lines 变量
        person_lines = [line for line in lines if line.startswith('0 ')]  # 筛选 person 类（cls_id=0）
    if not person_lines:
        continue
    # 复制图片和标签到对应目录
    img_file = txt_file.replace('.txt', '.jpg')
    img_src_path = os.path.join(src_img, img_file)
    # 额外增加图片存在性检查（避免 FileNotFoundError）
    if not os.path.exists(img_src_path):
        print(f'警告：图片 {img_src_path} 不存在，跳过')
        continue
    if i < train_size:
        # 训练集
        shutil.copy(img_src_path, dst_img_train)
        with open(os.path.join(dst_label_train, txt_file), 'w') as f:
            f.writelines(person_lines)
    else:
        # 验证集
        shutil.copy(img_src_path, dst_img_val)
        with open(os.path.join(dst_label_val, txt_file), 'w') as f:
            f.writelines(person_lines)

print(f'数据集准备完成！')
print(f'训练集图片数：{len(os.listdir(dst_img_train))}')
print(f'验证集图片数：{len(os.listdir(dst_img_val))}')
"
```

### 步骤 3：创建数据集配置文件（data.yaml）

在 `~/yolov5_train` 目录下创建 `data.yaml`（纯本地路径，无 leaper/ROS）：

```yaml
# data.yaml 内容（仅 person 类）
train: ../person_dataset/images/train  # 训练集图片路径
val: ../person_dataset/images/val      # 验证集图片路径
nc: 1                                  # 类别数：仅人像
names: ["person"]                      # 类别名称
```

## 三、本地训练YOLOv5s

### 步骤 1：进入 YOLOv5 源码目录

```bash
cd ~/yolov5_train/yolov5
```

### 步骤 2：执行训练命令（聚焦核心参数，简化配置）

```bash
# 训练 YOLOv5s 模型，适配 640×480 分辨率
python3 train.py \
  --img 640 480 \          # 分辨率：宽640，高480（贴合之前的要求）
  --batch 8 \              # 批次大小（根据电脑显存调整，显存小改4）
  --epochs 30 \            # 训练轮数（30轮足够达标）
  --data ../data.yaml \    # 数据集配置文件（纯本地路径）
  --weights yolov5s.pt \   # 预训练权重（自动下载）
  --cfg models/yolov5s.yaml \  # YOLOv5s 网络配置
  --name person_detector \ # 训练结果命名
  --save-period 5          # 每5轮保存一次权重
```

### 关键说明：

- 命令中所有路径都是**相对本地目录**，无任何 leaper/ROS 相关路径；
- 运行后会自动下载 `yolov5s.pt` 预训练权重，无需手动下载；
- 训练过程中会实时显示 loss，目标：box loss < 0.1、cls loss < 0.15 即可。

### 步骤 3：查看训练结果

训练完成后，结果会保存在 `~/yolov5_train/yolov5/runs/train/person_detector/`：

- `weights/best.pt`：最优权重（训练效果最好的模型）；
- `results.png`：训练曲线（loss、mAP 等指标）；
- `val_batch0_pred.jpg`：验证集预测可视化（可直观看到人像检测效果）。

## 四、纯本地推理测试（验证训练效果）

**用训练好的模型检测本地图片，验证是否能识别人像**：

```bash
# 下载一张测试图片
wget https://raw.githubusercontent.com/ultralytics/yolov5/master/data/images/zidane.jpg -O ~/yolov5_train/test.jpg

# 用训练好的模型推理
python3 detect.py \
  --weights runs/train/person_detector/weights/best.pt \
  --source ~/yolov5_train/test.jpg \
  --img 640 480 \
  --conf 0.8  # 置信度阈值（≥0.8，贴合要求，若出现no person问题，调低置信度）
```

### 查看推理结果：

结果保存在 `~/yolov5_train/yolov5/runs/detect/exp/`，打开 `zidane.jpg` 可看到：

- 人像被框出，标注 “person”；
- 置信度 ≥0.8，符合要求。

## 五、YOLO总结（核心关键点）

1. **路径完全独立**：所有操作都在 `~/yolov5_train` 目录下，无 leaper/.ros/ROS 相关路径，避免混乱；
2. **流程极简**：环境→数据集→训练→推理，四步完成纯本地 YOLOv5 训练，聚焦核心逻辑；
3. **参数适配**：分辨率 640×480、置信度 ≥0.8，和之前的要求一致，后续可无缝迁移到 leaper 工程。

按这个流程操作，你能先掌握 YOLOv5 训练的核心逻辑，等熟悉后，再把训练好的 `best.pt` 权重迁移到 leaper 工程即可（仅需复制权重文件，无需改训练逻辑）。

## 六、ONNX 转换

RK3588的NPU不直接支持`PyTorch`的`.pt`权重，需要先转换为专用格式：



## X.后记

### Q：**为什么选择`YOLOv5s`?**

A：我们当时评估过YOLOv8，精度确实略好一点，但考虑到嵌入式部署的稳定性，**最终选了YOLOv5s**。主要是它结构更规整，在RKNN转换时**算子全兼容**，后处理也更简单，能直接复用瑞芯微官方示例里的C++解码代码。项目要落地，**硬件跑得稳比精度高一点更重要**。

### Q：**现在RKNN工具链已经完美支持YOLOv8了，你怎么看？**

A：是的，新版本RKNN Toolkit 2确实支持YOLOv8了，但如果项目启动时团队对RKNN经验不足，或者客户指定的固件版本锁在了旧内核，选YOLOv5依然是**风险最低**的方案。如果是新项目且工具链可自由升级，我也会考虑YOLOv8。

### Q：如何平衡检测速度和精度？

**精度优化**：

* **数据集**：
  * **扩充数据**：微调人像数据集，**补充多样化样本（不同姿态/光照/背景的人像），减少过拟合**；
  * **标注规范**：**标注框完全包裹人像，避免漏标/错标**。标签格式严格归一化。
* **模型训练层面**：
  * **预训练权重**：用**大数据预训练**（COCO80类），再用**小数据集**（人像）微调；
  * **超参数调优**：调小学习率（lr0=0.001）、增加训练轮数（epochs=100）、调整 boxloss 权重（box=0.05→0.1），让模型**更聚焦人像框回归**；

**速度优化**：

* **模型轻量化**：

  * **选小模型**：优先选 YOLOv5s/YOLOv8n 等轻量化模型，满足实时性（比如机器人帧率要求≥10FPS）；

  * **模型剪枝/量化**：对训练训练好的模型做良好（FP32->FP16/INT8）量化；

* **输入层**：
  * **固定分辨率**：**统一训练和推理的分辨率**（都用480 x 480）,避免推理时的尺寸变化耗时；
* **部署层面**：
  * **硬件优化**：使用RK3588，内置6TOPS算力的NPU，可以实现硬件优化。

