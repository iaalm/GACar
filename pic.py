#! /usr/bin/python3
from matplotlib import pyplot as plt

for i in range(4):
	count = 0
	plt.subplot(220+i+1)
	plt.axis((-20,20,-20,20))
	with open("%d" % i,"r") as f:
		for line in f.readlines():
			if line[0] != '\t':
				b = line.split("\t")
				before = [float(b[0]),float(b[1])]
				plt.plot([float(b[0]) - 1,float(b[0]) + 1],[float(b[1]),float(b[1])],"g")
				print("%d:\t%d" % (i,count))
				count = count + 1
			else:
				n = line.split("\t")
				now = [float(n[1]),float(n[2])]
				plt.plot([before[0],now[0]],[before[1],now[1]])
				#print(before+now)
				before = now
plt.show()
