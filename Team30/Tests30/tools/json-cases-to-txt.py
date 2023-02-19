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

def writeQueries(readData, args):
    cases = readData[0]
    timeout = readData[1]
    
    startingIndex = 1 if args.start_index is None else int(args.start_index)
    
    counter = 0
    with open(args.output_file, 'w+') as f:
        for x in range(len(cases)):
            case = cases[x]
            if not ('name' in case):
                continue
            
            name = case['name']
            defn = case['defn']
            query = case['query']
            ans = case['ans']
            f.write(f'{startingIndex + counter} - {name}\n')
            f.write(f'{defn}\n')
            f.write(f'{query}\n')
            f.write(f'{ans}\n')
            f.write(f'{timeout}\n')
            counter += 1
    return counter

def readConfig(config):
    timeout = config['timeout']

    return [config['cases'], timeout]

def parseJSON(args):
    inputFile = args.input_json
    if not exists(inputFile):
        print('Input file not found')
        sys.exit(1)

    with open(inputFile, 'r') as f:
        data = f.read()

    data = json.loads(data)
    readData = readConfig(data)
    wCount = writeQueries(readData, args)
    print(f'Done writing {wCount} queries')

def main():
    parser = argparse.ArgumentParser(
        prog='json-cases-to-txt.py'
    )
    parser.add_argument('input_json', help='The matrix input data')
    parser.add_argument('output_file', help='The query file to output to')
    parser.add_argument('-i', '--start-index', help='The test case ID to start from', action='store')

    args = parser.parse_args()
    parseJSON(args)

main()
