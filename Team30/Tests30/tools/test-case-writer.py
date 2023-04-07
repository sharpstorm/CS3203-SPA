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

def writeStuff(args, timeout, decl):
    outputFile = args.output_queries
    output = ""
    nameCache = ""
    answerCache = ""
    counter = 1

    try:
        while True:
            nameInpAppend = " [" + nameCache + "]" if (len(nameCache) > 0) else ""
            ansInpAppend = " [" + answerCache + "]" if (len(answerCache) > 0) else ""
            name = input(f'{style.CYAN}Name{nameInpAppend}: {style.RESET}') or nameCache
            query = input(f'{style.BLUE}Query: {style.RESET}')
            answer = input(f'{style.YELLOW}Answer{ansInpAppend}: {style.RESET}') or answerCache
            nameCache = name
            answerCache = answer

            output += str(counter) + " - " + name + "\n"
            output += decl + "\n"
            output += query + "\n"
            output += answer + "\n"
            output += timeout + "\n"

            counter += 1
            print(chr(27) + "[5A")
            print(chr(27) + "[0J")
    except:
        pass

    with open(outputFile, 'w') as f:
        f.write(output)
    
    print()
    print(f'{style.MAGENTA}Written {counter - 1} Cases{style.RESET}')

def main():
    parser = argparse.ArgumentParser(
        prog='test-case-readback.py'
    )
    parser.add_argument('output_queries', help='The output path')

    args = parser.parse_args()
    timeout = input("Enter Timeout: ")
    decl = input("Enter Declaration: ")
    print()
    writeStuff(args, timeout, decl)

main()
