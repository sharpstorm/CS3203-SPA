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
            result.append(buffer + af[i])
            buffer = ''
        else:
            buffer += af[i]

    counter = 1
    for x in result:
        y = x.replace('{', '')
        y = y.replace(';', '')
        y = y.replace('}', '')
        y = y.replace(' ', '')

        if y.startswith('procedure'):
            print(f'    |  {x}')
        elif y == 'else':
            print(f'    |  {x}')
        elif y == '':
            print(f'    |  {x}')
        else:
            print(f'{counter:3d} |  {x}')
            counter += 1


def main():
    parser = argparse.ArgumentParser(
        prog='line-number-label.py'
    )
    parser.add_argument('input_source', help='The input source file')

    args = parser.parse_args()
    parseSource(args)

main()
