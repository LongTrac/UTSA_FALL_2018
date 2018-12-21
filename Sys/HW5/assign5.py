#!/usr/bin/env python3
import os
import sys
import glob
import re
import shutil as sh

#extracting the arguments
args= list(x for x in sys.argv[:])

#create the output dir if not already exist
if not os.path.exists(args[4]):
    os.makedirs(args[4])

#extract all the file from ./data dir and use globing
for file_name in glob.glob(os.path.join(args[1],'[0-9][0-9][0-9][0-9].item')):
    try:
        in_file= open(file_name,'r') 
        #parse the items
        lines = in_file.readlines()

        s_name = lines[0].split()[0]
        i_name = ' '.join(lines[0].split()[1:])
        qty = lines[1].split()[0]
        max_qty = lines[1].split()[1]
        body = lines[-1]
    except IOError:  
        print('ERROR OPENING A FILE')
    except:
        print('something unexpected happened')
        sys.exit(1)

    in_file.close()

    #copy file
    item_file_name = re.sub(r"^.*(\d{4}.item)$",r'\1' ,file_name)
    scr = './'+args[2]
    dest = args[4]+'/'+item_file_name

    #if less than 10% then process
    if int(qty)/int(max_qty)*100 < 10 and len(args)==7:
        try:
            sh.copyfile(scr,dest)
        except:
            print('FILE CAN NOT BE COPIED')

        try:
            with open (dest ,'r') as edit_file:
                item_string = edit_file.read()

            str_len_byte = len(item_string)
            item_string = re.sub( re.escape(args[5])+r'item_name'+re.escape(args[6]),i_name,item_string)
            item_string = re.sub( re.escape(args[5])+r'simple_name'+re.escape(args[6]),s_name,item_string)
            item_string = re.sub( re.escape(args[5])+r'current_quantity'+re.escape(args[6]),qty,item_string)
            item_string = re.sub( re.escape(args[5])+r'max_quantity'+re.escape(args[6]),max_qty,item_string)
            item_string = re.sub( re.escape(args[5])+r'body'+re.escape(args[6]),body,item_string)
            item_string = re.sub( re.escape(args[5])+r'date'+re.escape(args[6]),args[3],item_string)

            edit_file = open (dest ,'w')
            edit_file.write(item_string)
        except IOError:
            print('FILE CAN NOT BE OPEN')
        edit_file.close()

    if int(qty)/int(max_qty)*100 < 10 and len(args)==5:
            try:
                sh.copyfile(scr,dest)
            except:
                print('FILE CAN NOT BE COPIED')

            try:
                with open (dest ,'r') as edit_file:
                    item_string = edit_file.read()

                str_len_byte = len(item_string)
                item_string = re.sub(r'<<item_name>>',i_name,item_string)
                item_string = re.sub(r'<<simple_name>>',s_name,item_string)
                item_string = re.sub(r'<<current_quantity>>',qty,item_string)
                item_string = re.sub(r'<<max_quantity>>',max_qty,item_string)
                item_string = re.sub(r'<<body>>',body,item_string)
                item_string = re.sub(r'<<date>>',args[3],item_string)

                edit_file = open (dest ,'w')
                edit_file.write(item_string)
            except IOError:
                print('FILE CAN NOT BE OPEN')
            edit_file.close()