#converts all .docx files in a directory to a single json file for pretraining suiting Alpaca Format
#input: directory path containing .docx files
#output: path of the output json file
#allows the option for converting GB2312(simplified chinese) to UTF-8

import docx
import os
import codecs

print("input files directory path: ")
input_path = input()
print("output file path: ")
output_path = input()
print("convert GB2312 to UTF-8? (y/n): ")
convert = input()

if convert == 'y':
    convert=True
else:
    convert=False

files=[]
for file in os.listdir(input_path):
    if file.endswith(".docx"):
        files.append(file)
 
def getText(filename):
    doc = docx.Document(filename)
    return '\\n'.join([p.text for p in doc.paragraphs])

result="["
for file in files:
   result+= "{\"text\":\""+getText(input_path+"\\"+file)+"\"},"

result=result[:-1]+"]"


def convert_gb2312_to_utf8(input_path, output_path):
    with codecs.open(input_path, 'r', 'gb2312') as input_file:
        content = input_file.read()
    
    with codecs.open(output_path, 'w', 'utf-8') as output_file:
        output_file.write(content)
if convert:
    with open('temp.txt','w') as f:
        f.write(result)
    convert_gb2312_to_utf8('temp.txt', output_path)
    os.remove('temp.txt')
else:
    with open(output_path,'w') as f:
        f.write(result)
