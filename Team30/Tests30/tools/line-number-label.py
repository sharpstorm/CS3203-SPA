## Author: ZH (sharpstorm)

import argparse
from os.path import exists
import os
import sys

class style:
    MAGENTA = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    RESET = '\033[0m'

def parseSource(args):
    inputFile = args.input_source
    if not exists(inputFile):
        print('Input file not found')
        sys.exit(1)

    with open(inputFile, 'r') as f:
        af = f.read()
    
    result = []
    buffer = ''
    for i in range(len(af)):
        if af[i] == ';' or af[i] == '{' or af[i] == '}':
            buffer = buffer.replace('\n', '')
            buffer = buffer.replace('\t', ' ')
            elseStripped = buffer.replace(' ', '')
            if elseStripped == 'else':
                result[-1] += ' ' + (buffer + af[i]).strip()
            else:
                result.append((buffer + af[i]).strip())
            buffer = ''
        else:
            buffer += af[i]

    counter = 1
    level = 0
    for x in result:    
        y = x.replace('{', '')
        y = y.replace(';', '')
        y = y.replace('}', '')
        y = y.replace(' ', '')

        if x.endswith('}') or x.startswith('}'):
            level -= 1

        indent = '.'*(level * 2)
        if y.startswith('procedure'):
            print(f'    | {x}')
        elif y == 'else':
            print(f'    | {indent} {x}')
        elif y == '':
            print(f'    | {indent} {x}')
        else:
            print(f'{counter:3d} | {indent} {x}')
            counter += 1
        
        if x.endswith('{'):
            level += 1


def main():
    parser = argparse.ArgumentParser(
        prog='line-number-label.py'
    )
    parser.add_argument('input_source', help='The input source file')

    args = parser.parse_args()
    parseSource(args)

main()
