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
    queryResult = readData[0]
    commentResult = readData[1]
    timeout = readData[2]
    startingIndex = 1 if args.start_index is None else int(args.start_index)
    with open(args.output_file, 'w+') as f:
        for x in range(len(queryResult)):
            a = queryResult[x]
            b = commentResult[x]
            f.write(f'{startingIndex + x} - {b}\n')
            f.write(f'{a}\n')
            f.write('\n')
            f.write(f'{timeout}\n')

def readConfig(config):
    timeout = config['timeout']
    queries = config['queries']
    varying = config['varying']

    queryResult = []
    commentResult = []
    for x in queries:
        queryResult.append(x['query'])
        commentResult.append(x['comment'])

    for x in range(len(varying)):
        curItems = varying[x]
        newQueryList = []
        newCommentList = []
        for y in curItems:
            for z in range(len(queryResult)):
                query = queryResult[z]
                comment = commentResult[z]
                rQuery = query.replace(f'[{x}]', y)
                if rQuery not in newQueryList:
                    newQueryList.append(rQuery)
                    newCommentList.append(comment.replace(f'[{x}]', y))

        queryResult = newQueryList
        commentResult = newCommentList
    
    return [queryResult, commentResult, timeout]

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
    print(f'Done writing {len(readData[0])} queries')

def main():
    parser = argparse.ArgumentParser(
        prog='matrix-generator.py'
    )
    parser.add_argument('input_json', help='The matrix input data')
    parser.add_argument('output_file', help='The query file to output to')
    parser.add_argument('-i', '--start-index', help='The test case ID to start from', action='store')

    args = parser.parse_args()
    parseJSON(args)

main()
