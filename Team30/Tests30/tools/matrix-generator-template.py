## Author: ZH (sharpstorm)

import argparse
from os.path import exists
import os
import sys
import json

class style:
    MAGENTA = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    RESET = '\033[0m'

def writeQueries(templateResult, args):
    startingIndex = 1 if args.start_index is None else int(args.start_index)
    with open(args.output_file, 'w+') as f:
        for x in range(len(templateResult)):
            a = templateResult[x]
            f.write(f'{a}\n')

def readConfig(config):
    templates = config['templates']
    varying = config['varying']

    templateResult = []
    for x in templates:
        templateResult.append(x['template'])

    for x in range(len(varying)):
        curItems = varying[x]
        newList = []
        for y in curItems:
            for z in range(len(templateResult)):
                query = templateResult[z]
                rQuery = query.replace(f'[{x}]', y)
                if rQuery not in newList:
                    newList.append(rQuery)

        templateResult = newList
    return templateResult

def parseJSON(args):
    inputFile = args.input_json
    if not exists(inputFile):
        print('Input file not found')
        sys.exit(1)

    with open(inputFile, 'r') as f:
        data = f.read()

    data = json.loads(data)
    readData = readConfig(data)
    writeQueries(readData, args)
    print(f'Done writing {len(readData)} templates')

def main():
    parser = argparse.ArgumentParser(
        prog='matrix-generator-template.py'
    )
    parser.add_argument('input_json', help='The matrix input data')
    parser.add_argument('output_file', help='The query file to output to')
    parser.add_argument('-i', '--start-index', help='The test case ID to start from', action='store')

    args = parser.parse_args()
    parseJSON(args)

main()
