
from Project_Utilities import file_util
from Project_Utilities import code_util
from Project_Utilities.object_info import fl_object
from Project_Utilities.vs_util import fl_solution
from Project_Utilities.xcode_util import fl_pbxproj
from Project_Utilities.path_util import fl_paths

import os
from pathlib import Path
    

def update_all(add: bool):

    projects = Path(fl_paths().vs_max_projects()).glob('fl.*.vcxproj')
    project_list = list(projects)
    project_list.sort()
        
    for project in project_list:
        name = project.as_posix().rsplit("/", 1)[1].replace(".vcxproj", "")
        
        if add:
            print("add " + name)
        else:
            print("remove " + name)
        
        object_info = fl_object.create_from_name(name)

        if add:
            fl_solution().update_project(object_info, True)
        
        fl_solution().update(object_info, add)
        fl_pbxproj().update(object_info, add)


def rebuild():

    import time
    
    t1 = time.perf_counter_ns()
    update_all(False)
    t2 = time.perf_counter_ns()
    update_all(True)
    
    t3 = time.perf_counter_ns()
    print("Completed removal in " + str((t2 - t1)/1000000000.) + " seconds")
    print("Completed additions in " + str((t3 - t2)/1000000000.) + " seconds")
    print("Completed rebuild in " + str((t3 - t1)/1000000000.) + " seconds")


def new_object(object_info : fl_object):
    
    paths = fl_paths()

    os.makedirs(paths.object_dir(object_info), exist_ok = True)
    os.makedirs(paths.max_object_dir(object_info), exist_ok = True)
    
    file_util.create(paths.max_source(object_info), paths.template("fl.class_name~.cpp"), object_info)
    file_util.create(paths.object_header(object_info), paths.template("FrameLib_Class.h"), object_info)
    file_util.create(paths.object_source(object_info), paths.template("FrameLib_Class.cpp"), object_info)
    fl_solution().update_project(object_info)
    
    code_util.insert_cpp_single_build("FrameLib_MaxClass_Expand", object_info.max_class_name, object_info, paths.max_framelib(), "main(", "}")
    code_util.insert_cpp_single_build("FrameLib_PDClass_Expand", object_info.pd_class_name, object_info, paths.pd_framelib(), "framelib_pd_setup(", "}")
    code_util.insert_object_list_include(object_info)
    
    fl_solution().update(object_info, True)
    fl_pbxproj().update(object_info, True)
    
    
def main():

    new_object(fl_object("FrameLib_Test", "fl.test~", "Schedulers"))
    rebuild()
    
    
if __name__ == "__main__":
    main()
