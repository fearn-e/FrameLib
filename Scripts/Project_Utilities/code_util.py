from . import file_util
from . object_info import fl_object
from . path_util import fl_paths


def insert_code(contents: str, class_name: str, category: str, path: str, start: str, end: str, insert_end: str, exp: str, layout: str = ""):

    category_compare = category.replace("_", " ")
    categories = file_util.lines_regex(path, "^.*// (.*)", start, end, start, end)
    
    for index, c in enumerate(categories):
        if c[0] == category_compare:
            objects = file_util.lines_regex(path, exp, start, end, "// " + category_compare, "//")
            
            for o in objects:
                if o[0] > class_name:
                    file_util.insert(path, contents, [start, o[1]])
                    return
            
            file_util.insert(path, contents, [start, categories[index][1]], True)
            return
            
    contents = layout + "// " + category_compare + "\n\n" + contents + "\n"

    for c in categories:
        if c[0] > category_compare:
            file_util.insert(path, contents, [start, layout + "// " + c[0]])
            return
    
    file_util.insert(path, contents, [start, insert_end])
    
    
def insert_cpp_single_build(base_class: str, class_name: str, object_info: fl_object, path: str, start: str, end: str):
    contents = "    " + base_class + "<" + object_info["object_class"] + ">::makeClass(\"" + class_name + "\");\n"
    exp = "^.*?(fl.*~).*"
    insert_code(contents, class_name, object_info["category"], path, start, end, "    // Unary Operators", exp, "    ")
    
    
def insert_object_list_include(object_info: fl_object):

    path = fl_paths().objects_export_header()
    contents = file_util.templated_string(fl_paths().code_template("object_list_include"), object_info)
    exp = "^.*/FrameLib_Objects/" + object_info["category"] + "/(.*)\.h.*"
    insert_code(contents, object_info["object_class"], object_info["category"], path, "#ifndef", "#endif", "// Operators", exp)
    

