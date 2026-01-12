# config_reader.py
import configparser
import json
import sys

def read_ini(path: str) -> str:
    """读 ini → 返回 json 字符串"""
    cp = configparser.ConfigParser()
    cp.read(path, encoding='utf-8')
    data = {sec: dict(cp.items(sec)) for sec in cp.sections()}
    return json.dumps(data, ensure_ascii=False)

if __name__ == '__main__':
    # 命令行：python config_reader.py ../cfg/config.ini
    print(read_ini(sys.argv[1]))
