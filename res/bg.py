import json
def find_name(j,name,id="name"):
    for v in j:
        if v[id]==name: return v
    return None
def load_layer(file,name):
    with open(file,"r") as fp:
        j = json.load(fp)
    layer=find_name(j["layers"],name)
    return layer
def print_point(name,layer):
    print(f'u8 const {name}[] = {"{"}')
    for v in layer["objects"]:
        print(f"    0x{v['x']:02x}, 0x{v['y']:02x},")
    print('};')

print('#include "bios.h"')
print_point("enemyPointTable",load_layer("bg.tmj","enemyPointTable"))
