import os

def addResFile():
    # 直接使用已知的项目路径
    base_path = r"D:\code\Chat\Client"
    image_dir = os.path.join(base_path, "image")

    if not os.path.exists(image_dir):
        print(f"Directory {image_dir} does not exist")
        print("Available directories in base path:")
        for item in os.listdir(base_path):
            item_path = os.path.join(base_path, item)
            if os.path.isdir(item_path):
                print(f"  - {item}/")
        return

    # 获取所有png文件
    png_files = []
    for filename in os.listdir(image_dir):
        if filename.lower().endswith('.png'):
            png_files.append(filename)

    # 构建qrc文件内容
    qrc_content = '''<RCC>
    <qresource prefix="/">
        <file>style/LA.qss</file>\n'''

    # 添加image目录下的所有png文件
    for png_file in sorted(png_files):
        qrc_content += f'        <file>image/{png_file}</file>\n'

    qrc_content += '    </qresource>\n</RCC>'

    # 写入qrc文件到项目根目录
    qrc_file = os.path.join(base_path, "res.qrc")
    with open(qrc_file, 'w', encoding='utf-8') as f:
        f.write(qrc_content)

    print(f"Created {qrc_file} with {len(png_files)} PNG files:")
    for file in png_files:
        print(f"  - image/{file}")

if __name__ == "__main__":
    addResFile()
