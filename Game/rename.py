import os
import re

class_replace = {}
struct_replace = {}
func_replace = {}

def snakify_name(name):
	result = name
	num_replaced = 0
	for i, c, in enumerate(result):
		if i == 0:
			continue

		if c >= 'A' and c <= 'Z':
			result = result[:(i + num_replaced)] + "_" + result[(i + num_replaced):]
			num_replaced += 1

	return result

def process_file(file):
	f = open(file, 'r')
	lines = f.readlines()

	for line in lines:
		m = re.search('^class (\w+)', line)

		if m != None:
			for i in range(1, len(m.groups()) + 1):
				name = m.group(i)
				print(name + " => " + snakify_name(name))

		m = re.search('^struct (\w+)', line)
		if m != None:
			for i in range(1, len(m.groups()) + 1):
				name = m.group(i)
				print(name + " => " + snakify_name(name))

rootdir = "src"

for subdir, dirs, files in os.walk(rootdir):
	for file in files:
		process_file(os.path.join(subdir, file))