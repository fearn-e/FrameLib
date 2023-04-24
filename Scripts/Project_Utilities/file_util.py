
from pathlib import Path
from . object_info import fl_object


def do_regex(data: str, exp: str):

    import re
    
    regex = re.compile(exp)
    match = regex.search(data)
        
    if match is not None:
        return match.group(1)

    return ""


def do_regex_with_hint(data: str, exp: str, hint: str):

    hi = data.find(hint)
    hi = data.find("\n", hi)
    lo = data.rfind("\n", 0, hi)
    
    return do_regex(data[lo:hi], exp)
    

def item_regex(path: str, exp: str):
    
    with open(path) as f:
        data = f.read()
        return do_regex(data, exp)

    return ""
 
    
def lines_regex(path: str, exp: str, start: str, end: str, inner_start: str, inner_end: str):
    
    import re
    
    regex = re.compile(exp)
    list = []
    started1 = False
    started2 = False
    started = False
    
    with open(path) as f:
        for line in f:
            match = regex.search(line)
            
            if start in line:
                started1 = True
            if inner_start in line:
                started2 = True
              
            if started and (end in line or inner_end in line):
                return list
                
            started = started1 and started2
            
            if started and match is not None:
                list.append([match.group(1), match.group(0)])

    return list
    

def newline_setting(output_path: str):
    
    # Use window line endings for vcxproj files
        
    if Path(output_path).suffix == ".vcxproj":
        return '\r\n'
        
    return None
    
    
def create(output_path: str, template_path: str, object_info: fl_object):

    contents = templated_string(template_path, object_info)
    
    f = open(output_path, "w", newline = newline_setting(output_path))
    f.write(contents)
    f.close()


def find_next_blankline(data: str, index: int):

    while True:
        next = data.find("\n", index)
        if next >= 0:
            next = data.find("\n", next + 1)
        if data[index:next].isspace():
            return index + 1
        if next < 0:
            return index
        index = next
            
            
def find_section(data: str, bounds: list):

    index_lo = 0
    index_hi = len(data)
    
    for idx in range(int(len(bounds) / 2)):
        index_lo = data.find(bounds[idx * 2 + 0], index_lo, index_hi)
        index_hi = data.find(bounds[idx * 2 + 1], index_lo + len(bounds[idx * 2 + 0]), index_hi)

    return index_lo, index_hi
    
    
def get_section(path: str, bounds: list):
        
    with open(path, "r") as f:
        data = f.read()
        index_lo, index_hi = find_section(data, bounds)
        return data[index_lo:index_hi]
                
    return ""
   

def section_regex(path: str, bounds: list, exp: str, hint: str = ""):
    
    data = get_section(path, bounds)
    
    if hint != "":
        return do_regex_with_hint(data, exp, hint)
    else:
        return do_regex(data, exp)
        
        
def insert(path: str, contents: str, bounds: list, next_blank: bool = False):
    
    data = ""
    
    with open(path, "r") as f:
        data = f.read()
        index_lo, index = find_section(data, bounds)
                
        # Look for next whitespace line (skipping the first which will be immediate)
        
        if next_blank:
            index = find_next_blankline(data, index)
            
    with open(path, "w", newline = newline_setting(path)) as f:
        f.write(data[:index] + contents + data[index:])


def remove(path: str, contents: str, bounds: list):
    
    data = ""
    
    with open(path, "r") as f:
        data = f.read()
        index_lo, index_hi = find_section(data, bounds)
        index = data.find(contents, index_lo, index_hi)
        
    if index < 0:
        print("WARNING - FAILED TO REMOVE\n")
        print(contents)
        return
        
    with open(path, "w", newline = newline_setting(path)) as f:
        f.write(data[:index] + data[index + len(contents):])


def modify(path:str, contents: str, bounds: list, add: bool):
    if add:
        insert(path, contents, bounds)
    else:
        remove(path, contents, bounds)
        

def replace_next_key(data: str, object_info: fl_object):

    lo, hi = find_section(data, ["_##", "##_"])
    
    if lo < 0 or hi < 0:
        return False, data

    key = data[lo+3:hi]
    
    if key.lower() in object_info.__dict__:
        value = object_info.__dict__[key.lower()]
    else:
        value = fl_object.__dict__[key.lower()]
            
    return True, data.replace("_##" + key + "##_", value)
    
    
def templated_string(template_path: str, object_info: fl_object):

    with open(template_path, "r") as f:
        template = f.read()

    condition = True;
    
    while condition:
        condition, template = replace_next_key(template, object_info)
        
    return template
