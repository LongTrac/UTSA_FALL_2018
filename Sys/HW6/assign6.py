#!/usr/bin/env python3
import os
import sys
import shutil as sh
import re
import zipfile 

#   extracting the arguments
args= list(x for x in sys.argv[:])
#   check if the dir exist
if not (os.path.isdir(args[1])):
    print('ERROR: The directory does not exist')
    exit(1)

#   file_list       - all file and dir listed inside the designated directory
#   extract_name    - the regex part that we need to create the assignment folder
#   whole_name      - the complete name before extraction of files only
file_list = os.listdir(args[1])
extract_name = []
whole_name = []
pattern = r'^[pP][rR][oO][jJ](.*)\..*$'

#   extracting regex and store regex + whole name
for f in file_list:
    if os.path.isfile(args[1]+'/'+f):
        matches = re.search(pattern, f)
        #  if not match, object return is NoneType (None)
        if matches is None:
            extract_name.append('misc')
        else:
            extract_name.append(matches.group(1))
        whole_name.append(f)
            
#   dir_path_name    - the name of all dirs we need to create
#   src_path_name    - the source files' paths
#   des_path_name    - the destination file's paths
dir_path_name = list(map(lambda a: args[1]+'/'+a if a=='misc' else args[1]+'/assignment'+a, extract_name ))
src_path_name = list(map(lambda b: args[1]+'/'+b, whole_name))
des_path_name = []
for i in range(len(extract_name)):
    des_path_name.append(dir_path_name[i] +'/'+ whole_name[i])

#   creating sub-directory
for name in dir_path_name:
    if not os.path.exists(name):
        os.makedirs(name)

#   moving files to the correct location
for i in range(len(src_path_name)):
    try:
        sh.move(src_path_name[i], des_path_name[i])
    except IOError:
        print('Can not move file!!!')
        continue
    except:
        print('Something Unexpected thing happened with this file')
        continue

#Extra Credit===============================
#   zip_prefix      - the extraact name that we want to zip
#   zip_name        - the name of the zip_file 
#   abs_path        - the absolute path to the directory that we want to zip
#   final_zip_path  - the final location of the zip file
#   zip_file_list   - the list of file that we will zip together

if len(args) == 3:
    zip_prefix = args[2]
    zip_name = 'assignment'+zip_prefix+'.zip'
    abs_path = args[1]+'/assignment'+zip_prefix
    final_zip_path = args[1]+'/'+zip_name
    try:
        zip_file_list = os.listdir(abs_path)
    except FileNotFoundError:
        print('ERROR: File Not Found')
        exit(1)
    except:
        print('Something Unexpected thing happened with this list of file')
        exit(1)

    try:   
        with zipfile.ZipFile(final_zip_path,'w',zipfile.ZIP_DEFLATED) as my_zip:
            for f in zip_file_list:
                get_file = os.path.join(abs_path,f)
                my_zip.write(get_file,'./'+f)       
    except:
        print('Something Unexpected thing happened with this file')
        exit(1)