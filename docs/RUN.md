# ?? 运行说明 (RUN)

## ? 启动程序

```bat
code\scripts\run.bat
```

或：

```bat
cd msvc_release
objectRecoStudy.exe
```

程序从 **可执行文件所在目录** 向上查找仓库根（识别 `code/CMakeLists.txt` 与 `models/`），因此移动整个仓库后路径仍然有效。

```mermaid
flowchart BT
    EXE[msvc_release/objectRecoStudy.exe]
    EXE -->|向上查找| ROOT[仓库根目录]
    ROOT --> MODELS[models/]
    ROOT --> CODE[code/CMakeLists.txt]
    ROOT --> OUTPUT[output/]
    EXE --> INPUT[msvc_release/input/]
```

## ? 默认输入数据

| 类型 | 相对路径 | 说明 |
|------|----------|------|
| ?? 图像 | `msvc_release/input/people.jpeg` | 构建脚本可从 `D:\k8\media_images\xi_an_hot\people.jpeg` 复制 |
| ? 视频 | `msvc_release/input/demo.mp4` | 构建脚本可从默认 mp4 复制 |

未选择文件时，各算例按钮使用上述默认路径。

## ? 输出目录

运行结果写入仓库根目录 `output/`：

| 文件 | 说明 |
|------|------|
| `inputImage.xml` | Mask R-CNN 输入缓存 |
| `foreground.jpg` | GrabCut 前景 |
| `contour_*.jpg` | 多边形逼近可视化 |

## ?? 界面布局

```mermaid
flowchart TB
    subgraph UI[objectRecoStudy 主界面]
        direction LR
        subgraph LEFT[左侧 · 检测与分割]
            B1[Mask R-CNN 图像]
            B2[Mask R-CNN 摄像头]
            B3[Mask R-CNN 视频]
            B4[MobileNet 视频]
            B5[框优化]
            B6[GrabCut]
        end
        subgraph MID[中间 · 预览]
            PIC[?? 图像预览区]
        end
        subgraph RIGHT[右侧 · 日志]
            LOG[? 运行日志]
        end
    end
    subgraph BOTTOM[下方]
        CAM[? 摄像头编号]
        MODELS[? 框架算例入口]
    end
```

## ? 操作速查

| 区域 | 功能 |
|------|------|
| 左侧「检测与分割」 | Mask R-CNN 图像/摄像头/视频、MobileNet、框优化、GrabCut |
| 中间「预览」 | 算例结果图像 |
| 右侧「运行日志」 | 路径、耗时、状态 |
| 下方「基本设置」 | 摄像头编号（默认 0） |
| 系统菜单「关于」 | 版本号、版权、联系方式 |

? 鼠标悬停按钮可查看 **工具提示**；按钮旁有简短中文说明。

## ?? GrabCut 交互

```mermaid
stateDiagram-v2
    [*] --> 选择图像
    选择图像 --> 框选ROI: 拖拽矩形
    框选ROI --> 执行分割: 按 p
    执行分割 --> 框选ROI: 继续调参
    框选ROI --> [*]: 按 q 退出
```

1. 点击「GrabCut 学习」并选择图像  
2. 在 OpenCV 窗口 `grabcut_source` 中拖拽框选 ROI  
3. 按 **p** 执行分割，按 **q** 退出  

## ? 一键运行

「一键运行全部算例」依次执行 Mask R-CNN 图像检测与图像预览（视频/摄像头算例需手动触发以避免长时间阻塞）。

```mermaid
flowchart LR
    A[一键运行] --> B[Mask R-CNN 图像]
    B --> C[预览图像]
    C --> D[? 完成]
```

## ? 依赖运行时

- OpenCV DLL（与编译所用 OpenCV 版本一致，需在 PATH 或复制到 `msvc_release/`）
- MFC 运行库（通常随 Visual C++ Redistributable 提供）

## ? 模型查找顺序

```mermaid
flowchart TD
    START[加载模型] --> A{msvc_release/input/models/}
    A -->|存在| OK[? 使用]
    A -->|不存在| B{仓库 models/}
    B -->|存在| OK
    B -->|不存在| ERR[? 请运行 download_models.bat]
```

详见 [models.md](models.md)。
