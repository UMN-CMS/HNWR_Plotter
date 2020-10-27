import os

lines = open('output.txt').readlines()
arrays = []

for line in lines:
  words = line.split()
  print words
