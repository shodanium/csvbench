#!/usr/bin/env python3

import argparse
import random
import string
import sys

MAX_NUMBER = 2**32 - 1

MAX_ENTRY_LEN = 15
MAX_SPACE_LEN = 3


def generate(n):
	alphabet = string.ascii_uppercase + string.ascii_lowercase + string.digits
	spaces = [' ', '\t']
	line_breaks = ['\n', '\r\n']

	for _ in range(n):
		entry_len = random.randint(1, MAX_ENTRY_LEN)
		entry = ''.join(random.choice(alphabet) for a in range(entry_len))
		num = random.randint(0, MAX_NUMBER)

		space_len = random.randint(1, MAX_SPACE_LEN)
		space = ''.join(random.choice(spaces) for a in range(space_len))

		eol = random.choice(line_breaks)

		print('%s,%s%d%s' % (entry, space, num, eol), end='')


if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('-n', help='number of strings to generate', type=int, required=True)
	args = parser.parse_args()

	if args.n < 1:
		print('n must be greater than 0', file=sys.stderr)
		exit(1)

	random.seed(12345)
	generate(args.n)