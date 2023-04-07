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
    showDeclare = args.declare
    if not exists(inputFile):
        print('Input file not found')
        sys.exit(1)

    with open(inputFile, 'r') as f:
        af = f.read()
    
    
    counter = 0
    for line in af.split('\n'):
        if counter % 5 == 0:
            print(f'{style.CYAN}{line}{style.RESET}')
        elif counter % 5 == 1 and showDeclare:
            print(f'{style.YELLOW}{line}{style.RESET}')
        elif counter % 5 == 2:
            print(line)
        elif counter % 5 == 3:
            print(f'{style.GREEN}{line}{style.RESET}')
        elif counter % 5 == 4:
            print()
            input(':Enter to Continue')
            print(chr(27) + "["+str(7 + (1 if showDeclare else 0))+"A")
            print(chr(27) + "[0J")
        counter += 1

def main():
    parser = argparse.ArgumentParser(
        prog='test-case-readback.py'
    )
    parser.add_argument('input_source', help='The input source file')
    parser.add_argument('-d', '--declare', help='Show declarations', action='store_true')

    args = parser.parse_args()
    print()
    parseSource(args)
    

main()
