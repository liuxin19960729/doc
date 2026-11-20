# uv
[uv](https://docs.astral.sh/uv/)
```
uv:Python 包管理工具


Linux 安装
curl -LsSf https://astral.sh/uv/install.sh | sh
```
## uv 简单使用
```
// 查看Python 可使用的版本
uv python list



// 安装指定版本的python
uv python install <version>



为当前项目固定 Python 版本（会创建 .python-version 文件）：
uv python pin 3.12


创建虚拟环境：
uv venv
uv venv --python 3.12



激活虚拟环境
# macOS / Linux
source .venv/bin/activate

# Windows（PowerShell）
.venv\Scripts\activate


// 退出虚拟环境
deactivate




包管理（pip 兼容模式）

# 安装最新版本
uv pip install requests

# 安装特定版本
uv pip install requests==2.31.0

# 从 requirements.txt 批量安装
uv pip install -r requirements.txt


升级包：
uv pip install --upgrade requests

卸载包：
uv pip uninstall requests

查看已安装的包：
uv pip list



导出当前环境的依赖到 requirements.txt：
uv pip freeze > requirements.txt







```
## 项目管理（推荐方式）
```

uv init my_project



项目的添加和移除依赖

在项目模式下，推荐使用 uv add 和 uv remove 管理依赖，它们会自动更新 pyproject.toml 和 uv.lock：
uv add <library>

uv add "requests>=2.31.0"

# 添加开发依赖（只在开发环境使用，如测试框架）
uv add --dev pytest ruff


# 移除依赖
uv remove requests



安装项目全部依赖（uv sync）
    克隆项目或更新 pyproject.toml 后，运行以下命令一键安装所有依赖：
    uv sync
  

    note:
        在pyproject.toml添加加中国镜像源
        [tool.uv]
        index-url = "https://pypi.tuna.tsinghua.edu.cn/simple"



生成更新锁文件
uv lock
   该命令会解析pyproject.tom中的依赖并生成或更新uv.lock文件
    note:
        uv.lock 应该提交到版本控制中确保团队所有成员使用完全相同的版本版本



```

## 脚本运行
```
uv run my_script.py


```